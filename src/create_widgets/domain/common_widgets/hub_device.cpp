#include "hub_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsHub
 */

HubDevice::HubDevice(QWidget *parent) :
    _QWidget(parent)
{
    info = new QLabel("To add USB Hub", this);
    addr = new DeviceAddress(this);
    addr->type->clear();
    addr->type->addItem("Default", "");
    addr->type->addItem("USB addresses", "usb");
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(info);
    commonLayout->addWidget(addr);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument HubDevice::getDevDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _address, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("hub");
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
    /*
     * The hub element has one mandatory attribute,
     * the type whose value can only be 'usb'.
     */
    _devDesc.setAttribute("type", "usb");
    doc.appendChild(_device);
    return doc;
}
