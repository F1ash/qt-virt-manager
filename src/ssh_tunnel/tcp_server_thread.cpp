#include "tcp_server_thread.h"
#include <QAbstractEventDispatcher>
#include <QTextStream>

TCP_Server_Thread::TCP_Server_Thread(QObject *parent) :
    QThread(parent)
{
    server = new TCP_Server(this);
}
void TCP_Server_Thread::run()
{
    QTextStream s(stdout);
    server->run_socket_event_loop();
    /*
    QAbstractEventDispatcher *evDsp =
            QAbstractEventDispatcher::instance(this);
    if ( evDsp==nullptr ) {
        return;
    };
    this->setEventDispatcher(evDsp);
    QEventLoop loop;
    while ( key ) {
        loop.processEvents(
                    QEventLoop::AllEvents |
                    QEventLoop::ExcludeUserInputEvents,
                    30000);
        //s<< "one server event or cycle" << endl;
    };
    */
    s<< "TCP_Server_Thread finished" << endl;
}
void TCP_Server_Thread::stop()
{
    //server->stop_socket_event_loop();
    key = false;
}
