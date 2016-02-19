#include "pci_passthrough.h"

PCI_Passthrough::PCI_Passthrough(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _QWidget(parent, connPtrPtr)
{
    attentionIcon = new QLabel(this);
    attentionIcon->setPixmap(QIcon::fromTheme("dialog-warning")
                             .pixmap(this->fontInfo().pixelSize()));
    attention = new QLabel("WARNING: only for SR-IOV\n(Single Root I/O Virtualization)", this);
    driverLabel = new QLabel("Driver:", this);
    driver = new QComboBox(this);
    QString connType;
    if ( nullptr!=ptr_ConnPtr && nullptr!=*ptr_ConnPtr ) {
        connType = QString(virConnectGetType(*ptr_ConnPtr)).toLower();
    } else
        emit ptrIsNull();
    if ( connType=="lxc" ) {
        driver->addItem("VFIO");
    } else if ( connType=="qemu" ) {
        driver->addItem("VFIO");
        driver->addItem("KVM");
    };
    macLabel = new QLabel("MAC:", this);
    mac = new QLineEdit(this);
    mac->setPlaceholderText("11:22:33:44:55:66");
    pciAddrLabel = new QLabel("PCI Device Address:", this);
    pciAddr = new PciAddr(this);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(attentionIcon, 0, 0);
    baseLayout->addWidget(attention, 0, 1);
    baseLayout->addWidget(driverLabel, 1, 0);
    baseLayout->addWidget(driver, 1, 1);
    baseLayout->addWidget(macLabel, 2, 0);
    baseLayout->addWidget(mac, 2, 1);
    baseLayout->addWidget(pciAddrLabel, 3, 0);
    baseLayout->addWidget(pciAddr, 4, 0, 5, 2, Qt::AlignTop);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    virtPort = new VirtualPort(this);
    addr = new DeviceAddress(this);
    int idx = addr->type->findData(
                "pci",
                Qt::UserRole,
                Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(virtPort);
    commonLayout->addWidget(addr);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    virtPort->type->setCurrentIndex( virtPort->type->findText("802.1Qbh") );
    // dataChanged connections
    connect(driver, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(mac, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(pciAddr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(virtPort, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument PCI_Passthrough::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _address, _mac, _driver,
            _virtualport, _parameters, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    _driver = doc.createElement("driver");
    _driver.setAttribute("name", driver->currentText().toLower());
    _devDesc.appendChild(_driver);
    _source = doc.createElement("source");

    AttrList l = pciAddr->getAttrList();
    _address = doc.createElement("address");
    foreach (QString key, l.keys()) {
        if ( !key.isEmpty() )
        _address.setAttribute(key, l.value(key));
    };
    _address.setAttribute("type", "pci");
    _source.appendChild(_address);

    _devDesc.appendChild(_source);

    ParameterList p = virtPort->getParameterList();
    if ( !p.isEmpty() ) {
        _virtualport = doc.createElement("virtualport");
        _parameters = doc.createElement("parameters");
        foreach (QString key, p.keys()) {
            if ( !key.isEmpty() ) {
                if ( key=="type" ) {
                    _virtualport.setAttribute(key, p.value(key));
                } else
                    _parameters.setAttribute(key, p.value(key));
            };
            _virtualport.appendChild(_parameters);
        };
        _devDesc.appendChild(_virtualport);
    };

    if ( !mac->text().isEmpty() ) {
        _mac = doc.createElement("mac");
        _mac.setAttribute("address", mac->text());
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
    _devDesc.setAttribute("type", "hostdev");
    _devDesc.setAttribute("managed", "yes");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void PCI_Passthrough::setDataDescription(QString &xmlDesc)
{
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _driver, _address,
            _mac, _virtport, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("interface");
    _source = _device.firstChildElement("source");
    _driver = _device.firstChildElement("driver");
    _mac = _device.firstChildElement("mac");
    _virtport = _device.firstChildElement("virtualport");
    QString _attr;
    _attr = _driver.attribute("name");
    int idx = driver->findText(_attr, Qt::MatchContains);
    driver->setCurrentIndex( (idx<0)? 0:idx );
    if ( !_mac.isNull() ) mac->setText(_mac.attribute("address"));
    _address = _source.firstChildElement("address");
    if ( !_address.isNull() ) {
        pciAddr->domain->setText( _address.attribute("domain") );
        pciAddr->bus->setText( _address.attribute("bus") );
        pciAddr->slot->setText( _address.attribute("slot") );
        pciAddr->function->setValue( _address.attribute("function")
                                     .split("x").last().toInt() );
    };
    virtPort->setUsage( !_virtport.isNull() );
    if ( !_virtport.isNull() ) {
        ParameterList _list;
        _list.insert("type", _virtport.attribute("type"));
        QDomElement _params = _virtport.firstChildElement("parameters");
        if ( _params.hasAttribute("managerid") ) {
            _list.insert("managerid", _params.attribute("managerid"));
        };
        if ( _params.hasAttribute("typeid") ) {
            _list.insert("typeid", _params.attribute("typeid"));
        };
        if ( _params.hasAttribute("typeidver") ) {
            _list.insert("typeidver", _params.attribute("typeidver"));
        };
        if ( _params.hasAttribute("instanceId") ) {
            _list.insert("instanceid", _params.attribute("instanceid"));
        };
        if ( _params.hasAttribute("interfaceid") ) {
            _list.insert("interfaceid", _params.attribute("interfaceid"));
        };
        if ( _params.hasAttribute("profileid") ) {
            _list.insert("profileid", _params.attribute("profileid"));
        };
        virtPort->setParameterList(_list);
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
        wdg->domain->setText( _addr.attribute("domain") );
        wdg->bus->setText( _addr.attribute("bus") );
        wdg->slot->setText( _addr.attribute("slot") );
        wdg->function->setValue( _addr.attribute("function")
                                 .split("x").last().toInt() );
        if ( _addr.hasAttribute("multifunction") ) {
            wdg->multifunction->setEnabled(true);
            wdg->multifunction->setChecked(
                        _addr.attribute("multifunction")=="on" );
        };
    };
}
