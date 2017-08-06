#ifndef TCP_SERVER_THREAD_H
#define TCP_SERVER_THREAD_H

#include <QThread>
#include "tcp_server.h"

class TCP_Server_Thread : public QThread
{
    Q_OBJECT
public:
    explicit TCP_Server_Thread(QObject *parent = nullptr);
    TCP_Server  *server = nullptr;
    void         run();
    void         stop();
    bool         key = true;
};

#endif // TCP_SERVER_THREAD_H
