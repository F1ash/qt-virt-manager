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
QDomNodeList SmartCardDevice::getNodeList() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _address;
    _address = doc.createElement("address");
    _address.setAttribute("type", "ccid");
    _address.setAttribute("controller", "0");
    _address.setAttribute("slot", "0");
    doc.appendChild(_address);

    //qDebug()<<doc.toString();
    return doc.childNodes();
}
QString SmartCardDevice::getDevType() const
{
    QString _ret;
    if ( mode->itemData(mode->currentIndex(), Qt::UserRole).toString()=="passthrough" )
        _ret = QString("spicevmc");
    return _ret;
}
QString SmartCardDevice::getDevMode() const
{
    return mode->itemData(mode->currentIndex(), Qt::UserRole).toString();
}
