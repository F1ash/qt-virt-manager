#ifndef DOM_CONTROL_THREAD_H
#define DOM_CONTROL_THREAD_H

#include "virt_objects/control_thread.h"

class DomControlThread : public ControlThread
{
    Q_OBJECT
public:
    explicit DomControlThread(QObject *parent = NULL);

signals:

private:

public slots:
    void             execAction(uint, TASK);

private slots:
    void             run();
    Result           getAllDomainList();
    Result           createDomain();
    Result           defineDomain();
    Result           startDomain();
    Result           pauseDomain();
    Result           destroyDomain();
    Result           resetDomain();
    Result           rebootDomain();
    Result           shutdownDomain();
    Result           saveDomain();
    Result           restoreDomain();
    Result           undefineDomain();
    Result           changeAutoStartDomain();
    Result           getDomainXMLDesc();
    Result           migrateDomain();
    Result           createSnapshoteDomain();
    Result           revertSnapshoteDomain();
    Result           deleteSnapshoteDomain();
};

#endif // DOM_CONTROL_THREAD_H
