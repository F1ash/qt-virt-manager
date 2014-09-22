#ifndef HOST_DEVICE_H
#define HOST_DEVICE_H

#include <QStackedWidget>
#include "host_dev_widgets/usb_host_device.h"
#include "host_dev_widgets/pci_host_device.h"
#include "host_dev_widgets/scsi_host_device.h"
#include "host_dev_widgets/bch_host_device.h"

class HostDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit HostDevice(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QComboBox       *type;
    QStackedWidget  *info;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;
};

#endif // HOST_DEVICE_H
