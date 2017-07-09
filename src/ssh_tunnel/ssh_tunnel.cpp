//extern "C" {
//#include "ssh_common.c"
//}
#include "ssh_tunnel.h"

#define bufSize  4096

SSH_Tunnel::SSH_Tunnel(QObject *parent) :
    QThread(parent)
{
    socketToViewerPort = new QTcpSocket(this);
    ssh_tunnel = new QProcess(this);
}

void SSH_Tunnel::setData(QVariantMap _data)
{
    User = _data.value("User").toString();
    remoteHost = _data.value("RemoteHost").toString();
    remotePort = _data.value("RemotePort").toString();
    graphicsAddr = _data.value("GraphicsAddr").toString();
    graphicsPort = _data.value("GraphicsPort").toString();
}
void SSH_Tunnel::run()
{
    /*
    ssh_session session;
    ssh_channel channel;
    char buffer[256];
    int nbytes;
    int rc;

    session = connect_ssh("stationar", "root", 0);
    if (session == nullptr) {
        return;
    };

    channel = ssh_channel_new(session);
    if (channel == nullptr) {
        ssh_disconnect(session);
        ssh_free(session);
        return;
    };

    rc = ssh_channel_open_session(channel);
    if (rc < 0) {
        goto failed;
    };

    rc = ssh_channel_request_exec(channel, "lsof");
    if (rc < 0) {
        goto failed;
    };

    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    while (nbytes > 0) {
        if (fwrite(buffer, 1, nbytes, stdout) != (unsigned int) nbytes) {
            goto failed;
        };
        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    };

    if (nbytes < 0) {
        goto failed;
    };

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_free(session);

    return;
failed:
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_disconnect(session);
    ssh_free(session);

    return;
    */
    uint viewerPort = 0;
    for (viewerPort=33333; viewerPort<65536; viewerPort++) {
        socketToViewerPort->connectToHost(
                    "127.0.0.1",
                    viewerPort,
                    QIODevice::ReadWrite,
                    QAbstractSocket::IPv4Protocol);
        if ( socketToViewerPort->state()!=QAbstractSocket::ConnectedState ) {
            continue;
        };
        if ( !socketToViewerPort->open(QIODevice::ReadWrite) ) {
            continue;
        } else {
            break;
        };
    };
    if ( !socketToViewerPort->isOpen() ) {
        // emit error to Viewer
        emit errMsg("Socket to Viwer port not established");
        return;
    };
    // socket signal connections;
    connect(socketToViewerPort, SIGNAL(readyRead()),
            this, SLOT(write_to_remote_graphic_channel()));
    connect(socketToViewerPort, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(send_socket_errors(QAbstractSocket::SocketError)));

    ssh_tunnel->setProcessChannelMode(QProcess::SeparateChannels);
    connect(ssh_tunnel, SIGNAL(readyRead()),
            this, SLOT(write_to_viewer()));
    ssh_tunnel->start();
    if ( !ssh_tunnel->waitForStarted() ) {
        emit errMsg("SSH tunnel not established");
        return;
    } else {;
        emit established(viewerPort);
        ssh_tunnel->waitForFinished(-1);
    };
    if ( socketToViewerPort->isOpen() ) {
        socketToViewerPort->disconnectFromHost();
    };
}

/* private slots */
void SSH_Tunnel::write_to_viewer()
{
    char *buff;
    while ( 0<ssh_tunnel->read(buff, bufSize) ) {
        socketToViewerPort->write(buff, bufSize);
    };
}
void SSH_Tunnel::write_to_remote_graphic_channel()
{
    char *buff;
    while ( 0<socketToViewerPort->read(buff, bufSize) ) {
        ssh_tunnel->write(buff, bufSize);
    };
}
void SSH_Tunnel::resend_socket_errors(QAbstractSocket::SocketError _err)
{
    Q_UNUSED(_err);
    emit errMsg(socketToViewerPort->errorString());
}
