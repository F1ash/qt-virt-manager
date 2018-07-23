#ifndef DEVICE_ADDRESS_H
#define DEVICE_ADDRESS_H

#include <QCheckBox>
#include <QStackedWidget>
#include "address_widgets/pci_addr.h"
#include "address_widgets/drive_addr.h"
#include "address_widgets/virtioserial_addr.h"
#include "address_widgets/ccid_addr.h"
#include "address_widgets/usb_addr.h"
#include "address_widgets/spaprvio_addr.h"
#include "address_widgets/ccw_addr.h"
#include "address_widgets/isa_addr.h"

class DeviceAddress : public _Changed
{
    Q_OBJECT
public:
    explicit DeviceAddress(QWidget *parent = Q_NULLPTR);
    QComboBox       *type;
    QCheckBox       *use;
    QStackedWidget  *info;

private:
    QVBoxLayout     *commonLayout;

public slots:
    AttrList         getAttrList() const;
    QWidget*         getCurrentAddrWidget();
    void             setCurrentAddrWidget(int);

private slots:
    void             addressUsed(bool);
};

#endif // DEVICE_ADDRESS_H
