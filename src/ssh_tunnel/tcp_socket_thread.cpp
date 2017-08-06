#include "tcp_socket_thread.h"
#include <QTextStream>

TCP_Socket_Thread::TCP_Socket_Thread(
        qintptr  ID,
        QObject *parent) :
    QThread(parent), socketDescriptor(ID)
{

}

void TCP_Socket_Thread::run()
{
    // thread starts here
    QTextStream s(stdout);
    s << " Thread started" << endl;

    socket = new QTcpSocket(this);

    // set the ID
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(socket->error());
        return;
    }

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.

    connect(socket, SIGNAL(readyRead()),
            this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()),
            this, SLOT(disconnected()));

    // We'll have multiple clients, we want to know which is which
    s << socketDescriptor << " Client connected" << endl;

    // make this thread a loop,
    // thread will stay alive so that signal/slot to function properly
    // not dropped out in the middle when thread dies

    exec();
}

void TCP_Socket_Thread::readyRead()
{
    // get the information
    //QByteArray Data = socket->readAll();

    // will write on server side window
    //qDebug() << socketDescriptor << " Data in: " << Data;

    //socket->write(Data);
    QTextStream s(stdout);
    s << " readyRead" << endl;
    emit socketReadyRead(socket);
}

void TCP_Socket_Thread::disconnected()
{
    QTextStream s(stdout);
    s << socketDescriptor << " Disconnected" << endl;

    socket->deleteLater();
    exit(0);
}
