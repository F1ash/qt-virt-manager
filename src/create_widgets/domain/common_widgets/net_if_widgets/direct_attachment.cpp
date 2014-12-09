#include "direct_attachment.h"

DirectAttachment::DirectAttachment(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    netSourceLabel = new QLabel("Network source:", this);
    sourceModeLabel = new QLabel("Source mode:", this);
    netSource = new QComboBox(this);
    sourceMode = new QComboBox(this);
    sourceMode->addItem("VEPA");
    sourceMode->addItem("Bridge");
    sourceMode->addItem("Private");
    sourceMode->addItem("PassThrough");
    baseLayout = new QGridLayout();
    baseLayout->addWidget(netSourceLabel, 0, 0);
    baseLayout->addWidget(netSource, 0, 1);
    baseLayout->addWidget(sourceModeLabel, 1, 0);
    baseLayout->addWidget(sourceMode, 1, 1);
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
    setAvailableSources();
    virtPort->type->setCurrentIndex( virtPort->type->findText("802.1Qbh") );
    // dataChanged connections
    connect(netSource, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(sourceMode, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(virtPort, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
}

/* public slots */
QDomDocument DirectAttachment::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _virtualport,
            _parameters, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    _source = doc.createElement("source");
    QString _dev = netSource->itemData(netSource->currentIndex(), Qt::UserRole).toString();
    if ( !_dev.isEmpty() ) _source.setAttribute("dev", _dev);
    _source.setAttribute("mode", sourceMode->currentText().toLower());
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
    AttrList _l = addr->getAttrList();
    if ( !_l.isEmpty() ) {
        QDomElement _address = doc.createElement("address");
        foreach (QString key, _l.keys()) {
            if ( !key.isEmpty() )
            _address.setAttribute(key, _l.value(key));
        };
        _devDesc.appendChild(_address);
    };

    _devDesc.setAttribute("type", "direct");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void DirectAttachment::setDataDescription(QString &xmlDesc)
{
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _virtport, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("interface");
    _source = _device.firstChildElement("source");
    _virtport = _device.firstChildElement("virtualport");
    QString _attr;
    _attr = _source.attribute("dev");
    int idx = netSource->findData(
                _attr,
                Qt::UserRole,
                Qt::MatchContains);
    idx = (idx<0)? netSource->count()-1:idx;
    netSource->setCurrentIndex( idx );
    if ( idx==netSource->count()-1 ) {
        netSource->setEditText(_attr);
    };
    _attr = _source.attribute("mode");
    idx = sourceMode->findText(_attr, Qt::MatchContains);
    sourceMode->setCurrentIndex( (idx<0)? 0:idx );
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
    };
}

/* private slots */
void DirectAttachment::setAvailableSources()
{
    virNodeDevice  **nodeDevices = NULL;
    if ( currWorkConnect!=NULL ) {
        unsigned int flags =
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_NET;
        int ret = virConnectListAllNodeDevices(currWorkConnect, &nodeDevices, flags);
        if ( ret<0 ) {
            netSource->insertItem(0, "NetSource detect failed", "");
        } else {
            int i = 0;
            while ( nodeDevices[i] != NULL ) {
                QDomDocument doc;
                QString _dev, _interface;
                // flags: extra flags; not used yet,
                // so callers should always pass 0
                _dev = QString(virNodeDeviceGetXMLDesc(nodeDevices[i], 0));
                //qDebug()<<_dev;
                doc.setContent(_dev);
                _interface = doc.
                        firstChildElement("device").
                        firstChildElement("capability").
                        firstChildElement("interface").
                        firstChild().toText().data();
                if ( _interface!="lo" )
                    netSource->insertItem(
                            0,
                            QString("Host Device '%1' : macvtap")
                            .arg(_interface),
                            _interface);
                virNodeDeviceFree(nodeDevices[i]);
                i++;
            };
        };
        free(nodeDevices);
    };
}
