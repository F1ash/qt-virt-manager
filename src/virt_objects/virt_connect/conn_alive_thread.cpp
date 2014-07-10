#include "conn_alive_thread.h"

/*
 * TODO: Implement some event callbacks if necessary.
 */

ConnAliveThread::ConnAliveThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<CONN_STATE>("CONN_STATE");
    connect(this, SIGNAL(connectClosed(int)), this, SLOT(closeConnect(int)));
}
ConnAliveThread::~ConnAliveThread()
{
    disconnect(this, SIGNAL(connectClosed(int)), this, SLOT(closeConnect(int)));
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
        emit connMsg( QString("default event implementation registered: %1").arg(QVariant(registered).toString()) );
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
        emit connMsg( QString("connect opened: %1").arg(QVariant(conn!=NULL).toString()) );
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
    if ( virtErrors!=NULL ) {
        emit connMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                      .arg(QString().fromUtf8(virtErrors->message)) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void ConnAliveThread::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL )
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
}
void ConnAliveThread::unregisterConnEvents()
{
    int ret = virConnectUnregisterCloseCallback(conn, connEventCallBack);
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
    int ret = virConnectClose(conn);
    if ( ret<0 ) {
        sendConnErrors();
    } else {
        emit connMsg( QString("close exit code: %1").arg(ret) );
    };
    conn = NULL;
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
