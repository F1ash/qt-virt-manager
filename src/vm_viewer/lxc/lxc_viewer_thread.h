#ifndef LXC_VIEWER_THREAD_H
#define LXC_VIEWER_THREAD_H

#include "virt_objects/control_thread.h"

class LXC_ViewerThread : public ControlThread
{
    Q_OBJECT
public:
    explicit LXC_ViewerThread(QObject *parent = nullptr);
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
    void            sendDataToVMachine(const char*, ulong);

private slots:
    int             registerStreamEvents();
    void            unregisterStreamEvents();
    static void     freeData(void*);
    static void     streamEventCallBack(virStreamPtr, int, void*);
    void            updateStreamEvents(virStreamPtr, int);
    void            sendDataToDisplay();
};

#endif // LXC_VIEWER_THREAD_H
