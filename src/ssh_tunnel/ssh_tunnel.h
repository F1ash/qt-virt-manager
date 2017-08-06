#ifndef SSH_TUNNEL_H
#define SSH_TUNNEL_H

#include <QThread>
#include <QVariantMap>
#include <QProcess>
#include <QEventLoop>
#include "tcp_server_thread.h"

class SSH_Tunnel : public QThread
{
    Q_OBJECT
public:
    explicit SSH_Tunnel(QObject *parent = nullptr);
    ~SSH_Tunnel();
    void        setData(QVariantMap);
    void        run();

signals:
    void        established(quint16);
    void        tunnel_finished();
    void        errMsg(const QString);

private:
    QString     remoteHost, remotePort, User,
                graphicsAddr, graphicsPort;
    QTcpSocket *listenSocket = nullptr;
    QProcess   *ssh_tunnel = nullptr;
    Q_PID       pid;
    //QEventLoop *loop;
    QAbstractEventDispatcher *evDsp;
    TCP_Server_Thread *serverThread = nullptr;

private slots:
    void        write_to_viewer();
    void        write_to_remote_graphic_channel(QTcpSocket*);
    void        resend_socket_errors(QAbstractSocket::SocketError);
    void        resend_tunnel_errors(QProcess::ProcessError);
    void        thread_finished();
};

#endif // SSH_TUNNEL_H
