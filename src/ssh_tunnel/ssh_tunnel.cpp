//extern "C" {
//#include "ssh_common.c"
//}
#include "ssh_tunnel.h"
#include <QTcpSocket>
//#include <QTextStream>
//#include <signal.h>

#define buffSize  4096

SSH_Tunnel::SSH_Tunnel(QObject *parent) :
    QThread(parent)
{
    ssh_tunnel = new QProcess(this);
    ssh_tunnel->setProcessChannelMode(QProcess::SeparateChannels);
    connect(ssh_tunnel, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(resend_tunnel_errors(QProcess::ProcessError)));
    connect(this, SIGNAL(finished()),
            this, SLOT(thread_finished()));
}
SSH_Tunnel::~SSH_Tunnel()
{
    //QTextStream s(stdout);
    if ( ssh_tunnel!=nullptr ) {
        if ( ssh_tunnel->isOpen() ) {
            ssh_tunnel->close();
            //ssh_tunnel->waitForFinished();
            //s<< "ssh_tunnel is finished at thread deletion" << endl;
        //} else {
        //    ssh_tunnel->kill();
        //    s<< "ssh_tunnel is killed at thread deletion" << endl;
        };
        //kill(pid, SIGKILL);
        //ssh_tunnel->kill();
        ssh_tunnel->deleteLater();
    };
    //s<< "~SSH_Tunnel()" << endl;
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
    //QTextStream s(stdout);
    quint16 viewerPort = 33333;
    bool connected = false;
    bool finished = false;
    QStringList _args;
    QString ssh_command;

    // create SSH tunnel
    QTcpSocket *listenSocket = new QTcpSocket();
    while ( !connected ) {

        if ( !listenSocket->bind(0) ) {
            continue;
        };
        viewerPort = listenSocket->localPort();
        _args.clear();
        ssh_command.clear();
        _args   << "-p" << remotePort
                << "-L"
                << QString("%1:%2:%3").arg(viewerPort).arg(graphicsAddr).arg(graphicsPort)
                << QString("%1@%2").arg(User).arg(remoteHost);
        ssh_tunnel->start("ssh", _args, QIODevice::ReadWrite);
        //s << "command args: "<< _args.join(" ") << endl;
        if ( !ssh_tunnel->waitForStarted() ) continue;
        if ( !ssh_tunnel->isOpen() ) {
            ssh_tunnel->kill();
            continue;
        };
        listenSocket->close();
        listenSocket->deleteLater();
        //pid = ssh_tunnel->pid();
        connected = true;
        break;
    };

    emit established(viewerPort);
    //s<< "established: "<< viewerPort <<" State: "<<ssh_tunnel->state()
    // << " PID: "<< pid<< endl;
    finished = ssh_tunnel->waitForFinished(-1);
    //s<< "ssh tunnel thread is finished " << endl;
}

/* private slots */
void SSH_Tunnel::resend_tunnel_errors(QProcess::ProcessError _err)
{
    Q_UNUSED(_err);
    //QTextStream s(stdout);
    //s<<ssh_tunnel->errorString()<<endl;
    emit errMsg(ssh_tunnel->errorString());
}
void SSH_Tunnel::thread_finished()
{
    //QTextStream s(stdout);
    //s<<"ssh_command_process_is_finished" <<endl;
}
