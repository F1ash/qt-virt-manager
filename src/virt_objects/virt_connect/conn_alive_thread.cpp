#include "conn_alive_thread.h"

/*
 * TODO: Implement virConnectRegisterCloseCallback()
 * & some event callbacks if necessary.
 */

ConnAliveThread::ConnAliveThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<CONN_STATE>("CONN_STATE");
}
ConnAliveThread::~ConnAliveThread()
{
    conn = NULL;
    virtErrors = NULL;
    conn_keep_alive.remove(connName);
}

/* public slots */
bool ConnAliveThread::setData(QString &name, QString &uri)
{
    connName = name;
    URI = uri;
    bool res = !conn_keep_alive.contains(connName);
    if (res) {
        conn_keep_alive.insert(connName, new Keep_Alive);
    };
    return res;
}
void ConnAliveThread::setKeepAlive(bool b)
{
    conn_keep_alive.value(connName)->state = b;
    if ( isRunning() && !conn_keep_alive.value(connName)->state ) {
        closeConnect();
        terminate();
    };
}
bool ConnAliveThread::getKeepAlive() const
{
    return conn_keep_alive.value(connName)->state;
}
virConnect* ConnAliveThread::getConnect() const
{
    return conn;
}

/* private slots */
void ConnAliveThread::run()
{
    openConnect();
    int probe = 0;
    int ret;
    if ( conn_keep_alive.value(connName)->state && registered ) {
        /* Use if virEventRegisterDefaultImpl() is registered */
        ret = virConnectSetKeepAlive(conn, 60, 5);
        if ( ret<0 ) {
            sendConnErrors();
            closeConnect();
        } else if ( ret ) {
            emit connMsg( "Remote party doesn't support keepalive messages." );
        } else {
            emit connMsg( "Set keepalive messages." );
        };
        while ( conn_keep_alive.value(connName)->state ) {
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
        while ( conn_keep_alive.value(connName)->state ) {
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
    if ( conn_keep_alive.value(connName)->state ) closeConnect();
}
void ConnAliveThread::openConnect()
{
    conn_keep_alive.value(connName)->msg.clear();
    if ( virInitialize()+1 ) {
        registered = (virEventRegisterDefaultImpl()==0)?true:false;
        emit connMsg( QString("default event implementation registered: %1").arg(QVariant(registered).toString()) );
        /*
         * TODO: implement virConnectOpenReadOnly(), virConnectOpenAuth()
         */
        conn = virConnectOpen(URI.toUtf8().constData());
    };
    //qDebug()<<"openConn"<<conn;
    if (conn==NULL) {
        sendConnErrors();
        conn_keep_alive.value(connName)->state = false;
        emit connMsg( "Connection to the Hypervisor is failed." );
        emit changeConnState(FAILED);
    } else {
        conn_keep_alive.value(connName)->state = true;
        emit connMsg( QString("connect opened: %1").arg(QVariant(conn!=NULL).toString()) );
        emit changeConnState(RUNNING);
        registerConnEvents();
    };
}
void ConnAliveThread::closeConnect()
{
    conn_keep_alive.value(connName)->state = false;
    if ( !conn_keep_alive.value(connName)->msg.isEmpty() ) {
        emit connMsg(conn_keep_alive.value(connName)->msg);
        conn_keep_alive.value(connName)->msg.clear();
    };
    unregisterConnEvents();
    if ( conn!=NULL ) {
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
                                              NULL,
                                              freeData);
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
    switch (reason) {
    case VIR_CONNECT_CLOSE_REASON_ERROR:
        conn_keep_alive.value(connName)->msg.append("Misc I/O error");
        break;
    case VIR_CONNECT_CLOSE_REASON_EOF:
        conn_keep_alive.value(connName)->msg.append("End-of-file from server");
        break;
    case VIR_CONNECT_CLOSE_REASON_KEEPALIVE:
        conn_keep_alive.value(connName)->msg.append("Keepalive timer triggered");
        break;
    case VIR_CONNECT_CLOSE_REASON_CLIENT:
        conn_keep_alive.value(connName)->msg.append("Client requested it");
        break;
    default:
        break;
    };
    conn_keep_alive.value(connName)->state = false;
    freeData(opaque);
    virConnectClose(_conn);
    _conn = NULL;
    emit connMsg("78686868");
}
