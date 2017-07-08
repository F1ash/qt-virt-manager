#ifndef SSH_TUNNEL_H
#define SSH_TUNNEL_H

#include <QThread>
#include <QVariantMap>

class SSH_Tunnel : public QThread
{
    Q_OBJECT
public:
    explicit SSH_Tunnel(QObject *parent = nullptr);
    void    setData(QVariantMap);
    void    run();

signals:
    void    connection_result(bool, uint);

private:
    uint    portForViewer = 0;

private slots:
    uint    get_free_port_for_listen();
    void    write_to_viewer();
    void    write_to_remote_graphic_channel();
};

#endif // SSH_TUNNEL_H
