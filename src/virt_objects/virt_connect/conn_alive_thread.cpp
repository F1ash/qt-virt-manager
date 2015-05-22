#include "conn_alive_thread.h"
#define MINUTE 60000

/*
 * TODO: Implement some event callbacks if necessary.
 */

ConnAliveThread::ConnAliveThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<CONN_STATE>("CONN_STATE");
    onView = false;
    closeCallbackRegistered = false;
    domainEventRegistered = false;
}
ConnAliveThread::~ConnAliveThread()
{
    conn = NULL;
    virtErrors = NULL;
    wait(30000);
}

/* public slots */
void ConnAliveThread::setData(QString &uri) { URI = uri; }
void ConnAliveThread::closeConnection()
{
    //qDebug()<<"closeConnection"<<conn<<URI;
    if ( keep_alive ) {
        keep_alive = false;
        return;
    };
    CONN_STATE state;
    if ( conn!=NULL ) {
        unregisterConnEvents();
        int ret = virConnectClose(conn);
        //qDebug()<<"virConnectRef -1"<<"ConnAliveThread"<<URI<<(ret+1>0);
        if ( ret<0 ) {
            state = FAILED;
            sendConnErrors();
        } else {
            emit connMsg( QString("close exit code: %1").arg(ret) );
            state = STOPPED;
        };
        conn = NULL;
    } else {
        emit connMsg( QString("connect is NULL") );
        state = FAILED;
    };
    emit changeConnState(state);
}
virConnect* ConnAliveThread::getConnection() const
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
    openConnection();
    if (!keep_alive) return;
    int probe = 0;
    int ret = virConnectSetKeepAlive(conn, 3, 10);
    if ( ret!=0 ) {
        emit connMsg( "Remote party doesn't support keepalive messages or error is occurred." );
        /* virConnectIsAlive() --
         * Determine if the connection to the hypervisor is still alive
         * A connection will be classed as alive if it is either local,
         * or running over a channel (TCP or UNIX socket) which is not closed.
         *
         * WARNING: current policy for close Connection is 3 error per minute;
         * It maybe improved later.
         */
        emit connMsg( "The check of the alive connection in the loop." );
        QTime timeMark;
        timeMark.start();
        while ( keep_alive && NULL!=conn ) {
            ret = virConnectIsAlive(conn);
            if ( ret<0 ) {
                sendConnErrors();
                if ( ++probe>2 ) {
                    if ( timeMark.elapsed()>MINUTE ) {
                        emit connMsg("The connection is not stable and it was automatically closed.");
                        break;
                    } else {
                        probe = 0;
                        timeMark.restart();
                    };
                };
            } else if ( ret==0 ) {
                emit connMsg("Connection is died or this connection type is unsupported.");
                break;
            };
            msleep(500);
        };
    } else {
        emit connMsg( "Set keepalive messages." );
        while ( keep_alive && NULL!=conn ) {
            if ( virEventRunDefaultImpl() < 0 ) {
                sendConnErrors();
                //if ( ++probe>2 ) break;
            };
        };
    };
    closeConnection();
}
void ConnAliveThread::openConnection()
{
    //conn = virConnectOpen(URI.toUtf8().constData());
    auth.cb = authCallback;
    auth.cbdata = this;
    conn = virConnectOpenAuth(URI.toUtf8().constData(), &auth, 0);
    if (conn==NULL) {
        sendConnErrors();
        keep_alive = false;
        emit connMsg( "Connection to the Hypervisor is failed." );
        emit changeConnState(FAILED);
    } else {
        //qDebug()<<" openConnection"<<conn<<URI;
        keep_alive = true;
        emit connMsg( QString("connect opened: %1")
                      .arg(QVariant(conn!=NULL)
                           .toString()) );
        emit changeConnState(RUNNING);
        registerConnEvents();
    };
    //qDebug()<<"virConnectRef +1"<<"ConnAliveThread"<<URI<<(conn!=NULL);
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
    closeCallbackRegistered = !(ret<0);
    if (ret<0) sendConnErrors();
    ret = virConnectDomainEventRegister(conn,
                                        domEventCallback,
                                        this,
    // don't register freeData, because it remove this thread
                                        NULL);
    domainEventRegistered = !(ret<0);
    if (ret<0) sendConnErrors();
}
void ConnAliveThread::unregisterConnEvents()
{
    if ( closeCallbackRegistered ) {
        int ret = virConnectUnregisterCloseCallback(conn, connEventCallBack);
        if (ret<0) sendConnErrors();
    };
    if ( domainEventRegistered ) {
        int ret = virConnectDomainEventDeregister(conn, domEventCallback);
        if (ret<0) sendConnErrors();
    };
    closeCallbackRegistered = false;
    domainEventRegistered = false;
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
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(opaque);
    if ( NULL!=obj && obj->conn==_conn) {
        obj->unregisterConnEvents();
        obj->closeConnection(reason);
    };
}
int  ConnAliveThread::authCallback(virConnectCredentialPtr cred, unsigned int ncred, void *cbdata)
{
    //qDebug()<<ncred<<"keep auth";
    size_t i;
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(cbdata);
    if ( NULL==obj ) return -1;

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
        };
    };
    return 0;
}
int  ConnAliveThread::domEventCallback(virConnectPtr _conn, virDomainPtr dom, int event, int detail, void *opaque)
{
    ConnAliveThread *obj = static_cast<ConnAliveThread*>(opaque);
    if ( NULL==obj || obj->conn!=_conn ) return 0;
    QString msg;
    msg = QString("<b>'%1'</b> Domain %2 %3\n")
           .arg(virDomainGetName(dom))
           .arg(obj->eventToString(event))
           .arg(obj->eventDetailToString(event, detail));
    emit obj->connMsg(msg);
    if ( obj->onView ) {
        Result result;
        QStringList domainList;
        if ( _conn!=NULL && obj->keep_alive ) {
            virDomainPtr *domains = NULL;
            unsigned int flags = VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                                 VIR_CONNECT_LIST_DOMAINS_INACTIVE;
            // the number of domains found or -1 and sets domains to NULL in case of error.
            int ret = virConnectListAllDomains(_conn, &domains, flags);
            if ( ret<0 ) {
                obj->sendConnErrors();
                return 0;
            };
            // therefore correctly to use for() command, because domains[0] can not exist.
            for (int i = 0; i < ret; i++) {
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
                        domainState.append("UNKNOWN");
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
        default:
            ret = "Unknown";
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
                default:
                    ret = "Unknown";
                    break;
            };
            break;
        case VIR_DOMAIN_EVENT_SHUTDOWN:
            switch ((virDomainEventShutdownDetailType) detail) {
                case VIR_DOMAIN_EVENT_SHUTDOWN_FINISHED:
                    ret = "Finished";
                    break;
                default:
                    ret = "Unknown";
                    break;
                };
            break;
        default:
            ret = "Unknown";
            break;
    };
    return ret;
}
void ConnAliveThread::closeConnection(int reason)
{
    //qDebug()<<"closeConnection(int reason)"<<conn<<URI;
    // don't unregisterConnEvents and close connection,
    // because disconnected already
    conn = NULL;
    keep_alive = false;
    CONN_STATE state;
    switch (reason) {
        case VIR_CONNECT_CLOSE_REASON_ERROR:
            emit connMsg("Connection closed: Misc I/O error");
            state = FAILED;
            break;
        case VIR_CONNECT_CLOSE_REASON_EOF:
            emit connMsg("Connection closed: End-of-file from server");
            state = FAILED;
            break;
        case VIR_CONNECT_CLOSE_REASON_KEEPALIVE:
            emit connMsg("Connection closed: Keepalive timer triggered");
            state = STOPPED;
            break;
        case VIR_CONNECT_CLOSE_REASON_CLIENT:
            emit connMsg("Connection closed: Client requested it");
            state = STOPPED;
            break;
        default:
            emit connMsg("Connection closed: Unknown reason");
            state = FAILED;
            break;
    };
    emit changeConnState(state);
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
