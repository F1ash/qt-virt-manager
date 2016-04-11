#ifndef HOST_DEVICE_H
#define HOST_DEVICE_H

#include <QStackedWidget>
#include "host_dev_widgets/usb_host_device.h"
#include "host_dev_widgets/pci_host_device.h"
#include "host_dev_widgets/scsi_host_device.h"
#include "host_dev_widgets/bch_host_device.h"
#include "create_widgets/domain/_qwidget_threaded.h"

#define QEMU_DEV_LIST QStringList()\
    <<"USB"<<"PCI"<<"SCSI"

#define LXC_DEV_LIST QStringList()\
    <<"USB"<<"Block/Char"

class HostDevice : public _QWidget_Threaded
{
    Q_OBJECT
public:
    explicit HostDevice(
            QWidget        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);
    QComboBox       *type;
    QStackedWidget  *info;
    QVBoxLayout     *commonLayout;

private:
    uint             completedWdg = 0;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    virtual void     init_wdg();
    void             emitCompleteSignal();
};

#endif // HOST_DEVICE_H
