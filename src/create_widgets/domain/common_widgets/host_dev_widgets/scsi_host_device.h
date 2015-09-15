#ifndef SCSI_HOST_DEVICE_H
#define SCSI_HOST_DEVICE_H

#include "create_widgets/domain/_qwidget.h"
#include <QListWidget>

class SCSI_Host_Device : public _QWidget
{
    Q_OBJECT
public:
    explicit SCSI_Host_Device(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtrPtr = NULL);

private:
    QListWidget     *devList;
    QVBoxLayout     *commonLayout;

    virErrorPtr      virtErrors;

signals:
    // TODO: write to Log
    void             errorMsg(QString);

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             setAvailabledSCSIDevices();

    void             sendConnErrors();
    void             sendGlobalErrors();
};

#endif // SCSI_HOST_DEVICE_H
