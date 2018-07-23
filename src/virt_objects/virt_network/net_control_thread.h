#ifndef NET_CONTROL_THREAD_H
#define NET_CONTROL_THREAD_H

#include "virt_objects/control_thread.h"

class NetControlThread : public ControlThread
{
    Q_OBJECT
public:
    explicit NetControlThread(QObject *parent = Q_NULLPTR);

signals:

private:

public slots:
    void         execAction(uint, TASK);

private slots:
    void         run();
    Result       getAllNetworkList();
    Result       createNetwork();
    Result       defineNetwork();
    Result       startNetwork();
    Result       destroyNetwork();
    Result       undefineNetwork();
    Result       changeAutoStartNetwork();
    Result       getVirtNetXMLDesc();
};

#endif // NET_CONTROL_THREAD_H
