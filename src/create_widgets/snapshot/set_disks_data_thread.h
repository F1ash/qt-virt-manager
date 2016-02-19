#ifndef SET_DISKS_DATA_THREAD_H
#define SET_DISKS_DATA_THREAD_H

#include "virt_objects/control_thread.h"
#include <QDomDocument>

class SetDisksDataThread : public ControlThread
{
    Q_OBJECT
public:
    explicit SetDisksDataThread(QObject *parent = nullptr);
    ~SetDisksDataThread();
    void        run();

signals:
    void        diskData(QDomElement&);
};

#endif // SET_DISKS_DATA_THREAD_H
