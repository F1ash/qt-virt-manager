#ifndef INTERFACE_CONTROL_THREAD_H
#define INTERFACE_CONTROL_THREAD_H

#include "virt_objects/control_thread.h"

class InterfaceControlThread : public ControlThread
{
    Q_OBJECT
public:
    explicit InterfaceControlThread(QObject *parent = nullptr);

signals:

private:

public slots:
    void         execAction(uint, TASK);

private slots:
    void         run();
    Result       getAllIfaceList();
    Result       startIface();
    Result       destroyIface();
    Result       defineIface();
    Result       undefineIface();
    Result       ifaceChangeBegin();
    Result       ifaceChangeCommit();
    Result       ifaceChangeRollback();
    Result       getVirtIfaceXMLDesc();
};

#endif // INTERFACE_CONTROL_THREAD_H
