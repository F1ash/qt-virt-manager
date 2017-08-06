#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QTcpServer>
#include "tcp_socket_thread.h"

class TCP_Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit TCP_Server(QObject *parent = nullptr);

signals:
    void        readyRead(QTcpSocket*);

private:
    bool        key = true;

public slots:
    bool        start_listen(int);
    void        run_socket_event_loop();
    void        stop_socket_event_loop();

private slots:
    void        tcpReady();
    void        tcpError(QAbstractSocket::SocketError);
    void        useNewConnection();

protected:
    void        incomingConnection(quintptr);
};

#endif // TCP_SERVER_H
