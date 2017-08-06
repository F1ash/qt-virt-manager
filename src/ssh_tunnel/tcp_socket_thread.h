#ifndef TCP_SOCKET_THREAD_H
#define TCP_SOCKET_THREAD_H

#include <QThread>
#include <QTcpSocket>

class TCP_Socket_Thread : public QThread
{
    Q_OBJECT
public:
    explicit TCP_Socket_Thread(
            qintptr ID,
            QObject *parent = nullptr);

    void        run();

signals:
    void        error(QTcpSocket::SocketError);
    void        socketReadyRead(QTcpSocket*);

public slots:
    void        readyRead();
    void        disconnected();

private:
    QTcpSocket *socket = nullptr;
    qintptr     socketDescriptor;
};

#endif // TCP_SOCKET_THREAD_H
