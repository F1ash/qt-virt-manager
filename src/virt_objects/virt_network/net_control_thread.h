#ifndef NET_CONTROL_THREAD_H
#define NET_CONTROL_THREAD_H

#include <QThread>
#include <QStringList>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

enum Actions {
    GET_ALL_NETWORK,
    CREATE_NETWORK,
    CREATE_NETWORK_EXAMPLE,
    DEFINE_NETWORK,
    DEFINE_NETWORK_EXAMPLE,
    START_NETWORK,
    DESTROY_NETWORK,
    UNDEFINE_NETWORK,
    CHANGE_AUTOSTART
};

class NetControlThread : public QThread
{
    Q_OBJECT
public:
    explicit NetControlThread(QObject *parent = 0);

signals:
    void errorMsg(QString&);
    void resultData(QStringList);

private:
    Actions          action;
    bool             keep_alive;
    virConnect      *currWorkConnect = NULL;

public slots:
    bool setCurrentWorkConnect(virConnectPtr);
    void stop();
    void execAction(Actions);
    QStringList getAllNetworkList();

private slots:
    void run();

};

#endif // NET_CONTROL_THREAD_H
