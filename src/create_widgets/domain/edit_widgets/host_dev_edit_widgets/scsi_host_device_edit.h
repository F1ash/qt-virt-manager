#ifndef SCSI_HOST_DEVICE_EDIT_H
#define SCSI_HOST_DEVICE_EDIT_H

#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/domain/common_widgets/device_address.h"

class SCSI_Host_Device_Edit : public _QWidget
{
    Q_OBJECT
public:
    explicit SCSI_Host_Device_Edit(QWidget *parent = 0);

private:

    DeviceAddress       *addr;
    QVBoxLayout         *commonLayout;

public slots:
    QDomDocument         getDataDocument() const;
    void                 setDataDescription(QString&);
};

#endif // SCSI_HOST_DEVICE_EDIT_H
