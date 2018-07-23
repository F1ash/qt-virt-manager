#ifndef SCSI_HOST_DEVICE_EDIT_H
#define SCSI_HOST_DEVICE_EDIT_H

#include "create_widgets/common/_qwidget.h"
#include "create_widgets/domain/device_address.h"
#include "adapter_address.h"
#include "iscsi_device.h"

class SCSI_Host_Device_Edit : public _QWidget
{
    Q_OBJECT
public:
    explicit SCSI_Host_Device_Edit(
            QWidget        *parent      = Q_NULLPTR,
            virConnectPtr*  connPtrPtr  = Q_NULLPTR);

private:
    QLabel              *info, *infoText;
    QComboBox           *scsiType;
    QLineEdit           *devName;

    AdapterAddress      *scsiDevice;
    ISCSI_Device        *iscsiDevice;
    QStackedWidget      *scsiDevices;

    QGridLayout         *baseLayout;
    QWidget             *baseWdg;

    DeviceAddress       *addr;
    QVBoxLayout         *commonLayout;

public slots:
    QDomDocument         getDataDocument() const;
    void                 setDataDescription(const QString&);

private slots:
    void                 scsiTypeChanged(int);
};

#endif // SCSI_HOST_DEVICE_EDIT_H
