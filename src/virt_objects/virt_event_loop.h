#ifndef VIRT_EVENT_LOOP_H
#define VIRT_EVENT_LOOP_H

#include "_virt_thread.h"

class VirtEventLoop : public _VirtThread
{
    Q_OBJECT
public:
    explicit VirtEventLoop(QObject *parent = nullptr);
    QString     libVersion;
    void        run();

signals:
    void        result(bool);
};

#endif // VIRT_EVENT_LOOP_H
