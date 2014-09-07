#ifndef DEVICE_ADDRESS_H
#define DEVICE_ADDRESS_H

#include <QCheckBox>
#include "address_widgets/pci_addr.h"
#include "address_widgets/drive_addr.h"
#include "address_widgets/virtioserial_addr.h"
#include "address_widgets/ccid_addr.h"
#include "address_widgets/usb_addr.h"
#include "address_widgets/spaprvio_addr.h"
#include "address_widgets/ccw_addr.h"
#include "address_widgets/isa_addr.h"
#include "create_widgets/domain/_changed.h"

class DeviceAddress : public _Changed
{
    Q_OBJECT
public:
    explicit DeviceAddress(QWidget *parent = 0);
    ~DeviceAddress();
    QComboBox       *type;

private:
    QCheckBox       *use;
    QVBoxLayout     *commonLayout;
    _Addr           *info = NULL;

public slots:
    AttrList getAttrList() const;

private slots:
    void addressUsed(bool);
    void addrTypeChanged(int);
};

#endif // DEVICE_ADDRESS_H
