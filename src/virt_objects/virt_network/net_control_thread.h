#ifndef NET_CONTROL_THREAD_H
#define NET_CONTROL_THREAD_H

#include "virt_objects/control_thread.h"

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

class NetControlThread : public ControlThread
{
    Q_OBJECT
public:
    explicit NetControlThread(QObject *parent = NULL);

signals:
    void resultData(NetActions, Result);

private:
    NetActions  action;

public slots:
    void execAction(NetActions, QStringList);

private slots:
    void run();
    Result getAllNetworkList();
    Result createNetwork();
    Result defineNetwork();
    Result startNetwork();
    Result destroyNetwork();
    Result undefineNetwork();
    Result changeAutoStartNetwork();
    Result getVirtNetXMLDesc();

};

#endif // NET_CONTROL_THREAD_H
