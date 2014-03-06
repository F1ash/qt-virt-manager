#include "conn_alive_thread.h"
/*
 * TODO: implement an event loop with virEventRegisterImpl()
 * or virEventRegisterDefaultImpl() to be able to use keepalive messages.
 * Implement virConnectRegisterCloseCallback() if necessary.
 */

ConnAliveThread::ConnAliveThread(QObject *parent) :
    QThread(parent)
{
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
    // dirty hack for close connect while unregister events don't implemented here
    if ( isRunning() && registered && !keep_alive ) {
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
    // check for connect's validity
    /*
     * TODO: Maybe this is nessesary to extend for other type connects
     */
    if ( registered ) {
        /* Use if virEventRegisterDefaultImpl() is registered */
        ret = virConnectSetKeepAlive(conn, 60, 5);
        if ( ret<0 ) {
            sendConnErrors();
        } else if ( ret ) {
            emit connMsg( "Remote party doesn't support keepalive messages." );
        } else {
            emit connMsg( "Set keepalive messages." );
        };
        while ( keep_alive ) {
            if ( virEventRunDefaultImpl() < 0 )
                keep_alive = false;
        };
    } else {
        /* virConnectIsAlive() --
         * Determine if the connection to the hypervisor is still alive
         * A connection will be classed as alive if it is either local,
         * or running over a channel (TCP or UNIX socket) which is not closed.
         */
        while ( keep_alive ) {
            ret = virConnectIsAlive(conn);
            if ( ret<0 ) {
                if ( ++probe>2 ) keep_alive = false;
                sendConnErrors();
            } else keep_alive = (ret)?true:false;
            msleep(500);
        };
    };
    // dirty hack for close connect while unregister events don't implemented here
    if ( !registered ) closeConnect();
}
void ConnAliveThread::openConnect()
{
    /*
     * TODO: implement virConnectOpenReadOnly(), virConnectOpenAuth()
     */
    conn = virConnectOpen(URI.toUtf8().constData());
    //qDebug()<<"openConn"<<conn;
    sendGlobalErrors();
    if (conn==NULL) {
        keep_alive = false;
        emit connMsg( "Connection to the Hypervisor is failed." );
    } else {
        keep_alive = true;
        emit connMsg( QString("connect opened: %1").arg(QVariant(conn!=NULL).toString()) );
        registered = (virEventRegisterDefaultImpl()==0)?true:false;
        emit connMsg( QString("default event implementation registered: %1").arg(QVariant(registered).toString()) );
        emit connected();
    };
}
void ConnAliveThread::closeConnect()
{
    keep_alive = false;
    if ( conn!=NULL ) {
        int ret = virConnectClose(conn);
        sendGlobalErrors();
        if ( ret<0 ) {
            sendConnErrors();
        } else {
            connMsg( QString("close exit code: %1").arg(ret) );
        };
        conn = NULL;
    } else {
        emit connMsg( QString("connect is NULL") );
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

