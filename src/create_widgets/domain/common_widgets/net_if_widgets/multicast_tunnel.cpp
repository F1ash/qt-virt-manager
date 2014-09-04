#include "multicast_tunnel.h"

MultiCast_Tunnel::MultiCast_Tunnel(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    addrLabel = new QLabel("Address:", this);
    portLabel = new QLabel("Port:", this);
    macLabel = new QLabel("MAC:", this);
    addr = new QLineEdit(this);
    addr->setPlaceholderText("230.0.0.1");
    port = new QSpinBox(this);
    port->setRange(0, 65535);
    port->setValue(5558);
    mac = new QLineEdit(this);
    mac->setPlaceholderText("11:22:33:44:55:66");
    commonLayout = new QGridLayout();
    commonLayout->addWidget(addrLabel, 0, 0);
    commonLayout->addWidget(addr, 0, 1);
    commonLayout->addWidget(portLabel, 1, 0);
    commonLayout->addWidget(port, 1, 1);
    commonLayout->addWidget(macLabel, 2, 0, Qt::AlignTop);
    commonLayout->addWidget(mac, 2, 1, Qt::AlignTop);
    setLayout(commonLayout);
}

/* public slots */
QDomDocument MultiCast_Tunnel::getDevDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _mac, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    _source = doc.createElement("source");
    _source.setAttribute("address", addr->text());
    _source.setAttribute("port", port->text());
    _devDesc.appendChild(_source);
    if ( !mac->text().isEmpty() ) {
        _mac = doc.createElement("mac");
        _mac.setAttribute("address", mac->text());
        _devDesc.appendChild(_mac);
    };
    _devDesc.setAttribute("type", "mcast");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
