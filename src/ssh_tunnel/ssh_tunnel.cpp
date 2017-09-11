//extern "C" {
//#include "ssh_common.c"
//}
#include "ssh_tunnel.h"
#include <QHostAddress>
#include <QTextStream>
//#include <signal.h>

#define buffSize  4096

SSH_Tunnel::SSH_Tunnel(QObject *parent) :
    QThread(parent)
{
    listenSocket = new QTcpSocket(this);
    //listenSocket->setSocketOption(
    //            QAbstractSocket::KeepAliveOption, true);
    serverThread = new TCP_Server_Thread(this);
    connect(serverThread->server, SIGNAL(readyRead(QTcpSocket*)),
            this, SLOT(write_to_remote_graphic_channel(QTcpSocket*)));
    connect(serverThread->server, SIGNAL(established(quint16)),
            this, SIGNAL(established(quint16)));
    ssh_tunnel = new QProcess(this);
    connect(this, SIGNAL(finished()),
            this, SLOT(thread_finished()));
    //loop = new QEventLoop(this);
    //connect(ssh_tunnel, SIGNAL(finished(int)),
    //        loop, SLOT(quit()));
}
SSH_Tunnel::~SSH_Tunnel()
{
    QTextStream s(stdout);
    if ( ssh_tunnel!=nullptr ) {
        if ( ssh_tunnel->isOpen() ) {
            ssh_tunnel->close();
            //ssh_tunnel->waitForFinished();
            s<< "ssh_tunnel is finished at thread deletion" << endl;
        //} else {
        //    ssh_tunnel->kill();
        //    s<< "ssh_tunnel is killed at thread deletion" << endl;
        };
        //kill(pid, SIGKILL);
        ssh_tunnel->kill();
    };
    //if ( serverThread!=nullptr ) {
    //    serverThread->server->close();
    //    serverThread->stop();
    //};
    //listenSocket->disconnectFromHost();
    //listenSocket->waitForDisconnected();
    //listenSocket->close();
    s<< "~SSH_Tunnel()" << endl;
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
    QTextStream s(stdout);
    quint16 viewerPort = 33333;
    bool connected = false;
    bool finished = false;
    QStringList _args;
    QString nc_command;

    /*
    while ( !connected) {

        bool bound = listenSocket->bind(
                    QHostAddress::LocalHost,
                    0,
                    QAbstractSocket::DontShareAddress); // as listen service
        if ( !bound ) {
            viewerPort = 0;
            continue;
        };
        //listenSocket->
        viewerPort = listenSocket->localPort();
        if ( viewerPort>0 ) {
            connected = true;
            break;
        };

        ++viewerPort;
        listenSocket->connectToHost(
                    QHostAddress::LocalHost,
                    viewerPort,
                    QIODevice::ReadWrite);
        if ( !listenSocket->waitForConnected() ) {
            viewerPort = 0;
            continue;
        };
        if ( !listenSocket->open(QIODevice::ReadWrite) ) {
            viewerPort = 0;
            continue;
        } else {
            connected = true;
        };
    };

    if ( !connected ) {
        // emit error to Viewer
        emit errMsg("Socket to Viwer port not established");
        s<< "not connected" << endl;
        goto thread_exit;
    };
    */
    // socket signal connections;
    //connect(listenSocket, SIGNAL(readyRead()),
    //        this, SLOT(write_to_remote_graphic_channel()));
    //connect(listenSocket, SIGNAL(error(QAbstractSocket::SocketError)),
    //        this, SLOT(send_socket_errors(QAbstractSocket::SocketError)));

    //while ( !serverThread->server->start_listen(viewerPort) ) {
    //    ++viewerPort;
    //    if (viewerPort>65536) break;
    //};


    // create SSH tunnel
    ssh_tunnel->setProcessChannelMode(QProcess::SeparateChannels);
    //connect(ssh_tunnel, SIGNAL(readyRead()),
    //        this, SLOT(write_to_viewer()));
    //connect(ssh_tunnel, SIGNAL(finished(int)),
    //        this, SIGNAL(tunnel_finished()));
    //connect(ssh_tunnel, SIGNAL(destroyed(QObject*)),
    //        this, SIGNAL(tunnel_finished()));
    connect(ssh_tunnel, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(resend_tunnel_errors(QProcess::ProcessError)));
    /*
    nc_command = QString(
"'nc -q 2>&1 | grep \"requires an argument\" >/dev/null;\
if [ $? -eq 0 ] ; then\
    CMD=\"nc -q 0 %1 %2\";\
else\
    CMD=\"nc %1 %2\";\
fi;\
eval \"$CMD\";'").arg(graphicsAddr).arg(graphicsPort);
    _args   << "-p" << remotePort
            << "-l" << User << remoteHost
            << "sh" << "-c"
            << nc_command;
*/

    ///*
    while ( !connected ) {

        if ( !listenSocket->bind(0) ) {
            continue;
        };
        viewerPort = listenSocket->localPort();
        _args.clear();
        nc_command.clear();
        _args   << "-p" << remotePort
                << "-L"
                << QString("%1:%3:%2").arg(viewerPort).arg(graphicsPort).arg(graphicsAddr)
                << QString("%1@%2").arg(User).arg(remoteHost);
        ssh_tunnel->start("ssh", _args, QIODevice::ReadWrite);
        s << "command args: "<< _args.join(" ") << endl;
        if ( !ssh_tunnel->waitForStarted() ) continue;
        if ( !ssh_tunnel->isOpen() ) {
            ssh_tunnel->kill();
            continue;
        };
        listenSocket->close();
        listenSocket->deleteLater();
        pid = ssh_tunnel->pid();
        connected = true;
        break;
    };
    //*/


    //s << "command args: "<< _args.join(" ") << endl;
    //ssh_tunnel->start("ssh", _args, QIODevice::ReadWrite);
    //if ( !ssh_tunnel->waitForStarted() ) {
    //    s<< "remote redirect established: "<< "none" << endl;
    //    emit errMsg("SSH tunnel not established");
    //    goto thread_exit;
    //};

    emit established(viewerPort);
    //serverThread->start();
    s<< "established: "<< viewerPort <<" State: "<<ssh_tunnel->state()
     << " PID: "<< pid<< endl;
    finished = ssh_tunnel->waitForFinished(-1);
    //loop->exec();
    //exec();
    s<< "netcat is finished: "<< finished << endl;

thread_exit:
    //if ( listenSocket->isOpen() ) {
    //    listenSocket->disconnectFromHost();
    //    listenSocket->waitForDisconnected();
    //    s<< "socket is disconnected " << endl;
    //};
    s<< "ssh tunnel thread is finished " << endl;
}

/* private slots */
void SSH_Tunnel::write_to_viewer()
{
    QTextStream s(stdout);
    quint64 written = 0;
    char buff[buffSize];
    quint64 bytes = ssh_tunnel->read(buff, buffSize);
    while ( 0<bytes ) {
        //s<<buff;
        QTcpSocket *_sock = serverThread->server->nextPendingConnection();
        while ( _sock!=nullptr ) {
            written += _sock->write(buff, bytes);
            //_sock->waitForBytesWritten();
            _sock = serverThread->server->nextPendingConnection();
        };
        bytes = ssh_tunnel->read(buff, buffSize);
    };
    s<<"\nwrite_to_viewer: "<<written<<endl;
}
void SSH_Tunnel::write_to_remote_graphic_channel(QTcpSocket *_sock)
{
    QTextStream s(stdout);
    quint64 written = 0;
    char buff[buffSize];
    quint64 bytes = _sock->read(buff, buffSize);
    while ( 0<bytes ) {
        //s<<buff;
        written += ssh_tunnel->write(buff, bytes);
        //ssh_tunnel->waitForBytesWritten();
        bytes = _sock->read(buff, buffSize);
    };
    s<<"\nwrite_to_remote_graphic_channel: "<<written<<endl;
}
void SSH_Tunnel::resend_socket_errors(QAbstractSocket::SocketError _err)
{
    Q_UNUSED(_err);
    //QTextStream s(stdout);
    //s<<listenSocket->errorString()<<endl;
    //emit errMsg(listenSocket->errorString());
}
void SSH_Tunnel::resend_tunnel_errors(QProcess::ProcessError _err)
{
    Q_UNUSED(_err);
    QTextStream s(stdout);
    s<<ssh_tunnel->errorString()<<endl;
    emit errMsg(ssh_tunnel->errorString());
}
void SSH_Tunnel::thread_finished()
{
    QTextStream s(stdout);
    s<<"thread_finished" <<endl;
}
