#ifndef SSH_TUNNEL_H
#define SSH_TUNNEL_H

#include <QThread>
#include <QVariantMap>
#include <QProcess>
#include <QTcpSocket>

class SSH_Tunnel : public QThread
{
    Q_OBJECT
public:
    explicit SSH_Tunnel(QObject *parent = nullptr);
    void        setData(QVariantMap);
    void        run();

signals:
    void        established(const uint);
    void        errMsg(const QString);

private:
    uint        portForViewer = 0;
    QString     remoteHost, remotePort, User,
                graphicsAddr, graphicsPort;
    QTcpSocket *socketToViewerPort = nullptr;
    QProcess   *ssh_tunnel = nullptr;

private slots:
    void        write_to_viewer();
    void        write_to_remote_graphic_channel();
    void        resend_socket_errors(QAbstractSocket::SocketError);
    void        resend_tunnel_errors(QProcess::ProcessError);
};

#endif // SSH_TUNNEL_H
