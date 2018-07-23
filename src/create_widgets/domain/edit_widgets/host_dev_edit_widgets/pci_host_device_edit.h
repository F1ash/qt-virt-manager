#ifndef PCI_HOST_DEVICE_EDIT_H
#define PCI_HOST_DEVICE_EDIT_H

#include "create_widgets/common/_qwidget.h"
#include "create_widgets/domain/device_address.h"

class PCI_Host_Device_Edit : public _QWidget
{
    Q_OBJECT
public:
    explicit PCI_Host_Device_Edit(QWidget *parent = Q_NULLPTR);
    DeviceAddress       *addr;

private:
    QVBoxLayout         *commonLayout;

public slots:
    QDomDocument         getDataDocument() const;
    void                 setDataDescription(const QString&);
};

#endif // PCI_HOST_DEVICE_EDIT_H
