#include "conn_alive_thread.h"

/*
 * TODO: Implement some event callbacks if necessary.
 */

ConnAliveThread::ConnAliveThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<CONN_STATE>("CONN_STATE");
    onView = false;
}
ConnAliveThread::~ConnAliveThread()
{
    conn = NULL;
    virtErrors = NULL;
}

/* public slots */
void ConnAliveThread::setData(QString &uri) { URI = uri; }
void ConnAliveThread::setKeepAlive(bool b)
{
    keep_alive = b;
    if ( isRunning() && !keep_alive ) {
        closeConnect();
        terminate();
        wait(60000);
    };
}
bool ConnAliveThread::getKeepAlive() const
{
    return keep_alive;
}
virConnect* ConnAliveThread::getConnect() const
{
    return conn;
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
    openConnect();
    int probe = 0;
    int ret;
    if ( keep_alive && registered ) {
        /* Use if virEventRegisterDefaultImpl() is registered */
        ret = virConnectSetKeepAlive(conn, 10, 3);
        if ( ret<0 ) {
            sendConnErrors();
            closeConnect();
        } else if ( ret ) {
            emit connMsg( "Remote party doesn't support keepalive messages." );
        } else {
            emit connMsg( "Set keepalive messages." );
        };
        while ( keep_alive ) {
            if ( virEventRunDefaultImpl() < 0 ) {
                sendConnErrors();
                if ( ++probe>2 ) break;
            };
        };
    } else {
        /* virConnectIsAlive() --
         * Determine if the connection to the hypervisor is still alive
         * A connection will be classed as alive if it is either local,
         * or running over a channel (TCP or UNIX socket) which is not closed.
         */
        emit connMsg( "Check the connection is alive." );
        while ( keep_alive ) {
            msleep(500);
            ret = virConnectIsAlive(conn);
            if ( ret<0 ) {
                sendConnErrors();
                if ( ++probe>2 ) break;
            } else if ( ret==0 ) {
                emit connMsg( "Connection is dead." );
                break;
            };
        };
    };
    if ( keep_alive ) closeConnect();
}
void ConnAliveThread::openConnect()
{
    if ( virInitialize()+1 ) {
        registered = (virEventRegisterDefaultImpl()==0)?true:false;
        emit connMsg( QString("default event implementation registered: %1")
                      .arg(QVariant(registered).toString()) );
        //conn = virConnectOpen(URI.toUtf8().constData());
        auth.cb = authCallback;
        auth.cbdata = this;
        conn = virConnectOpenAuth(URI.toUtf8().constData(), &auth, 0);
    };
    //qDebug()<<"openConn"<<conn;
    if (conn==NULL) {
        sendConnErrors();
        keep_alive = false;
        emit connMsg( "Connection to the Hypervisor is failed." );
        emit changeConnState(FAILED);
    } else {
        keep_alive = true;
        emit connMsg( QString("connect opened: %1")
                      .arg(QVariant(conn!=NULL)
                           .toString()) );
        emit changeConnState(RUNNING);
        registerConnEvents();
    };
}
void ConnAliveThread::closeConnect()
{
    keep_alive = false;
    if ( conn!=NULL ) {
        unregisterConnEvents();
        int ret = virConnectClose(conn);
        if ( ret<0 ) {
            sendConnErrors();
        } else {
            emit connMsg( QString("close exit code: %1").arg(ret) );
        };
        conn = NULL;
        emit changeConnState(STOPPED);
    } else {
        emit connMsg( QString("connect is NULL") );
        emit changeConnState(FAILED);
    };
}
void ConnAliveThread::sendConnErrors()
{
    virtErrors = virConnGetLastError(conn);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        emit connMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                      .arg(QString().fromUtf8(virtErrors->message)) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void ConnAliveThread::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL && virtErrors->code>0 )
        emit connMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                      .arg(QString().fromUtf8(virtErrors->message)) );
    virResetLastError();
}
void ConnAliveThread::registerConnEvents()
{
    int ret = virConnectRegisterCloseCallback(conn,
                                              connEventCallBack,
                                              this,
    // don't register freeData, because it remove this thread
                                              NULL);
    if (ret<0) sendConnErrors();
    ret = virConnectDomainEventRegister(conn,
                                        domEventCallback,
                                        this,
    // don't register freeData, because it remove this thread
                                        NULL);
    if (ret<0) sendConnErrors();
}
void ConnAliveThread::unregisterConnEvents()
{
    int ret = virConnectUnregisterCloseCallback(conn, connEventCallBack);
    if (ret<0) sendConnErrors();
    ret = virConnectDomainEventDeregister(conn, domEventCallback);
    if (ret<0) sendConnErrors();
}
void ConnAliveThread::freeData(void *opaque)
{
    if ( opaque!=NULL ) {
        void *data = opaque;
        free(data);
    }
}
void ConnAliveThread::connEventCallBack(virConnectPtr _conn, int reason, void *opaque)
{
    Q_UNUSED(_conn);
    static_cast<ConnAliveThread*>(opaque)->closeConnect(reason);
}
int  ConnAliveThread::authCallback(virConnectCredentialPtr cred, unsigned int ncred, void *cbdata)
{
    qDebug()<<ncred<<"keep auth";
    size_t i;
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(cbdata);

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
    obj->authData.username = NULL;
    obj->authData.password = NULL;
    QString crd;
    for (i = 0; i < ncred; ++i) {
        switch (cred[i].type) {
        case VIR_CRED_AUTHNAME:
            crd = "Username";
            obj->getAuthCredentials(crd);
            cred[i].result = strdup(obj->authData.username);
            if (cred[i].result == NULL) {
                return -1;
            };
            cred[i].resultlen = strlen(cred[i].result);
            // clear/shred authData credential for more security
            if ( obj->authData.username!=NULL )
                memset(&obj->authData.username[0], 0, sizeof(obj->authData.username));
            break;
        case VIR_CRED_PASSPHRASE:
            crd = "Password";
            obj->getAuthCredentials(crd);
            cred[i].result = strdup(obj->authData.password);
            if (cred[i].result == NULL) {
                return -1;
            };
            cred[i].resultlen = strlen(cred[i].result);
            // clear/shred authData credential for more security
            if ( obj->authData.password!=NULL )
                memset(&obj->authData.password[0], 0, sizeof(obj->authData.password));
            break;
        default:
            qDebug()<<cred[i].type<<"unused credential type";
            return -1;
        }
    };
    return 0;
}
int  ConnAliveThread::domEventCallback(virConnectPtr _conn, virDomainPtr dom, int event, int detail, void *opaque)
{
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(opaque);
    QString msg;
    msg = QString("<font color='blue'><b>EVENT</b></font>: <b>'%1'</b> Domain %2 %3\n")
           .arg(virDomainGetName(dom))
           .arg(obj->eventToString(event))
           .arg(obj->eventDetailToString(event, detail));
    emit obj->connMsg(msg);
    if ( obj->onView ) {
        Result result;
        QStringList domainList;
        if ( _conn!=NULL && obj->keep_alive ) {
            virDomainPtr *domains;
            unsigned int flags = VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                                 VIR_CONNECT_LIST_DOMAINS_INACTIVE;
            int ret = virConnectListAllDomains(_conn, &domains, flags);
            if ( ret<0 ) {
                obj->sendConnErrors();
                return 0;
            };
            int i = 0;
            while ( domains[i] != NULL ) {
                QStringList currentAttr;
                QString autostartStr;
                int is_autostart = 0;
                if (virDomainGetAutostart(domains[i], &is_autostart) < 0) {
                    autostartStr.append("no autostart");
                } else autostartStr.append( is_autostart ? "yes" : "no" );
                int state;
                int reason;
                // flags : extra flags; not used yet, so callers should always pass 0
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
                        break;
                    }
                } else domainState.append("ERROR");
                currentAttr<< QString().fromUtf8( virDomainGetName(domains[i]) )
                           << QString("%1:%2")
                              .arg( virDomainIsActive(domains[i]) ? "active" : "inactive" )
                              .arg(domainState)
                           << autostartStr
                           << QString( virDomainIsPersistent(domains[i]) ? "yes" : "no" );
                domainList.append(currentAttr.join(DFR));
                virDomainFree(domains[i]);
                i++;
            };
            free(domains);
        };
        //result.name   = ;
        result.type   = "domain";
        //result.number = number;
        result.action = GET_ALL_ENTITY;
        result.result = true;
        result.msg = domainList;
        emit obj->domStateChanged(result);
    };
    return 0;
}
const char* ConnAliveThread::eventToString(int event) {
    const char *ret = "";
    switch ((virDomainEventType) event) {
        case VIR_DOMAIN_EVENT_DEFINED:
            ret ="Defined";
            break;
        case VIR_DOMAIN_EVENT_UNDEFINED:
            ret ="Undefined";
            break;
        case VIR_DOMAIN_EVENT_STARTED:
            ret ="Started";
            break;
        case VIR_DOMAIN_EVENT_SUSPENDED:
            ret ="Suspended";
            break;
        case VIR_DOMAIN_EVENT_RESUMED:
            ret ="Resumed";
            break;
        case VIR_DOMAIN_EVENT_STOPPED:
            ret ="Stopped";
            break;
        case VIR_DOMAIN_EVENT_SHUTDOWN:
            ret = "Shutdown";
            break;
    };
    return ret;
}
const char* ConnAliveThread::eventDetailToString(int event, int detail) {
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
            }
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
            }
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
            }
            break;
        case VIR_DOMAIN_EVENT_STOPPED:
            switch ((virDomainEventStoppedDetailType) detail) {
            case VIR_DOMAIN_EVENT_STOPPED_SHUTDOWN:
                ret = "Shutdown";
                break;
            case VIR_DOMAIN_EVENT_STOPPED_DESTROYED:
                ret = "Destroyed";
                break;
            case VIR_DOMAIN_EVENT_STOPPED_CRASHED:
                ret = "Crashed";
                break;
            case VIR_DOMAIN_EVENT_STOPPED_MIGRATED:
                ret = "Migrated";
                break;
            case VIR_DOMAIN_EVENT_STOPPED_SAVED:
                ret = "Saved";
                break;
            case VIR_DOMAIN_EVENT_STOPPED_FAILED:
                ret = "Failed";
                break;
            case VIR_DOMAIN_EVENT_STOPPED_FROM_SNAPSHOT:
                ret = "Snapshot";
                break;
            }
            break;
        case VIR_DOMAIN_EVENT_SHUTDOWN:
            switch ((virDomainEventShutdownDetailType) detail) {
            case VIR_DOMAIN_EVENT_SHUTDOWN_FINISHED:
                ret = "Finished";
                break;
            }
            break;
    };
    return ret;
}
void ConnAliveThread::closeConnect(int reason)
{
    CONN_STATE state;
    keep_alive = false;
    switch (reason) {
    case VIR_CONNECT_CLOSE_REASON_ERROR:
        emit connMsg("Connect closed: Misc I/O error");
        state = FAILED;
        break;
    case VIR_CONNECT_CLOSE_REASON_EOF:
        emit connMsg("Connect closed: End-of-file from server");
        state = FAILED;
        break;
    case VIR_CONNECT_CLOSE_REASON_KEEPALIVE:
        emit connMsg("Connect closed: Keepalive timer triggered");
        state = STOPPED;
        break;
    case VIR_CONNECT_CLOSE_REASON_CLIENT:
        emit connMsg("Connect closed: Client requested it");
        state = STOPPED;
        break;
    default:
        emit connMsg("Connect closed: Unknown reason");
        state = FAILED;
        break;
    };
    // don't unregisterConnEvents, because disconnected already
    sendConnErrors();
    if ( virConnectClose(conn)<0 ) sendConnErrors();
    conn = NULL;
    emit changeConnState(state);
    keep_alive = false;
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
