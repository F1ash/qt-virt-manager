#include "ssh_tunnel.h"
#include <QTcpSocket>
//#include <QTextStream>

SSH_Tunnel::SSH_Tunnel(QObject *parent) :
    QThread(parent)
{
    ssh_tunnel = new QProcess(this);
    ssh_tunnel->setProcessChannelMode(QProcess::SeparateChannels);
    connect(ssh_tunnel, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(resend_tunnel_errors(QProcess::ProcessError)));
    //connect(this, SIGNAL(finished()),
    //        this, SLOT(thread_finished()));
}
SSH_Tunnel::~SSH_Tunnel()
{
    //QTextStream s(stdout);
    if ( ssh_tunnel->isOpen() ) {
        ssh_tunnel->close();
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
    //QTextStream s(stdout);
    quint16 viewerPort = 33333;
    bool connected = false;
    bool finished = false;
    QStringList _args;

    // create SSH tunnel
    QTcpSocket *listenSocket = new QTcpSocket();
    while ( !connected ) {

        if ( !listenSocket->bind(0) ) {
            continue;
        };
        viewerPort = listenSocket->localPort();
        _args.clear();
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
