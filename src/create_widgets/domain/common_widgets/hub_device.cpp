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
}

/* public slots */
QDomNodeList HubDevice::getNodeList() const
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
QString HubDevice::getDevType() const
{
    /*
     * The hub element has one mandatory attribute,
     * the type whose value can only be 'usb'.
     */
    return QString("usb");
}
