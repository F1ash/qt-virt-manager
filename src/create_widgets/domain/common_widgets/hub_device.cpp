#include "hub_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsHub
 */

HubDevice::HubDevice(QWidget *parent) :
    _QWidget(parent)
{
    info = new QLabel("To add USB Hub", this);
    addr = new DeviceAddress(this);
    int idx = addr->type->findData(
                "usb",
                Qt::UserRole,
                Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    addr->setCurrentAddrWidget(idx);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(info);
    commonLayout->addWidget(addr);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
}

/* public slots */
QDomDocument HubDevice::getDataDocument() const
{
    QDomDocument doc;
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
