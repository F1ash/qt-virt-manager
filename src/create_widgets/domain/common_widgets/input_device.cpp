#include "input_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsInput
 */

InputDevice::InputDevice(QWidget *parent) :
    _QWidget(parent)
{
    info = new QLabel("To add additional devices explicitly", this);
    type = new QComboBox(this);
    type->addItem("EvTouch Graphics Tablet", "tablet");
    type->addItem("Generic Mouse", "mouse");
    type->addItem("Generic Keyboard", "keyboard");
    bus  = new QComboBox(this);
    bus->addItem("Default (USB)", "usb");
    bus->addItem("PS2/2", "ps2");
    bus->addItem("Paravirtualized (XEN)", "xen");
    addr = new DeviceAddress(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(info);
    commonLayout->addWidget(type);
    commonLayout->addWidget(bus);
    commonLayout->addWidget(addr);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
}
InputDevice::~InputDevice()
{
    delete info;
    info = 0;
    delete type;
    type = 0;
    delete bus;
    bus = 0;
    delete addr;
    addr = 0;
    delete commonLayout;
    commonLayout = 0;
}

/* public slots */
QDomNodeList InputDevice::getNodeList() const
{
    QDomDocument doc = QDomDocument();
    // WARNING: address implemented experimentally
    AttrList l = addr->getAttrList();
    if ( !l.isEmpty() ) {
        QDomElement _address = doc.createElement("address");
        foreach (QString key, l.keys()) {
            if ( !key.isEmpty() )
                _address.setAttribute(key, l.value(key));
        };
        doc.appendChild(_address);
    };
    return doc.childNodes();
}
QString InputDevice::getDevType() const
{
    return type->itemData(type->currentIndex(), Qt::UserRole).toString();
}
QString InputDevice::getDevBus() const
{
    return bus->itemData(bus->currentIndex(), Qt::UserRole).toString();
}
