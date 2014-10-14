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
#include "create_widgets/domain/_changed.h"

class DeviceAddress : public _Changed
{
    Q_OBJECT
public:
    explicit DeviceAddress(QWidget *parent = NULL);
    QComboBox       *type;
    QCheckBox       *use;

private:
    QVBoxLayout     *commonLayout;
    QStackedWidget  *info;

public slots:
    AttrList         getAttrList() const;
    QWidget*         getCurrentAddrWidget();
    void             setCurrentAddrWidget(int);

private slots:
    void             addressUsed(bool);
};

#endif // DEVICE_ADDRESS_H
