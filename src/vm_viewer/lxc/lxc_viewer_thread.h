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

private:
    bool            streamRegistered, connRef, EndOfFile;
    int             ptySlaveFd;
    QString         domain;
    virStream      *stream;
    virDomainPtr    domainPtr;

public slots:
    void            setData(QString&, virDomainPtr, int);
    void            run();
    void            sendDataToVMachine(const char*, int);

private slots:
    int             registerStreamEvents();
    void            unregisterStreamEvents();
    static void     freeData(void*);
    static void     streamEventCallBack(virStreamPtr, int, void*);
    void            updateStreamEvents(virStreamPtr, int);
    void            sendDataToDisplay();
    void            closeStream();
};

#endif // LXC_VIEWER_THREAD_H
