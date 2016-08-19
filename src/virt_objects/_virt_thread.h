#ifndef _VIRT_THREAD_H
#define _VIRT_THREAD_H

#include <QThread>
#include <QString>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include <QDebug>

class _VirtThread : public QThread
{
    Q_OBJECT
public:
    explicit _VirtThread(
            QObject        *parent      = nullptr,
            virConnectPtr  *connPtrPtr  = nullptr);
    virtual ~_VirtThread()              = 0;
    bool             keep_alive;
    uint             number;
    virConnectPtr   *ptr_ConnPtr;
    virErrorPtr      virtErrors;

    virtual QString  sendConnErrors();
    virtual QString  sendGlobalErrors();

signals:
    void             errorMsg(const QString&, const uint);
    void             ptrIsNull();
public slots:
    virtual void     stop();
private:
    void             customErrorNotification(const QString&, const uint);
};

#endif // _VIRT_THREAD_H
