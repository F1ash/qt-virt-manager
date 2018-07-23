#ifndef SSH_TUNNEL_H
#define SSH_TUNNEL_H

#include <QThread>
#include <QVariantMap>
#include <QProcess>

class SSH_Tunnel : public QThread
{
    Q_OBJECT
public:
    explicit SSH_Tunnel(QObject *parent = Q_NULLPTR);
    void        setData(QVariantMap);
    void        run();
    void        stop();

signals:
    void        established(quint16);

private:
    QString     remoteHost, remotePort, User,
                graphicsAddr, graphicsPort, graphicsSock;
    QProcess   *ssh_tunnel = Q_NULLPTR;
};

#endif // SSH_TUNNEL_H
