#ifndef NWFILTER_CONTROL_THREAD_H
#define NWFILTER_CONTROL_THREAD_H

#include "virt_objects/control_thread.h"

class NWFilterControlThread : public ControlThread
{
    Q_OBJECT
public:
    explicit NWFilterControlThread(QObject *parent = Q_NULLPTR);

signals:

private:

public slots:
    void         execAction(uint, TASK);

private slots:
    void         run();
    Result       getAllNWFilterList();
    Result       defineNWFilter();
    Result       undefineNWFilter();
    Result       getVirtNWFilterXMLDesc();
};

#endif // NWFILTER_CONTROL_THREAD_H
