#include "multicast_tunnel.h"

MultiCast_Tunnel::MultiCast_Tunnel(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    addrLabel = new QLabel("Address:", this);
    portLabel = new QLabel("Port:", this);
    addr = new QLineEdit(this);
    addr->setPlaceholderText("230.0.0.1");
    port = new QSpinBox(this);
    port->setRange(0, 65535);
    port->setValue(5558);
    mac = new MAC_Address(this);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(addrLabel, 0, 0);
    commonLayout->addWidget(addr, 0, 1);
    commonLayout->addWidget(portLabel, 1, 0);
    commonLayout->addWidget(port, 1, 1);
    commonLayout->addWidget(mac, 2, 0, 3, 2, Qt::AlignTop);
    setLayout(commonLayout);
    // dataChanged connects
    connect(addr, SIGNAL(textEdited(QString)),
            this, SIGNAL(dataChanged()));
    connect(port, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(mac, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
}

/* public slots */
QDomDocument MultiCast_Tunnel::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _mac, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    _source = doc.createElement("source");
    _source.setAttribute("address", addr->text());
    _source.setAttribute("port", port->text());
    _devDesc.appendChild(_source);
    if ( mac->isUsed() ) {
        _mac = doc.createElement("mac");
        _mac.setAttribute("address", mac->getMACAddress());
        _devDesc.appendChild(_mac);
    };
    _devDesc.setAttribute("type", "mcast");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void MultiCast_Tunnel::setDataDescription(QString &xmlDesc)
{
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _mac;
    _device = doc.firstChildElement("device")
            .firstChildElement("interface");
    _source = _device.firstChildElement("source");
    _mac = _device.firstChildElement("mac");
    QString _attr;
    _attr = _source.attribute("address");
    addr->setText(_attr);
    _attr = _source.attribute("port");
    port->setValue(_attr.toInt());
    mac->setUsage(!_mac.isNull());
    if ( !_mac.isNull() ) {
        _attr = _mac.attribute("address");
        mac->setMACAddress(_attr);
    };
}
