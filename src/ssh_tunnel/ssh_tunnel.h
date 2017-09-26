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
    ~SSH_Tunnel();
    void        setData(QVariantMap);
    void        run();

signals:
    void        established(quint16);
    void        errMsg(const QString&);

private:
    QString     remoteHost, remotePort, User,
                graphicsAddr, graphicsPort;
    QProcess   *ssh_tunnel = nullptr;

private slots:
    void        resend_tunnel_errors(QProcess::ProcessError);
    void        thread_finished();
};

#endif // SSH_TUNNEL_H
