#include "device_address.h"

DeviceAddress::DeviceAddress(QWidget *parent) :
    _Changed(parent)
{
    use = new QCheckBox("Use address", this);
    type =new QComboBox(this);
    type->setVisible(false);
    type->addItem("PCI addresses", "pci");
    type->addItem("Drive addresses", "drive");
    type->addItem("Virtio-serial address", "virtio-serial");
    type->addItem("A CCID address, for smart-cards", "ccid");
    type->addItem("USB addresses", "usb");
    type->addItem("On PowerPC", "spapr-vio");
    type->addItem("s390 guests", "ccw");
    type->addItem("ISA addresses", "isa");
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(use);
    commonLayout->addWidget(type);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(use, SIGNAL(toggled(bool)),
            this, SLOT(addressUsed(bool)));
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(addrTypeChanged(int)));
}
DeviceAddress::~DeviceAddress()
{
    disconnect(use, SIGNAL(toggled(bool)),
               this, SLOT(addressUsed(bool)));
    disconnect(type, SIGNAL(currentIndexChanged(int)),
               this, SLOT(addrTypeChanged(int)));
    delete use;
    use = 0;
    delete type;
    type = 0;
    if ( info!=NULL ) {
        delete info;
        info = 0;
    };
    delete commonLayout;
    commonLayout = 0;
}

/* public slots */
AttrList DeviceAddress::getAttrList() const
{
    AttrList _ret;
    if ( use->isChecked() && info!=NULL ) {
        // get attribute list from current address type
        _ret = info->getAttrList();
        /*
         * Every address has a mandatory attribute type
         * that describes which bus the device is on.
         */
        QString _type = type->itemData(
                    type->currentIndex(),
                    Qt::UserRole)
                .toString();
        if ( !_type.isEmpty() )
            _ret.insert("type", _type);
    };
    return _ret;
}

/* private slots */
void DeviceAddress::addressUsed(bool state)
{
    type->setVisible(state);
    if ( info!=NULL ) info->setVisible(state);
    emit dataChanged();
}
void DeviceAddress::addrTypeChanged(int i)
{
    if ( info!=NULL ) {
        commonLayout->removeWidget(info);
        disconnect(info, SIGNAL(dataChanged()),
                   this, SLOT(stateChanged()));
        delete info;
        info = 0;
    };
    if ( type->itemData(i, Qt::UserRole).toString()=="pci" ) {
        info = new PciAddr(this);
    } else if ( type->itemData(i, Qt::UserRole).toString()=="drive" ) {
        info = new DriveAddr(this);
    } else if ( type->itemData(i, Qt::UserRole).toString()=="virtio-serial" ) {
        info = new VirtioSerialAddr(this);
    } else if ( type->itemData(i, Qt::UserRole).toString()=="ccid" ) {
        info = new CCIDAddr(this);
    } else if ( type->itemData(i, Qt::UserRole).toString()=="usb" ) {
        info = new USBAddr(this);
    } else if ( type->itemData(i, Qt::UserRole).toString()=="spapr-vio" ) {
        info = new SpaprVioAddr(this);
    } else if ( type->itemData(i, Qt::UserRole).toString()=="ccw" ) {
        info = new CCWAddr(this);
    } else if ( type->itemData(i, Qt::UserRole).toString()=="isa" ) {
        info = new IsaAddr(this);
    } else info = new _Addr(this);
    commonLayout->insertWidget(2, info, -1);
    connect(info, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    emit dataChanged();
}
