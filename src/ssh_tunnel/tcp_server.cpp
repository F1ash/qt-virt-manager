#include "tcp_server.h"
#include <QTextStream>

TCP_Server::TCP_Server(QObject *parent) :
    QTcpServer(parent)
{
    //setMaxPendingConnections(1);
    //server_socket = new QTcpSocket(this);
    connect(this, SIGNAL(newConnection()),
            this, SLOT(useNewConnection()));

}
TCP_Server::~TCP_Server()
{
    //server_socket->disconnectFromHost();
    //server_socket->waitForDisconnected();
}

void TCP_Server::tcpReady()
{
    QTextStream s(stdout);
    s<< "tcpReady" << endl;
    emit readyRead(static_cast<QTcpSocket*>(sender()));
}

void TCP_Server::tcpError(QAbstractSocket::SocketError error)
{
    QTextStream s(stdout);
    s<< tr("TCP error: %1").arg(
            static_cast<QTcpSocket*>(sender())->errorString() )
     << endl;
}


bool TCP_Server::start_listen(int port_no)
{
    QTextStream s(stdout);
    if( !this->listen( QHostAddress::LocalHost, port_no ) ) {
        s<< tr("Cannot listen to port %1").arg(port_no) << endl;
        return false;
    } else {
        s<< tr("Listen to port %1").arg(port_no) << endl;
        return true;
    };
}

void TCP_Server::useNewConnection()
{
    QTextStream s(stdout);
    s<< tr("newConnection") << endl;
    QTcpSocket *_sock = nextPendingConnection();
    if ( _sock==nullptr ) return;
    connect( _sock, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpError(QAbstractSocket::SocketError)) );
    connect( _sock, SIGNAL(readyRead()),
             this, SLOT(tcpReady()) );
    connect(_sock, SIGNAL(disconnected()),
            _sock, SLOT(deleteLater()));
}
