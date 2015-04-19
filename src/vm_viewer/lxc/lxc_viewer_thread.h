#ifndef LXC_VIEWER_THREAD_H
#define LXC_VIEWER_THREAD_H

#include "virt_objects/control_thread.h"
#include <unistd.h>

class LXC_ViewerThread : public ControlThread
{
    Q_OBJECT
public:
    LXC_ViewerThread(QObject *parent = NULL);
    ~LXC_ViewerThread();

signals:
    void            termEOF();

private:
    int             ptySlaveFd = -1;
    QString         domain;
    virStream      *stream = NULL;
    virDomainPtr    domainPtr = NULL;

public slots:
    void            setData(QString&, virDomainPtr, int);
    void            run();
    void            stop();
    int             registerStreamEvents();
    void            sendDataToVMachine(const char*, int);

private slots:
    int             unregisterStreamEvents();
    static void     freeData(void*);
    static void     streamEventCallBack(virStreamPtr, int, void*);
    void            updateStreamEvents(virStreamPtr, int);
    void            sendDataToDisplay(virStreamPtr);
    void            closeStream();
};

#endif // LXC_VIEWER_THREAD_H
