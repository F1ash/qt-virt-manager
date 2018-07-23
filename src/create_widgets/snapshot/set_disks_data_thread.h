#ifndef SET_DISKS_DATA_THREAD_H
#define SET_DISKS_DATA_THREAD_H

#include "virt_objects/control_thread.h"
#include <QDomDocument>

class SetDisksDataThread : public ControlThread
{
    Q_OBJECT
public:
    explicit SetDisksDataThread(QObject *parent = Q_NULLPTR);
    ~SetDisksDataThread();
    void        run();

signals:
    void        diskData(QDomElement&);
};

#endif // SET_DISKS_DATA_THREAD_H
