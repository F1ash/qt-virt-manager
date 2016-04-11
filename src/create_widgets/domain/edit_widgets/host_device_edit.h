#ifndef HOST_DEVICE_EDIT_H
#define HOST_DEVICE_EDIT_H

#include "create_widgets/domain/common_widgets/host_device.h"
#include "host_dev_edit_widgets/usb_host_device_edit.h"
#include "host_dev_edit_widgets/bch_host_device_edit.h"
#include "host_dev_edit_widgets/pci_host_device_edit.h"
#include "host_dev_edit_widgets/scsi_host_device_edit.h"

class HostDevice_Edit : public HostDevice
{
    Q_OBJECT
public:
    explicit HostDevice_Edit(
            QWidget        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr);

private:
    QStackedWidget  *infoEdit;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);

private slots:
    void             init_wdg();
};

#endif // HOST_DEVICE_EDIT_H
