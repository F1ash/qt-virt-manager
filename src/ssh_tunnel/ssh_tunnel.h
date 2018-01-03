#ifndef SSH_TUNNEL_H
#define SSH_TUNNEL_H

#include <QThread>
#include <QVariantMap>
#include <QProcess>

class SSH_Tunnel : public QThread
{
    Q_OBJECT
public:
    explicit SSH_Tunnel(QObject *parent = nullptr);
    void        setData(QVariantMap);
    void        run();
    void        stop();

signals:
    void        established(quint16);

private:
    QString     remoteHost, remotePort, User,
                graphicsAddr, graphicsPort, graphicsSock;
    QProcess   *ssh_tunnel = nullptr;
};

#endif // SSH_TUNNEL_H
