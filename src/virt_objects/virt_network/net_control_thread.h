#ifndef NET_CONTROL_THREAD_H
#define NET_CONTROL_THREAD_H

#include <QThread>
#include <QDir>
#include <QTemporaryFile>
#include <QStringList>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

enum NetActions {
    GET_ALL_NETWORK,
    CREATE_NETWORK,
    DEFINE_NETWORK,
    START_NETWORK,
    DESTROY_NETWORK,
    UNDEFINE_NETWORK,
    CHANGE_NET_AUTOSTART,
    GET_NET_XML_DESC,
    NET_EMPTY_ACTION
};

class NetControlThread : public QThread
{
    Q_OBJECT
public:
    explicit NetControlThread(QObject *parent = 0);

signals:
    void errorMsg(QString);
    void resultData(NetActions, QStringList);

private:
    NetActions          action;
    QStringList      args;
    bool             keep_alive;
    virConnect      *currWorkConnect = NULL;
    virErrorPtr      virtErrors;

public slots:
    bool setCurrentWorkConnect(virConnectPtr);
    void stop();
    void execAction(NetActions, QStringList);

private slots:
    void run();
    QStringList getAllNetworkList();
    QStringList createNetwork();
    QStringList defineNetwork();
    QStringList startNetwork();
    QStringList destroyNetwork();
    QStringList undefineNetwork();
    QStringList changeAutoStartNetwork();
    QStringList getVirtNetXMLDesc();

    void sendConnErrors();
    void sendGlobalErrors();

};

#endif // NET_CONTROL_THREAD_H
