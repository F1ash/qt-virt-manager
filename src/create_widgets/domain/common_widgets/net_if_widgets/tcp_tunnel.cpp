#include "tcp_tunnel.h"

TCP_Tunnel::TCP_Tunnel(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    typeLabel =new QLabel("Type:", this);
    addrLabel = new QLabel("Address:", this);
    portLabel = new QLabel("Port:", this);
    type = new QComboBox(this);
    type->addItems(QStringList()<<"server"<<"client");
    addr = new QLineEdit(this);
    addr->setPlaceholderText("192.168.0.1");
    port = new QSpinBox(this);
    port->setRange(0, 65535);
    port->setValue(5558);
    mac = new MAC_Address(this);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(typeLabel, 0, 0);
    commonLayout->addWidget(type, 0, 1);
    commonLayout->addWidget(addrLabel, 1, 0);
    commonLayout->addWidget(addr, 1, 1);
    commonLayout->addWidget(portLabel, 2, 0);
    commonLayout->addWidget(port, 2, 1);
    commonLayout->addWidget(mac, 3, 0, 4, 2, Qt::AlignTop);
    setLayout(commonLayout);
    // dataChanged connects
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(addr, SIGNAL(textEdited(QString)),
            this, SIGNAL(dataChanged()));
    connect(port, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(mac, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
}

/* public slots */
QDomDocument TCP_Tunnel::getDataDocument() const
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
    _devDesc.setAttribute("type", type->currentText());
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void TCP_Tunnel::setDataDescription(QString &xmlDesc)
{
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _mac;
    _device = doc.firstChildElement("device")
            .firstChildElement("interface");
    _source = _device.firstChildElement("source");
    _mac = _device.firstChildElement("mac");
    QString _attr;
    _attr = _device.attribute("type");
    int idx = type->findText(_attr, Qt::MatchContains);
    type->setCurrentIndex( (idx<0)? 0:idx );
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
