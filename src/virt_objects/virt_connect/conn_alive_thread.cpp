#include "conn_alive_thread.h"
//#include <QTextStream>

#define WAIT_AUTH 300    // dev 10 (sec.)

/* The list of credential types supported by our auth callback */
static int credTypes[] = {
    VIR_CRED_AUTHNAME,
    VIR_CRED_PASSPHRASE
};

/* The auth struct that will be passed to virConnectOpenAuth */
static virConnectAuth auth = {
    credTypes,
    sizeof(credTypes) / sizeof(int),
    nullptr, // cb will be initialized in thread
    nullptr, // cbdata will be initialized in thread
};

ConnAliveThread::ConnAliveThread(QObject *parent) :
    _VirtThread(parent)
{
    qRegisterMetaType<CONN_STATE>("CONN_STATE");
    _connPtr = nullptr;
    onView = false;
    // -1 on error; another is success or a callback identifier
    closeCallbackRegistered     = false;
    domainsLifeCycleCallback    = -1;
    networksLifeCycleCallback   = -1;
#if LIBVIR_VERSION_NUMBER >= 2000000
    poolsLifeCycleCallback      = -1;
#endif
#if LIBVIR_VERSION_NUMBER >= 3000000
    secretsLifeCycleCallback    = -1;
#endif
}
ConnAliveThread::~ConnAliveThread()
{
    keep_alive = false;
}
void ConnAliveThread::setOnViewState(bool state)
{
    onView = state;
}
bool ConnAliveThread::getOnViewState() const
{
    return onView;
}
virConnectPtr *ConnAliveThread::getPtr_connectionPtr() const
{
    return ptr_ConnPtr;
}
void ConnAliveThread::setData(const QString &uri) { URI = uri; }
void ConnAliveThread::closeConnection()
{
    //qDebug()<<"closeConnection0"<<*ptr_ConnPtr<<URI;
    /*
    if ( keep_alive ) {
        keep_alive = false;
        return;
    };
    */
    //qDebug()<<"closeConnection1"<<*ptr_ConnPtr<<URI;
    CONN_STATE state;
    if ( nullptr!=ptr_ConnPtr && *ptr_ConnPtr!=nullptr ) {
        //qDebug()<<"closeConnection2"<<*ptr_ConnPtr<<URI;
        unregisterConnEvents();
        //qDebug()<<"closeConnection3"<<*ptr_ConnPtr<<URI;
        int ret = virConnectClose(*ptr_ConnPtr);
        *ptr_ConnPtr = nullptr;
        //qDebug()<<"virConnectRef -1"<<"ConnAliveThread"<<URI<<(ret+1>0);
        if ( ret<0 ) {
            state = FAILED;
            sendConnErrors();
        } else {
            emit connMsg( QString("close exit code: %1").arg(ret) );
            state = CLOSED;
            emit connClosed(onView);
        };
    } else {
        emit connMsg( QString("connect is nullptr") );
        state = FAILED;
    };
    emit changeConnState(state);
}
void ConnAliveThread::setAuthCredentials(const QString &crd, const QString &text)
{
    if ( crd.toLower()=="username" )
        authData.username = text.toUtf8().data();
    if ( crd.toLower()=="password" )
        authData.password = text.toUtf8().data();
    authWaitKey = false;
}

/* private */
void ConnAliveThread::run()
{
    openConnection();
    while ( keep_alive && nullptr!=*ptr_ConnPtr ) {
        msleep(500);
    };
    keep_alive = false;
    //closeConnection();
}
void ConnAliveThread::openConnection()
{
    //qDebug()<<"openConnection0"<<ptr_ConnPtr<<URI;
    auth.cb = authCallback;
    auth.cbdata = this;
    _connPtr = virConnectOpenAuth(URI.toUtf8().constData(), &auth, 0);
    ptr_ConnPtr = ( nullptr!=_connPtr )? &_connPtr : nullptr;
    //qDebug()<<"openConnection1"<<*ptr_ConnPtr<<URI;
    if ( nullptr==ptr_ConnPtr || *ptr_ConnPtr==nullptr ) {
        sendConnErrors();
        keep_alive = false;
        emit connMsg( "Connection to the Hypervisor is failed." );
        emit changeConnState(FAILED);
    } else {
        //qDebug()<<" openConnection"<<*ptr_ConnPtr<<URI;
        keep_alive = true;
        emit connMsg( QString("connect opened: %1")
                      .arg(QVariant(*ptr_ConnPtr!=nullptr)
                           .toString()) );
        emit changeConnState(RUNNING);
        registerConnEvents();
    };
    //qDebug()<<"virConnectRef +1"<<"ConnAliveThread"<<URI<<(*ptr_ConnPtr!=nullptr);
}
void ConnAliveThread::registerConnEvents()
{
    int ret = virConnectRegisterCloseCallback(
                *ptr_ConnPtr,
                connEventCallBack,
                this,
    // don't register freeData, because it remove this thread
                nullptr);
    closeCallbackRegistered = !(ret<0);
    if (ret<0) sendConnErrors();

    domainsLifeCycleCallback = virConnectDomainEventRegisterAny(
                *ptr_ConnPtr,
                nullptr,
    // set domainsLifeCycleCallback signature
                VIR_DOMAIN_EVENT_ID_LIFECYCLE,
                VIR_DOMAIN_EVENT_CALLBACK(domEventCallback),
                this,
    // don't register freeData, because it remove this thread
                nullptr);
    if (domainsLifeCycleCallback<0) sendConnErrors();

    networksLifeCycleCallback = virConnectNetworkEventRegisterAny(
                *ptr_ConnPtr,
                nullptr,
    // set networksLifeCycleCallback signature
                VIR_NETWORK_EVENT_ID_LIFECYCLE,
                VIR_NETWORK_EVENT_CALLBACK(netEventCallback),
                this,
    // don't register freeData, because it remove this thread
                nullptr);
    if (networksLifeCycleCallback<0) sendConnErrors();

#if LIBVIR_VERSION_NUMBER >= 2000000
    poolsLifeCycleCallback = virConnectStoragePoolEventRegisterAny(
                *ptr_ConnPtr,
                nullptr,
    // set poolsLifeCycleCallback signature
                VIR_STORAGE_POOL_EVENT_ID_LIFECYCLE,
                VIR_STORAGE_POOL_EVENT_CALLBACK(poolEventCallback),
                this,
    // don't register freeData, because it remove this thread
                nullptr);
    if (poolsLifeCycleCallback<0) sendConnErrors();
#endif

#if LIBVIR_VERSION_NUMBER >= 3000000
    secretsLifeCycleCallback = virConnectSecretEventRegisterAny(
                *ptr_ConnPtr,
                nullptr,
    // set secretsLifeCycleCallback signature
                VIR_SECRET_EVENT_ID_LIFECYCLE,
                VIR_SECRET_EVENT_CALLBACK(secEventCallback),
                this,
    // don't register freeData, because it remove this thread
                nullptr);
    if (secretsLifeCycleCallback<0) sendConnErrors();
#endif
}
void ConnAliveThread::unregisterConnEvents()
{
    //qDebug()<<"unregisterConnEvents0"<<*ptr_ConnPtr<<URI;
    if ( closeCallbackRegistered ) {
        int ret = virConnectUnregisterCloseCallback(
                    *ptr_ConnPtr, connEventCallBack);
        if (ret<0) sendConnErrors();
        else closeCallbackRegistered = false;
    };
    if ( domainsLifeCycleCallback>=0 ) {
        int ret = virConnectDomainEventDeregisterAny(
                    *ptr_ConnPtr, domainsLifeCycleCallback);
        if (ret<0) sendConnErrors();
        domainsLifeCycleCallback = -1;
    };
    if ( networksLifeCycleCallback>=0 ) {
        int ret = virConnectNetworkEventDeregisterAny(
                    *ptr_ConnPtr, networksLifeCycleCallback);
        if (ret<0) sendConnErrors();
        networksLifeCycleCallback = -1;
    };

#if LIBVIR_VERSION_NUMBER >= 2000000
    if ( poolsLifeCycleCallback>=0 ) {
        int ret = virConnectStoragePoolEventDeregisterAny(
                    *ptr_ConnPtr, poolsLifeCycleCallback);
        if (ret<0) sendConnErrors();
        poolsLifeCycleCallback = -1;
    };
#endif

#if LIBVIR_VERSION_NUMBER >= 3000000
    if ( secretsLifeCycleCallback>=0 ) {
        int ret = virConnectSecretEventDeregisterAny(
                    *ptr_ConnPtr, secretsLifeCycleCallback);
        if (ret<0) sendConnErrors();
        secretsLifeCycleCallback = -1;
    };
#endif

    //qDebug()<<"unregisterConnEvents1"<<*ptr_ConnPtr<<URI;
}
void ConnAliveThread::freeData(void *opaque)
{
    if ( opaque!=nullptr ) {
        void *data = opaque;
        free(data);
    }
}
void ConnAliveThread::connEventCallBack(virConnectPtr _conn, int reason, void *opaque)
{
    //qDebug()<<"connEventCallBack"<<_conn;
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(opaque);
    if ( nullptr!=obj && *(obj->ptr_ConnPtr)==_conn) {
        obj->closeConnection(reason);
    };
}
int  ConnAliveThread::authCallback(virConnectCredentialPtr cred, unsigned int ncred, void *cbdata)
{
    //qDebug()<<ncred<<"keep auth";
    size_t i;
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(cbdata);
    if ( nullptr==obj ) return -1;

    /* libvirt might request multiple credentials in a single call.
     * This example supports VIR_CRED_AUTHNAME and VIR_CRED_PASSPHRASE
     * credentials only, but there are several other types.
     *
     * A request may also contain a prompt message that can be displayed
     * to the user and a challenge. The challenge is specific to the
     * credential type and hypervisor type.
     *
     * For example the ESX driver passes the hostname of the ESX or vCenter
     * server as challenge. This allows a auth callback to return the
     * proper credentials. */
    obj->authData.username = nullptr;
    obj->authData.password = nullptr;
    QString crd;
    for (i = 0; i < ncred; ++i) {
        switch (cred[i].type) {
            case VIR_CRED_AUTHNAME:
                crd = "Username";
                obj->getAuthCredentials(crd);
                cred[i].result = strdup(obj->authData.username);
                if (cred[i].result == nullptr) {
                    return -1;
                };
                cred[i].resultlen = strlen(cred[i].result);
                // clear/shred authData credential for more security
                if ( obj->authData.username!=nullptr )
                    memset(&obj->authData.username[0], 0, strlen(obj->authData.username));
                break;
            case VIR_CRED_PASSPHRASE:
                crd = "Password";
                obj->getAuthCredentials(crd);
                cred[i].result = strdup(obj->authData.password);
                if (cred[i].result == nullptr) {
                    return -1;
                };
                cred[i].resultlen = strlen(cred[i].result);
                // clear/shred authData credential for more security
                if ( obj->authData.password!=nullptr )
                    memset(&obj->authData.password[0], 0, strlen(obj->authData.password));
                break;
            default:
                qDebug()<<cred[i].type<<"unused credential type";
                return -1;
        };
    };
    return 0;
}
int  ConnAliveThread::domEventCallback(virConnectPtr _conn, virDomainPtr dom, int event, int detail, void *opaque)
{
    //QTextStream s(stdout); s<<"domEventCallback "<<_conn;
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(opaque);
    if ( nullptr==obj || *(obj->ptr_ConnPtr)!=_conn ) return 0;
    bool end = false;
    QString msg, domainName;
    domainName = QString(virDomainGetName(dom));
    msg = QString("<b>'%1'</b> Domain %2: %3\n")
           .arg(domainName)
           .arg(obj->domEventToString(event))
           .arg(obj->domEventDetailToString(event, detail, &end));
    if ( end ) emit obj->domainEnd(domainName);
    emit obj->connMsg(msg);
    if ( obj->onView ) {
        emit obj->domStateChanged();
    };
    return 0;
}
int  ConnAliveThread::netEventCallback(virConnectPtr _conn, virNetworkPtr net, int event, int detail, void *opaque)
{
    //QTextStream s(stdout); s<<"netEventCallback "<<_conn;
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(opaque);
    if ( nullptr==obj || *(obj->ptr_ConnPtr)!=_conn ) return 0;
    QString msg;
    msg = QString("<b>'%1'</b> Network %2: %3\n")
           .arg(virNetworkGetName(net))
           .arg(obj->netEventToString(event))
           .arg(obj->netEventDetailToString(event, detail));
    emit obj->connMsg(msg);
    if ( obj->onView ) {
        emit obj->netStateChanged();
    };
    return 0;
}

#if LIBVIR_VERSION_NUMBER >= 2000000
int  ConnAliveThread::poolEventCallback(virConnectPtr _conn, virStoragePoolPtr pool, int event, int detail, void *opaque)
{
    //QTextStream s(stdout); s<<"poolEventCallback "<<_conn;
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(opaque);
    if ( nullptr==obj || *(obj->ptr_ConnPtr)!=_conn ) return 0;
    QString msg;
    msg = QString("<b>'%1'</b> Pool %2: %3\n")
           .arg(virStoragePoolGetName(pool))
           .arg(obj->poolEventToString(event))
           .arg(obj->poolEventDetailToString(event, detail));
    emit obj->connMsg(msg);
    if ( obj->onView ) {
        emit obj->poolStateChanged();
    };
    return 0;
}
#endif

#if LIBVIR_VERSION_NUMBER >= 3000000
int  ConnAliveThread::secEventCallback(virConnectPtr _conn, virSecretPtr sec, int event, int detail, void *opaque)
{
    //QTextStream s(stdout); s<<"secEventCallback "<<_conn;
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(opaque);
    if ( nullptr==obj || *(obj->ptr_ConnPtr)!=_conn ) return 0;
    QString msg;
    msg = QString("<b>'%1'</b> Secert %2: %3\n")
           .arg(virSecretGetUsageID(sec))
           .arg(obj->secEventToString(event))
           .arg(obj->secEventDetailToString(event, detail));
    emit obj->connMsg(msg);
    if ( obj->onView ) {
        emit obj->secStateChanged();
    };
    return 0;
}
#endif

const char* ConnAliveThread::domEventToString(int event)
{
    const char *ret = "";
    switch ((virDomainEventType) event) {
        case VIR_DOMAIN_EVENT_DEFINED:
            ret = "Defined";
            break;
        case VIR_DOMAIN_EVENT_UNDEFINED:
            ret = "Undefined";
            break;
        case VIR_DOMAIN_EVENT_STARTED:
            ret = "Started";
            break;
        case VIR_DOMAIN_EVENT_SUSPENDED:
            ret = "Suspended";
            break;
        case VIR_DOMAIN_EVENT_RESUMED:
            ret = "Resumed";
            break;
        case VIR_DOMAIN_EVENT_STOPPED:
            ret = "Stopped";
            break;
        case VIR_DOMAIN_EVENT_SHUTDOWN:
            ret = "Shutdown";
            break;
        case VIR_DOMAIN_EVENT_PMSUSPENDED:
            ret = "PMSuspended";
            break;
        case VIR_DOMAIN_EVENT_CRASHED:
            ret = "Crashed";
            break;
        default:
            ret = "Unknown";
            break;
    };
    return ret;
}
const char* ConnAliveThread::domEventDetailToString(int event, int detail, bool *end)
{
    const char *ret = "";
    switch ((virDomainEventType) event) {
        case VIR_DOMAIN_EVENT_DEFINED:
            switch ((virDomainEventDefinedDetailType) detail) {
            case VIR_DOMAIN_EVENT_DEFINED_ADDED:
                ret = "Newly created config file";
                break;
            case VIR_DOMAIN_EVENT_DEFINED_UPDATED:
                ret = "Changed config file";
                break;
#if LIBVIR_VERSION_NUMBER >= 1002020
            case VIR_DOMAIN_EVENT_DEFINED_RENAMED:
                ret = "Domain was renamed";
                break;
#endif
#if LIBVIR_VERSION_NUMBER >= 1003003
            case VIR_DOMAIN_EVENT_DEFINED_FROM_SNAPSHOT:
                ret = "Config was restored from a snapshot";
                break;
#endif
            default:
                break;
            };
            break;
        case VIR_DOMAIN_EVENT_UNDEFINED:
            switch ((virDomainEventUndefinedDetailType) detail) {
            case VIR_DOMAIN_EVENT_UNDEFINED_REMOVED:
                ret = "Deleted the config file";
                break;
#if LIBVIR_VERSION_NUMBER >= 1002020
            case VIR_DOMAIN_EVENT_UNDEFINED_RENAMED:
                ret = "Domain was renamed";
                break;
#endif
            default:
                break;
            };
            break;
        case VIR_DOMAIN_EVENT_STARTED:
            switch ((virDomainEventStartedDetailType) detail) {
                case VIR_DOMAIN_EVENT_STARTED_BOOTED:
                    ret = "Normal startup from boot";
                    break;
                case VIR_DOMAIN_EVENT_STARTED_MIGRATED:
                    ret = "Incoming migration from another host";
                    break;
                case VIR_DOMAIN_EVENT_STARTED_RESTORED:
                    ret = "Restored from a state file";
                    break;
                case VIR_DOMAIN_EVENT_STARTED_FROM_SNAPSHOT:
                    ret = "Restored from snapshot";
                    break;
                case VIR_DOMAIN_EVENT_STARTED_WAKEUP:
                    ret = "Started due to wakeup event";
                    break;
                default:
                    ret = "Unknown";
                    break;
            };
            break;
        case VIR_DOMAIN_EVENT_SUSPENDED:
            switch ((virDomainEventSuspendedDetailType) detail) {
                case VIR_DOMAIN_EVENT_SUSPENDED_PAUSED:
                    ret = "Normal suspend due to admin pause";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_MIGRATED:
                    ret = "Suspended for offline migration";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_IOERROR:
                    ret = "Suspended due to a disk I/O error";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_WATCHDOG:
                    ret = "Suspended due to a watchdog firing";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_RESTORED:
                    ret = "Restored from paused state file";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_FROM_SNAPSHOT:
                    ret = "Restored from paused snapshot";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_API_ERROR:
                    ret = "suspended after failure during libvirt API call";
                    break;
#if LIBVIR_VERSION_NUMBER >= 1003001
                case VIR_DOMAIN_EVENT_SUSPENDED_POSTCOPY:
                    ret = "suspended for post-copy migration";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_POSTCOPY_FAILED:
                    ret = "suspended after failed post-copy";
                    break;
#endif
                default:
                    ret = "Unknown";
                    break;
            };
            break;
        case VIR_DOMAIN_EVENT_RESUMED:
            switch ((virDomainEventResumedDetailType) detail) {
                case VIR_DOMAIN_EVENT_RESUMED_UNPAUSED:
                    ret = "Normal resume due to admin unpause";
                    break;
                case VIR_DOMAIN_EVENT_RESUMED_MIGRATED:
                    ret = "Resumed for completion of migration";
                    break;
                case VIR_DOMAIN_EVENT_RESUMED_FROM_SNAPSHOT:
                    ret = "Resumed from snapshot";
                    break;
#if LIBVIR_VERSION_NUMBER >= 1003001
                case VIR_DOMAIN_EVENT_RESUMED_POSTCOPY:
                    ret = "Resumed, but migration is still \
running in post-copy mode";
                    break;
#endif
                default:
                    ret = "Unknown";
                    break;
            };
            break;
        case VIR_DOMAIN_EVENT_STOPPED:
            switch ((virDomainEventStoppedDetailType) detail) {
                case VIR_DOMAIN_EVENT_STOPPED_SHUTDOWN:
                    ret = "Normal shutdown";
                    *end = true;
                    break;
                case VIR_DOMAIN_EVENT_STOPPED_DESTROYED:
                    ret = "Forced poweroff from host";
                    *end = true;
                    break;
                case VIR_DOMAIN_EVENT_STOPPED_CRASHED:
                    ret = "Guest crashed";
                    *end = true;
                    break;
                case VIR_DOMAIN_EVENT_STOPPED_MIGRATED:
                    ret = "Migrated off to another host";
                    break;
                case VIR_DOMAIN_EVENT_STOPPED_SAVED:
                    ret = "Saved to a state file";
                    break;
                case VIR_DOMAIN_EVENT_STOPPED_FAILED:
                    ret = "Host emulator/mgmt failed";
                    *end = true;
                    break;
                case VIR_DOMAIN_EVENT_STOPPED_FROM_SNAPSHOT:
                    ret = "offline snapshot loaded";
                    break;
                default:
                    ret = "Unknown";
                    *end = true;
                    break;
            };
            break;
        case VIR_DOMAIN_EVENT_SHUTDOWN:
            switch ((virDomainEventShutdownDetailType) detail) {
                case VIR_DOMAIN_EVENT_SHUTDOWN_FINISHED:
                    ret = "Finished";
                    *end = true;
                    break;
                default:
                    ret = "Unknown";
                    *end = true;
                    break;
                };
            break;
        case VIR_DOMAIN_EVENT_PMSUSPENDED:
            switch ((virDomainEventPMSuspendedDetailType) detail) {
            case VIR_DOMAIN_EVENT_PMSUSPENDED_MEMORY:
                ret = "Guest was PM suspended to memory";
                break;
            case VIR_DOMAIN_EVENT_PMSUSPENDED_DISK:
                ret = "Guest was PM suspended to disk";
                break;
            default:
                ret = "Unknown";
                break;
            };
            break;
        case VIR_DOMAIN_EVENT_CRASHED:
           switch ((virDomainEventCrashedDetailType) detail) {
           case VIR_DOMAIN_EVENT_CRASHED_PANICKED:
               ret = "Guest was panicked";
               break;
           default:
               ret = "Unknown";
               break;
           };
           *end = true;
           break;
        default:
            ret = "Unknown";
            *end = true;
            break;
    };
    return ret;
}
const char* ConnAliveThread::netEventToString(int event)
{
    const char *ret = "";
    switch ((virNetworkEventLifecycleType) event) {
        case VIR_NETWORK_EVENT_DEFINED:
            ret = "Defined";
            break;
        case VIR_NETWORK_EVENT_UNDEFINED:
            ret = "Undefined";
            break;
        case VIR_NETWORK_EVENT_STARTED:
            ret = "Started";
            break;
        case VIR_NETWORK_EVENT_STOPPED:
            ret = "Stopped";
            break;
        default:
            ret = "Unknown";
            break;
    };
    return ret;
}
const char* ConnAliveThread::netEventDetailToString(int event, int detail)
{
    const char *ret = "";
    switch ((virNetworkEventLifecycleType) event) {
        case VIR_NETWORK_EVENT_DEFINED:
            ret = "N0_DETAILS";
            break;
        case VIR_NETWORK_EVENT_UNDEFINED:
            ret = "N0_DETAILS";
            break;
        case VIR_NETWORK_EVENT_STARTED:
            ret = "N0_DETAILS";
            break;
        case VIR_NETWORK_EVENT_STOPPED:
            ret = "N0_DETAILS";
            break;
        default:
            ret = "N0_DETAILS";
            break;
    };
    return ret;
}

#if LIBVIR_VERSION_NUMBER >= 2000000
const char* ConnAliveThread::poolEventToString(int event)
{
    const char *ret = "";
    switch ((virStoragePoolEventLifecycleType) event) {
        case VIR_STORAGE_POOL_EVENT_DEFINED:
            ret = "Defined";
            break;
        case VIR_STORAGE_POOL_EVENT_UNDEFINED:
            ret = "Undefined";
            break;
        case VIR_STORAGE_POOL_EVENT_STARTED:
            ret = "Started";
            break;
        case VIR_STORAGE_POOL_EVENT_STOPPED:
            ret = "Stopped";
            break;
        default:
            ret = "Unknown";
            break;
    };
    return ret;
}
const char* ConnAliveThread::poolEventDetailToString(int event, int detail)
{
    const char *ret = "";
    switch ((virStoragePoolEventLifecycleType) event) {
        case VIR_STORAGE_POOL_EVENT_DEFINED:
            ret = "N0_DETAILS";
            break;
        case VIR_STORAGE_POOL_EVENT_UNDEFINED:
            ret = "N0_DETAILS";
            break;
        case VIR_STORAGE_POOL_EVENT_STARTED:
            ret = "N0_DETAILS";
            break;
        case VIR_STORAGE_POOL_EVENT_STOPPED:
            ret = "N0_DETAILS";
            break;
        default:
            ret = "N0_DETAILS";
            break;
    };
    return ret;
}
#endif

#if LIBVIR_VERSION_NUMBER >= 3000000
const char* ConnAliveThread::secEventToString(int event)
{
    const char *ret = "";
    switch ((virSecretEventLifecycleType) event) {
        case VIR_SECRET_EVENT_DEFINED:
            ret = "Defined";
            break;
        case VIR_SECRET_EVENT_UNDEFINED:
            ret = "Undefined";
            break;
        default:
            ret = "Unknown";
            break;
    };
    return ret;
}
const char* ConnAliveThread::secEventDetailToString(int event, int detail)
{
    const char *ret = "";
    switch ((virSecretEventLifecycleType) event) {
        case VIR_SECRET_EVENT_DEFINED:
            ret = "N0_DETAILS";
            break;
        case VIR_SECRET_EVENT_UNDEFINED:
            ret = "N0_DETAILS";
            break;
        default:
            ret = "N0_DETAILS";
            break;
    };
    return ret;
}
#endif

void ConnAliveThread::closeConnection(int reason)
{
    //qDebug()<<"closeConnection(reason)"<<*ptr_ConnPtr<<URI;
    keep_alive = false;
    switch (reason) {
        case VIR_CONNECT_CLOSE_REASON_ERROR:
            emit connMsg("Connection closed: Misc I/O error");
            break;
        case VIR_CONNECT_CLOSE_REASON_EOF:
            emit connMsg("Connection closed: End-of-file from server");
            break;
        case VIR_CONNECT_CLOSE_REASON_KEEPALIVE:
            emit connMsg("Connection closed: Keepalive timer triggered");
            break;
        case VIR_CONNECT_CLOSE_REASON_CLIENT:
            emit connMsg("Connection closed: Client requested it");
            break;
        default:
            emit connMsg("Connection closed: Unknown reason");
            break;
    };
    // emit connCloseed(bool), because thread is finished
    // and closeConnection() will not be called
    emit connClosed(onView);
}
void ConnAliveThread::getAuthCredentials(const QString &crd)
{
    /*
     * Get credentials for authData;
     * invoke Input Credentials graphic widget &
     * wait a credentials from him
     */
    //qDebug()<<crd<<"keep auth";
    authWaitKey = true;
    emit authRequested(crd);
    int i = 0;
    while ( i<WAIT_AUTH) {
        i++;
        if ( !authWaitKey ) {
            break;
        } else {
            msleep(100);
        };
    };
}
