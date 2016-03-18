#ifndef SCSI_HOST_DEVICE_H
#define SCSI_HOST_DEVICE_H

#include "create_widgets/domain/_qwidget.h"
#include <QListWidget>
#include "virt_objects/_virt_thread.h"

class iscsi_hostHlpThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit iscsi_hostHlpThread(
            QObject        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);
    void             run();
signals:
    void             result(QStringList&);
};

class SCSI_Host_Device : public _QWidget
{
    Q_OBJECT
public:
    explicit SCSI_Host_Device(
            QWidget        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);

private:
    QListWidget     *devList;
    QVBoxLayout     *commonLayout;

    iscsi_hostHlpThread
                    *hlpThread;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             setAvailabledSCSIDevices(QStringList&);
    void             emitCompleteSignal();
};

#endif // SCSI_HOST_DEVICE_H
