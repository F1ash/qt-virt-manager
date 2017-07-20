#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class TCP_Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit TCP_Server(QObject *parent = nullptr);
    ~TCP_Server();
    QTcpSocket *server_socket;

signals:
    void        readyRead(QTcpSocket*);

public slots:
    void        tcpReady();
    void        tcpError( QAbstractSocket::SocketError error );
    bool        start_listen(int port_no);

private slots:
    void        useNewConnection();
};

#endif // TCP_SERVER_H
