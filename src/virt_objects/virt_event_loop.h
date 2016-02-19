#ifndef VIRT_EVENT_LOOP_H
#define VIRT_EVENT_LOOP_H

#include "_virt_thread.h"

class VirtEventLoop : public _VirtThread
{
    Q_OBJECT
public:
    explicit VirtEventLoop(QObject *parent = nullptr);
    void run();
};

#endif // VIRT_EVENT_LOOP_H
