#ifndef SSH_TUNNEL_H
#define SSH_TUNNEL_H

#include <QThread>

class SSH_Tunnel : public QThread
{
    Q_OBJECT
public:
    explicit SSH_Tunnel(QObject *parent = nullptr);
    void    setData();
    void    run();
};

#endif // SSH_TUNNEL_H
