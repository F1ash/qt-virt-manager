#include "tcp_tunnel.h"

TCP_Tunnel::TCP_Tunnel(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    _QWidget(parent, connPtrPtr)
{
    typeLabel =new QLabel(tr("Type:"), this);
    addrLabel = new QLabel(tr("Address:"), this);
    portLabel = new QLabel(tr("Port:"), this);
    type = new QComboBox(this);
    type->addItems(QStringList()<<"server"<<"client");
    ipAddr = new QLineEdit(this);
    ipAddr->setPlaceholderText("192.168.0.1");
    port = new QSpinBox(this);
    port->setRange(0, 65535);
    port->setValue(5558);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(typeLabel, 0, 0);
    baseLayout->addWidget(type, 0, 1);
    baseLayout->addWidget(addrLabel, 1, 0);
    baseLayout->addWidget(ipAddr, 1, 1);
    baseLayout->addWidget(portLabel, 2, 0);
    baseLayout->addWidget(port, 2, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    mac = new MAC_Address(this);
    addr = new DeviceAddress(this);
    int idx = addr->type->findData(
                "pci",
                Qt::UserRole,
                Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(mac);
    commonLayout->addWidget(addr);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    // dataChanged connections
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(ipAddr, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(port, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(mac, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument TCP_Tunnel::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _mac, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    _source = doc.createElement("source");
    _source.setAttribute("address", ipAddr->text());
    _source.setAttribute("port", port->text());
    _devDesc.appendChild(_source);
    if ( mac->isUsed() ) {
        _mac = doc.createElement("mac");
        _mac.setAttribute("address", mac->getMACAddress());
        _devDesc.appendChild(_mac);
    };
    AttrList _l = addr->getAttrList();
    if ( !_l.isEmpty() ) {
        QDomElement _address = doc.createElement("address");
        foreach (QString key, _l.keys()) {
            if ( !key.isEmpty() )
            _address.setAttribute(key, _l.value(key));
        };
        _devDesc.appendChild(_address);
    };
    _devDesc.setAttribute("type", type->currentText());
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void TCP_Tunnel::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device, _source, _mac, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("interface");
    _source = _device.firstChildElement("source");
    _mac = _device.firstChildElement("mac");
    QString _attr;
    _attr = _device.attribute("type");
    int idx = type->findText(_attr, Qt::MatchContains);
    type->setCurrentIndex( (idx<0)? 0:idx );
    _attr = _source.attribute("address");
    ipAddr->setText(_attr);
    _attr = _source.attribute("port");
    port->setValue(_attr.toInt());
    mac->setUsage(!_mac.isNull());
    if ( !_mac.isNull() ) {
        _attr = _mac.attribute("address");
        mac->setMACAddress(_attr);
    };
    _addr = _device.firstChildElement("address");
    addr->use->setChecked( !_addr.isNull() );
    if ( !_addr.isNull() ) {
        idx = addr->type->findData(
                    "pci",
                    Qt::UserRole,
                    Qt::MatchContains);
        addr->type->setCurrentIndex( (idx<0)? 0:idx );
        addr->type->setEnabled(false);
        PciAddr *wdg = static_cast<PciAddr*>(addr->getCurrentAddrWidget());
        if ( wdg!=Q_NULLPTR ) {
            wdg->domain->setText( _addr.attribute("domain") );
            wdg->bus->setText( _addr.attribute("bus") );
            wdg->slot->setText( _addr.attribute("slot") );
            wdg->function->setValue( _addr.attribute("function")
                                     .split("x").last().toInt() );
            if ( _addr.hasAttribute("multifunction") ) {
                wdg->multifunction->setEnabled(true);
                wdg->multifunction->setChecked(
                            _addr.attribute("multifunction").compare("on")==0 );
            };
        };
    };
}
