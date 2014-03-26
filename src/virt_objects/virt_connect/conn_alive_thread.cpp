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

/* private slots */
void ConnAliveThread::run()
{
    openConnect();
    int probe = 0;
    int ret;
    if ( keep_alive && registered ) {
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
    /*
     * TODO: implement virConnectOpenReadOnly(), virConnectOpenAuth()
     */
    conn = virConnectOpen(URI.toUtf8().constData());
    //qDebug()<<"openConn"<<conn;
    sendConnErrors();
    if (conn==NULL) {
        keep_alive = false;
        emit connMsg( "Connection to the Hypervisor is failed." );
        emit changeConnState(FAILED);
    } else {
        keep_alive = true;
        emit connMsg( QString("connect opened: %1").arg(QVariant(conn!=NULL).toString()) );
        registered = (virEventRegisterDefaultImpl()==0)?true:false;
        emit connMsg( QString("default event implementation registered: %1").arg(QVariant(registered).toString()) );
        emit changeConnState(RUNNING);
        registerConnEvents();
    };
}
void ConnAliveThread::closeConnect()
{
    keep_alive = false;
    deregisterConnEvents();
    if ( conn!=NULL ) {
        int ret = virConnectClose(conn);
        if ( ret<0 ) {
            sendConnErrors();
        } else {
            connMsg( QString("close exit code: %1").arg(ret) );
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
        emit connMsg( QString("VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void ConnAliveThread::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL )
        emit connMsg( QString("VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message) );
    virResetLastError();
}
void ConnAliveThread::registerConnEvents()
{
    virConnectRegisterCloseCallback(conn, connectCloseCallback, NULL, NULL);
}
void ConnAliveThread::deregisterConnEvents()
{
    if ( NULL!=conn ) virConnectUnregisterCloseCallback(conn, connectCloseCallback);
}
