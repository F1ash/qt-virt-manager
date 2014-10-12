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
    bus->addItem("PS/2", "ps2");
    bus->addItem("Paravirtualized (XEN)", "xen");
    addr = new DeviceAddress(this);
    int idx = addr->type->findText("pci", Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    addr->setCurrentAddrWidget(idx);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(info);
    commonLayout->addWidget(type);
    commonLayout->addWidget(bus);
    commonLayout->addWidget(addr);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
}

/* public slots */
QDomDocument InputDevice::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _address, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("input");
    // WARNING: address implemented experimentally
    AttrList l = addr->getAttrList();
    if ( !l.isEmpty() ) {
        _address = doc.createElement("address");
        foreach (QString key, l.keys()) {
            if ( !key.isEmpty() )
                _address.setAttribute(key, l.value(key));
        };
        _devDesc.appendChild(_address);
    };
    _device.appendChild(_devDesc);
    _devDesc.setAttribute("type", type->itemData(type->currentIndex(), Qt::UserRole).toString());
    _devDesc.setAttribute("bus", bus->itemData(bus->currentIndex(), Qt::UserRole).toString());
    doc.appendChild(_device);
    return doc;
}
