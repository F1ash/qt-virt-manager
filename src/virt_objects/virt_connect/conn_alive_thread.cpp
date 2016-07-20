#include "conn_alive_thread.h"
#define MINUTE 60000

ConnAliveThread::ConnAliveThread(QObject *parent) :
    _VirtThread(parent)
{
    qRegisterMetaType<CONN_STATE>("CONN_STATE");
    _connPtr = nullptr;
    onView = false;
    // virConnectRegisterCloseCallback && virConnectDomainEventRegisterAny
    // -1 on error; another is success or a callback identifier
    closeCallbackRegistered  = false;
    domainsLifeCycleCallback = -1;
    networkLifeCycleCallback = -1;
}
ConnAliveThread::~ConnAliveThread()
{
    keep_alive = false;
}

/* public slots */
void ConnAliveThread::setData(QString &uri) { URI = uri; }
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
virConnectPtr *ConnAliveThread::getPtr_connectionPtr()
{
    return ptr_ConnPtr;
}
void ConnAliveThread::setAuthCredentials(QString &crd, QString &text)
{
    if ( crd.toLower()=="username" )
        authData.username = text.toUtf8().data();
    if ( crd.toLower()=="password" )
        authData.password = text.toUtf8().data();
    authWaitKey = false;
}

/* private slots */
void ConnAliveThread::run()
{
    openConnection();
    while ( keep_alive && nullptr!=*ptr_ConnPtr ) {
        msleep(1000);
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
    if (ret<0) sendConnErrors();
    networkLifeCycleCallback = virConnectNetworkEventRegisterAny(
                *ptr_ConnPtr,
                nullptr,
    // set networksLifeCycleCallback signature
                VIR_NETWORK_EVENT_ID_LIFECYCLE,
                VIR_NETWORK_EVENT_CALLBACK(netEventCallback),
                this,
    // don't register freeData, because it remove this thread
                nullptr);
    if (ret<0) sendConnErrors();
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
    if ( domainsLifeCycleCallback ) {
        int ret = virConnectDomainEventDeregisterAny(
                    *ptr_ConnPtr, domainsLifeCycleCallback);
        if (ret<0) sendConnErrors();
        domainsLifeCycleCallback = -1;
    };
    if ( networkLifeCycleCallback ) {
        int ret = virConnectNetworkEventDeregisterAny(
                    *ptr_ConnPtr, networkLifeCycleCallback);
        if (ret<0) sendConnErrors();
        networkLifeCycleCallback = -1;
    };
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
    //qDebug()<<"domEventCallback"<<_conn;
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(opaque);
    if ( nullptr==obj || *(obj->ptr_ConnPtr)!=_conn ) return 0;
    bool end = false;
    QString msg, domainName;
    domainName = QString(virDomainGetName(dom));
    msg = QString("<b>'%1'</b> Domain %2 %3\n")
           .arg(domainName)
           .arg(obj->domEventToString(event))
           .arg(obj->domEventDetailToString(event, detail, &end));
    if ( end ) emit obj->domainEnd(domainName);
    emit obj->connMsg(msg);
    if ( obj->onView ) {
        Result result;
        QStringList domainList;
        if ( _conn!=nullptr && obj->keep_alive ) {
            virDomainPtr *domains = nullptr;
            unsigned int flags =
                    VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                    VIR_CONNECT_LIST_DOMAINS_INACTIVE;
            // the number of domains found or -1 and sets domains
            // to NULL in case of error.
            int ret = virConnectListAllDomains(
                        _conn, &domains, flags);
            if ( ret<0 ) {
                obj->sendConnErrors();
                return 0;
            };
            // therefore correctly to use for() command,
            // because domains[0] can not exist.
            for (int i = 0; i < ret; i++) {
                QStringList currentAttr;
                QString autostartStr;
                int is_autostart = 0;
                if (virDomainGetAutostart(domains[i], &is_autostart) < 0) {
                    autostartStr.append("no autostart");
                } else autostartStr.append( is_autostart ? "yes" : "no" );
                int state;
                int reason;
                // flags : extra flags; not used yet,
                // so callers should always pass 0
                flags = 0;
                QString domainState;
                if ( virDomainGetState(domains[i], &state, &reason, flags)+1 ) {
                    switch (state) {
                    case VIR_DOMAIN_NOSTATE:
                        domainState.append("NOSTATE");
                        break;
                    case VIR_DOMAIN_RUNNING:
                        domainState.append("RUNNING");
                        break;
                    case VIR_DOMAIN_BLOCKED:
                        domainState.append("BLOCKED");
                        break;
                    case VIR_DOMAIN_PAUSED:
                        domainState.append("PAUSED");
                        break;
                    case VIR_DOMAIN_SHUTDOWN:
                        domainState.append("SHUTDOWN");
                        break;
                    case VIR_DOMAIN_SHUTOFF:
                        domainState.append("SHUTOFF");
                        break;
                    case VIR_DOMAIN_CRASHED:
                        domainState.append("CRASHED");
                        break;
                    case VIR_DOMAIN_PMSUSPENDED:
                        domainState.append("PMSUSPENDED");
                        break;
                    default:
                        domainState.append("UNKNOWN");
                        break;
                    }
                } else domainState.append("ERROR");
                currentAttr<< QString().fromUtf8( virDomainGetName(domains[i]) )
                           << QString("%1:%2")
                              .arg( virDomainIsActive(
                                    domains[i]) ? "active" : "inactive" )
                              .arg(domainState)
                           << autostartStr
                           << QString( virDomainIsPersistent(
                                           domains[i]) ? "yes" : "no" );
                domainList.append(currentAttr.join(DFR));
                virDomainFree(domains[i]);
            };
            free(domains);
        };
        //result.name   = ;
        result.type   = "domain";
        //result.number = number;
        result.action = GET_ALL_ENTITY_STATE;
        result.result = true;
        result.msg = domainList;
        emit obj->domStateChanged(result);
    };
    return 0;
}
int  ConnAliveThread::netEventCallback(virConnectPtr _conn, virNetworkPtr net, int event, int detail, void *opaque)
{
    //qDebug()<<"netEventCallback"<<_conn;
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(opaque);
    if ( nullptr==obj || *(obj->ptr_ConnPtr)!=_conn ) return 0;
    QString msg;
    msg = QString("<b>'%1'</b> Network %2 %3\n")
           .arg(virNetworkGetName(net))
           .arg(obj->netEventToString(event))
           .arg(obj->netEventDetailToString(event, detail));
    emit obj->connMsg(msg);
    if ( obj->onView ) {
        Result result;
        QStringList virtNetList;
        if ( _conn!=nullptr && obj->keep_alive ) {
            virNetworkPtr *networks = nullptr;
            unsigned int flags =
                    VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                    VIR_CONNECT_LIST_NETWORKS_INACTIVE;
            // the number of networks found or -1 and sets networks
            // to NULL in case of error.
            int ret = virConnectListAllNetworks(
                        _conn, &networks, flags);
            if ( ret<0 ) {
                obj->sendConnErrors();
                return 0;
            };
            // therefore correctly to use for() command,
            // because networks[0] can not exist.
            for (int i = 0; i < ret; i++) {
                QStringList currentAttr;
                QString autostartStr;
                int is_autostart = 0;
                if (virNetworkGetAutostart(networks[i], &is_autostart) < 0) {
                    autostartStr.append("no autostart");
                } else autostartStr.append( is_autostart ? "yes" : "no" );
                currentAttr<< QString().fromUtf8(
                                  virNetworkGetName(networks[i]) )
                           << QString( virNetworkIsActive(
                                           networks[i]) ? "active" : "inactive" )
                           << autostartStr
                           << QString( virNetworkIsPersistent(
                                           networks[i]) ? "yes" : "no" );
                virtNetList.append(currentAttr.join(DFR));
                //qDebug()<<currentAttr;
                virNetworkFree(networks[i]);
            };
            free(networks);
        };
        //result.name   = ;
        result.type   = "network";
        //result.number = number;
        result.action = GET_ALL_ENTITY_STATE;
        result.result = true;
        result.msg = virtNetList;
        emit obj->netStateChanged(result);
    };
    return 0;
}
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
            if (detail == VIR_DOMAIN_EVENT_DEFINED_ADDED)
                ret = "Added";
            else if (detail == VIR_DOMAIN_EVENT_DEFINED_UPDATED)
                ret = "Updated";
            break;
        case VIR_DOMAIN_EVENT_UNDEFINED:
            if (detail == VIR_DOMAIN_EVENT_UNDEFINED_REMOVED)
                ret = "Removed";
            break;
        case VIR_DOMAIN_EVENT_STARTED:
            switch ((virDomainEventStartedDetailType) detail) {
                case VIR_DOMAIN_EVENT_STARTED_BOOTED:
                    ret = "Booted";
                    break;
                case VIR_DOMAIN_EVENT_STARTED_MIGRATED:
                    ret = "Migrated";
                    break;
                case VIR_DOMAIN_EVENT_STARTED_RESTORED:
                    ret = "Restored";
                    break;
                case VIR_DOMAIN_EVENT_STARTED_FROM_SNAPSHOT:
                    ret = "Snapshot";
                    break;
                case VIR_DOMAIN_EVENT_STARTED_WAKEUP:
                    ret = "Event wakeup";
                    break;
                default:
                    ret = "Unknown";
                    break;
            };
            break;
        case VIR_DOMAIN_EVENT_SUSPENDED:
            switch ((virDomainEventSuspendedDetailType) detail) {
                case VIR_DOMAIN_EVENT_SUSPENDED_PAUSED:
                    ret = "Paused";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_MIGRATED:
                    ret = "Migrated";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_IOERROR:
                    ret = "I/O Error";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_WATCHDOG:
                    ret = "Watchdog";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_RESTORED:
                    ret = "Restored";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_FROM_SNAPSHOT:
                    ret = "Snapshot";
                    break;
                case VIR_DOMAIN_EVENT_SUSPENDED_API_ERROR:
                    ret = "API error";
                    break;
                default:
                    ret = "Unknown";
                    break;
            };
            break;
        case VIR_DOMAIN_EVENT_RESUMED:
            switch ((virDomainEventResumedDetailType) detail) {
                case VIR_DOMAIN_EVENT_RESUMED_UNPAUSED:
                    ret = "Unpaused";
                    break;
                case VIR_DOMAIN_EVENT_RESUMED_MIGRATED:
                    ret = "Migrated";
                    break;
                case VIR_DOMAIN_EVENT_RESUMED_FROM_SNAPSHOT:
                    ret = "Snapshot";
                    break;
                default:
                    ret = "Unknown";
                    break;
            };
            break;
        case VIR_DOMAIN_EVENT_STOPPED:
            switch ((virDomainEventStoppedDetailType) detail) {
                case VIR_DOMAIN_EVENT_STOPPED_SHUTDOWN:
                    ret = "Shutdown";
                    *end = true;
                    break;
                case VIR_DOMAIN_EVENT_STOPPED_DESTROYED:
                    ret = "Destroyed";
                    *end = true;
                    break;
                case VIR_DOMAIN_EVENT_STOPPED_CRASHED:
                    ret = "Crashed";
                    *end = true;
                    break;
                case VIR_DOMAIN_EVENT_STOPPED_MIGRATED:
                    ret = "Migrated";
                    break;
                case VIR_DOMAIN_EVENT_STOPPED_SAVED:
                    ret = "Saved";
                    break;
                case VIR_DOMAIN_EVENT_STOPPED_FAILED:
                    ret = "Failed";
                    *end = true;
                    break;
                case VIR_DOMAIN_EVENT_STOPPED_FROM_SNAPSHOT:
                    ret = "Snapshot";
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
                ret = "Memory";
                break;
            case VIR_DOMAIN_EVENT_PMSUSPENDED_DISK:
                ret = "Disk";
                break;
            };
            break;
        case VIR_DOMAIN_EVENT_CRASHED:
           switch ((virDomainEventCrashedDetailType) detail) {
           case VIR_DOMAIN_EVENT_CRASHED_PANICKED:
               ret = "Panicked";
               *end = true;
               break;
           };
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
void ConnAliveThread::getAuthCredentials(QString &crd)
{
    /*
     * Get credentials for authData;
     * invoke Input Credentials graphic widget &
     * wait a credentials from him
     */
    qDebug()<<crd<<"keep auth";
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
