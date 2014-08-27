#include "smartcard_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsSmartcard
 */

SmartCardDevice::SmartCardDevice(QWidget *parent) :
    _QWidget(parent)
{
    mode = new QComboBox(this);
    mode->addItem("Host", "host");
    mode->addItem("Passthrough", "passthrough");
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(mode);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
}
SmartCardDevice::~SmartCardDevice()
{
    delete mode;
    mode = 0;
    delete commonLayout;
    commonLayout = 0;
}

/* public slots */
QDomDocument SmartCardDevice::getDevDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _address, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("smartcard");
    _address = doc.createElement("address");
    _address.setAttribute("type", "ccid");
    _address.setAttribute("controller", "0");
    _address.setAttribute("slot", "0");
    _devDesc.appendChild(_address);

    _device.appendChild(_devDesc);
    _devDesc.setAttribute("mode", mode->itemData(mode->currentIndex(), Qt::UserRole).toString());
    if ( mode->itemData(mode->currentIndex(), Qt::UserRole).toString()=="passthrough" )
        _devDesc.setAttribute("type", "spicevmc");
    doc.appendChild(_device);
    //qDebug()<<doc.toString();
    return doc;
}
