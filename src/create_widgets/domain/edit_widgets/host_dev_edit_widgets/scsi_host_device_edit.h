#ifndef SCSI_HOST_DEVICE_EDIT_H
#define SCSI_HOST_DEVICE_EDIT_H

#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/domain/common_widgets/device_address.h"
#include "adapter_address.h"
#include "iscsi_device.h"

class SCSI_Host_Device_Edit : public _QWidget
{
    Q_OBJECT
public:
    explicit SCSI_Host_Device_Edit(
            QWidget        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);

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
    void                 setDataDescription(QString&);

private slots:
    void                 scsiTypeChanged(int);
};

#endif // SCSI_HOST_DEVICE_EDIT_H
