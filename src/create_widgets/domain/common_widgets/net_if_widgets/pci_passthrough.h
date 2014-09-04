#ifndef PCI_PASSTHROUGH_H
#define PCI_PASSTHROUGH_H

#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/domain/common_widgets/address_widgets/pci_addr.h"
#include "virtual_port.h"

class PCI_Passthrough : public _QWidget
{
    Q_OBJECT
public:
    explicit PCI_Passthrough(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QString              connType;
    QLabel              *attentionIcon, *attention,
                        *driverLabel, *macLabel,
                        *pciAddrLabel;
    QComboBox           *driver;
    QLineEdit           *mac;
    PciAddr             *addr;
    VirtualPort         *virtPort;
    QWidget             *baseWdg;
    QGridLayout         *baseLayout;
    QVBoxLayout         *commonLayout;

public slots:
    QDomDocument getDevDocument() const;
};

#endif // PCI_PASSTHROUGH_H
