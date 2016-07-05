#ifndef PCI_PASSTHROUGH_H
#define PCI_PASSTHROUGH_H

#include "create_widgets/domain/_qwidget_threaded.h"
#include "create_widgets/domain/common_widgets/address_widgets/pci_addr.h"
#include "virtual_port.h"
#include "create_widgets/domain/common_widgets/device_address.h"

class PCI_Passthrough : public _QWidget_Threaded
{
    Q_OBJECT
public:
    explicit PCI_Passthrough(
            QWidget        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);

private:
    QString              connType;
    QLabel              *attentionIcon, *attention,
                        *driverLabel, *macLabel,
                        *pciAddrLabel;
    QComboBox           *driver;
    QLineEdit           *mac;
    PciAddr             *pciAddr;
    QGridLayout         *baseLayout;
    QWidget             *baseWdg;
    VirtualPort         *virtPort;
    DeviceAddress       *addr;
    QVBoxLayout         *commonLayout;

public slots:
    QDomDocument         getDataDocument() const;
    void                 setDataDescription(const QString&);

private slots:
    void                 init_wdg();
};

#endif // PCI_PASSTHROUGH_H
