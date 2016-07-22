#ifndef USB_HOST_DEVICE_EDIT_H
#define USB_HOST_DEVICE_EDIT_H

#include "create_widgets/common/_qwidget.h"
#include "create_widgets/domain/common_widgets/device_address.h"
#include "create_widgets/domain/common_widgets/disk_widgets/_startup_policy.h"

class USB_Host_Device_Edit : public _QWidget
{
    Q_OBJECT
public:
    explicit USB_Host_Device_Edit(QWidget *parent = nullptr);
    DeviceAddress       *addr;

private:
    QLabel              *vendorLabel, *productLabel;
    QLineEdit           *vendor, *product;
    _StartupPolicy      *startupPolicy;
    QWidget             *baseWdg;
    QGridLayout         *baseLayout;
    QVBoxLayout         *commonLayout;

public slots:
    QDomDocument         getDataDocument() const;
    void                 setDataDescription(const QString&);
};

#endif // USB_HOST_DEVICE_EDIT_H
