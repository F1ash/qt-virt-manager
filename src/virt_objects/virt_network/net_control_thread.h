#ifndef NET_CONTROL_THREAD_H
#define NET_CONTROL_THREAD_H

#include <QThread>
#include <QFile>
#include <QStringList>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

enum Actions {
    GET_ALL_NETWORK,
    CREATE_NETWORK,
    DEFINE_NETWORK,
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
    void errorMsg(QString);
    void resultData(Actions, QStringList);

private:
    Actions          action;
    QStringList      args;
    bool             keep_alive;
    virConnect      *currWorkConnect = NULL;
    virErrorPtr      virtErrors;

public slots:
    bool setCurrentWorkConnect(virConnectPtr);
    void stop();
    void execAction(Actions, QStringList);

private slots:
    void run();
    QStringList getAllNetworkList();
    QStringList createNetwork();
    QStringList defineNetwork();
    QStringList startNetwork();
    QStringList destroyNetwork();
    QStringList undefineNetwork();
    QStringList changeAutoStartNetwork();

    void sendConnErrors();
    void sendGlobalErrors();

};

#endif // NET_CONTROL_THREAD_H
