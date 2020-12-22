#ifndef VIRT_EVENT_LOOP_H
#define VIRT_EVENT_LOOP_H

#include "_virt_thread.h"

class VirtEventLoop : public _VirtThread
{
    Q_OBJECT
public:
    explicit VirtEventLoop(QObject *parent = Q_NULLPTR);
    QString     libVersion;
    bool        isSuccess() const;
    void        run();

signals:
    void        result(bool);
    void        timerStopped();

private:
    bool        success;
    int         timer_id;
    static void callback(int, void*);
    static void freeCallback(void*);

public slots:
    void        stop();

};

#endif // VIRT_EVENT_LOOP_H
