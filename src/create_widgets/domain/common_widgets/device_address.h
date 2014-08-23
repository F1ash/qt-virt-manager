#ifndef DEVICE_ADDRESS_H
#define DEVICE_ADDRESS_H

#include <QCheckBox>
#include <QComboBox>
#include <QVBoxLayout>
#include "address_widgets/pci_addr.h"
#include "address_widgets/drive_addr.h"
#include "address_widgets/virtioserial_addr.h"
#include "address_widgets/ccid_addr.h"
#include "address_widgets/usb_addr.h"
#include "address_widgets/spaprvio_addr.h"
#include "address_widgets/ccw_addr.h"
#include "address_widgets/isa_addr.h"


class DeviceAddress : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceAddress(QWidget *parent = 0);
    ~DeviceAddress();

signals:

private:
    QCheckBox       *use;
    QComboBox       *type;
    QVBoxLayout     *commonLayout;
    _Addr           *info = NULL;

public slots:
    AttrList getAttrList() const;

private slots:
    void addressUsed(bool);
    void addrTypeChanged(int);
};

#endif // DEVICE_ADDRESS_H
