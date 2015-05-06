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
    info = new QStackedWidget(this);
    info->setVisible(false);
    info->addWidget(new PciAddr(this));
    info->addWidget(new DriveAddr(this));
    info->addWidget(new VirtioSerialAddr(this));
    info->addWidget(new CCIDAddr(this));
    info->addWidget(new USBAddr(this));
    info->addWidget(new SpaprVioAddr(this));
    info->addWidget(new CCWAddr(this));
    info->addWidget(new IsaAddr(this));
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(use);
    commonLayout->addWidget(type);
    commonLayout->addWidget(info);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(use, SIGNAL(toggled(bool)),
            this, SLOT(addressUsed(bool)));
    connect(type, SIGNAL(currentIndexChanged(int)),
            info, SLOT(setCurrentIndex(int)));
    // dataChanged signals
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    for (int i=0; i<info->count(); i++) {
        connect(info->widget(i), SIGNAL(dataChanged()),
                this, SLOT(stateChanged()));
    };
}

/* public slots */
AttrList DeviceAddress::getAttrList() const
{
    AttrList _ret;
    if ( use->isChecked() && info!=NULL ) {
        // get attribute list from current address type
        _Addr *wdg = static_cast<_Addr*>(info->currentWidget());
        _ret = wdg->getAttrList();
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
QWidget *DeviceAddress::getCurrentAddrWidget()
{
    return info->currentWidget();
}
void DeviceAddress::setCurrentAddrWidget(int i)
{
    info->setCurrentIndex(i);
}

/* private slots */
void DeviceAddress::addressUsed(bool state)
{
    type->setVisible(state);
    if ( info!=NULL ) info->setVisible(state);
    emit dataChanged();
}
