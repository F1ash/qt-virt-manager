#include "ssh_tunnel.h"
#include <QTcpSocket>
//#include <QTextStream>

SSH_Tunnel::SSH_Tunnel(QObject *parent) :
    QThread(parent)
{
    ssh_tunnel = new QProcess();
    ssh_tunnel->setProcessChannelMode(QProcess::SeparateChannels);
}

void SSH_Tunnel::setData(QVariantMap _data)
{
    User         = _data.value("User").toString();
    remoteHost   = _data.value("RemoteHost").toString();
    remotePort   = _data.value("RemotePort").toString();
    graphicsAddr = _data.value("GraphicsAddr").toString();
    graphicsPort = _data.value("GraphicsPort").toString();
    graphicsSock = _data.value("GraphicsSock").toString();
}
void SSH_Tunnel::run()
{
    //QTextStream s(stdout);
    quint16 viewerPort = 0;
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
        _args << "-p" << remotePort << "-L";
        if ( !graphicsPort.isEmpty() && graphicsSock.isEmpty() ) {
            _args << QString("%1:%2:%3").arg(viewerPort).arg(graphicsAddr).arg(graphicsPort);
        } else if ( graphicsPort.isEmpty() && !graphicsSock.isEmpty() ) {
            _args << QString("%1:%2").arg(viewerPort).arg(graphicsSock);
        } else {
            _args << tr("data_error");
        };
        _args << QString("%1@%2").arg(User).arg(remoteHost);
        ssh_tunnel->start("ssh", _args, QIODevice::ReadWrite);
        //s << "command args: "<< _args.join(" ") << endl;
        if ( !ssh_tunnel->waitForStarted() ) continue;
        if ( !ssh_tunnel->isOpen() ) {
            ssh_tunnel->kill();
            continue;
        };
        listenSocket->close();
        connected = true;
        break;
    };
    listenSocket->deleteLater();

    emit established(viewerPort);
    //s<< "established: "<< viewerPort <<" State: "<<ssh_tunnel->state();
    finished = ssh_tunnel->waitForFinished(-1);
    //s<< "ssh tunnel thread is finished " << endl;
}
void SSH_Tunnel::stop()
{
    if ( ssh_tunnel==Q_NULLPTR ) return;
    if ( ssh_tunnel->isOpen() ) {
        ssh_tunnel->close();
        ssh_tunnel->kill();
    };
}
