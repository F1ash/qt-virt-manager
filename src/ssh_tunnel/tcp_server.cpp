#include "tcp_server.h"
#include <QAbstractEventDispatcher>
#include <QEventLoop>
#include <QTextStream>

TCP_Server::TCP_Server(QObject *parent) :
    QTcpServer(parent)
{
    //setMaxPendingConnections(1);
    //connect(this, SIGNAL(newConnection()),
    //        this, SLOT(useNewConnection()));

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

void TCP_Server::run_socket_event_loop()
{
    QAbstractEventDispatcher *evDsp =
            QAbstractEventDispatcher::instance();
    if ( evDsp==nullptr ) {
        return;
    };
    QTextStream s(stdout);
    //QEventLoop loop;
    while ( key ) {
        evDsp->processEvents(
                    QEventLoop::AllEvents |
                    QEventLoop::ExcludeUserInputEvents |
                    QEventLoop::WaitForMoreEvents);
        s<< "one server event or cycle" << endl;
    };
    s<< "TCP_Server finished" << endl;
}

void TCP_Server::stop_socket_event_loop()
{
    key = false;
}

void TCP_Server::useNewConnection()
{
    QTextStream s(stdout);
    s<< tr("newConnection") << endl;
    QTcpSocket *_sock = nextPendingConnection();
    if ( _sock==nullptr ) return;
    connect(_sock, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpError(QAbstractSocket::SocketError)));
    connect(_sock, SIGNAL(readyRead()),
            this, SLOT(tcpReady()), Qt::DirectConnection);
    connect(_sock, SIGNAL(disconnected()),
            _sock, SLOT(deleteLater()));
}

void TCP_Server::incomingConnection(quintptr descriptor)
{
    QTextStream s(stdout);
    s<< "newConnection" << endl;

    //QTcpSocket *_sock = new QTcpSocket(this);
    //_sock->setSocketDescriptor(descriptor);
    //connect(_sock, SIGNAL(error(QAbstractSocket::SocketError)),
    //        this, SLOT(tcpError(QAbstractSocket::SocketError)));
    ///connect(_sock, SIGNAL(readyRead()),
    //        this, SLOT(tcpReady()));
    //connect(_sock, SIGNAL(disconnected()),
    //        _sock, SLOT(deleteLater()));
    //addPendingConnection(_sock);

    TCP_Socket_Thread *thread = new TCP_Socket_Thread(descriptor, this);

    // connect signal/slot
    connect(thread, SIGNAL(socketReadyRead(QTcpSocket*)),
            this, SIGNAL(readyRead(QTcpSocket*)), Qt::DirectConnection);
    // once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()),
            thread, SLOT(deleteLater()));

    thread->start();
    s<< "newConnection thread started" << endl;
}

