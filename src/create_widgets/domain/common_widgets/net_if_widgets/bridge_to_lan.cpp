#include "bridge_to_lan.h"

bridge_HlpThread::bridge_HlpThread(
        QObject *parent,
        virConnectPtr* connPtrPtr) :
    qwdHelpThread(parent, connPtrPtr)
{
    qRegisterMetaType<QStringList>("QStringList&");
}
void bridge_HlpThread::run()
{
    if ( nullptr==ptr_ConnPtr || nullptr==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };
    connType = QString::fromUtf8(
                virConnectGetType(*ptr_ConnPtr))
            .toLower();

    virNWFilterPtr *filters = nullptr;
    //extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    int ret = virConnectListAllNWFilters(
                *ptr_ConnPtr, &filters, flags);
    if ( ret<0 ) {
        sendConnErrors();
    } else {
        // therefore correctly to use for() command,
        // because filters[0] can not exist.
        for (int i = 0; i < ret; i++) {
            nwFilters.append( virNWFilterGetName(filters[i]) );
            virNWFilterFree(filters[i]);
        };
        if (filters) free(filters);
    };
    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
}

Bridge_to_LAN::Bridge_to_LAN(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _QWidget_Threaded(parent, connPtrPtr)
{
    bridgeLabel = new QLabel("Bridge:", this);
    bridge = new QLineEdit(this);
    bridge->setPlaceholderText("br0");
    targetLabel = new QLabel("Target Device name:", this);
    target = new QLineEdit(this);
    target->setPlaceholderText("vnet7");
    baseLayout = new QGridLayout();
    baseLayout->addWidget(bridgeLabel, 0, 0);
    baseLayout->addWidget(bridge, 0, 1);
    baseLayout->addWidget(targetLabel, 1, 0);
    baseLayout->addWidget(target, 1, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    mac = new MAC_Address(this);
    virtPort = new VirtualPort(this);
    addr = new DeviceAddress(this);
    int idx = addr->type->findData(
                "pci",
                Qt::UserRole,
                Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    nwFilterParams = new NWFilter_Params(
                this,
                "Network Filter on Interface");
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(mac);
    commonLayout->addWidget(virtPort);
    commonLayout->addWidget(addr);
    commonLayout->addWidget(nwFilterParams);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    virtPort->type->setCurrentIndex(
                virtPort->type->findText("Open vSwitch") );
    hlpThread = new bridge_HlpThread(this, connPtrPtr);
    connect(hlpThread, SIGNAL(errorMsg(const QString&, const uint)),
            this, SIGNAL(errorMsg(const QString&)));
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(emitCompleteSignal()));
    hlpThread->start();
    // dataChanged connections
    connect(bridge, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(target, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(mac, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(virtPort, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(nwFilterParams, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Bridge_to_LAN::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source,_target, _mac, _virtualport,
            _parameters, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    _source = doc.createElement("source");
    _source.setAttribute("bridge", bridge->text());
    _devDesc.appendChild(_source);
    if ( !target->text().isEmpty() ) {
        _target = doc.createElement("target");
        _target.setAttribute("dev", target->text());
        _devDesc.appendChild(_target);
    };

    if ( mac->isUsed() ) {
        _mac = doc.createElement("mac");
        _mac.setAttribute("address", mac->getMACAddress());
        _devDesc.appendChild(_mac);
    };

    VirtPortParamList p = virtPort->getParameterList();
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
    if ( nwFilterParams->isUsed() ) {
        _devDesc.appendChild(
                    nwFilterParams->getDataDocument());
    };
    _devDesc.setAttribute("type", "bridge");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void Bridge_to_LAN::setDataDescription(const QString &xmlDesc)
{
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _target, _mac,
            _virtport, _addr, _filterref;
    _device = doc.firstChildElement("device")
            .firstChildElement("interface");
    _source = _device.firstChildElement("source");
    _target = _device.firstChildElement("target");
    _mac = _device.firstChildElement("mac");
    _virtport = _device.firstChildElement("virtualport");
    QString _attr;
    _attr = _source.attribute("bridge");
    bridge->setText(_attr);
    _attr = _target.attribute("dev");
    target->setText(_attr);
    mac->setUsage(!_mac.isNull());
    if ( !_mac.isNull() ) {
        _attr = _mac.attribute("address");
        mac->setMACAddress(_attr);
    };
    virtPort->setUsage( !_virtport.isNull() );
    if ( !_virtport.isNull() ) {
        VirtPortParamList _list;
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
        int idx = addr->type->findData(
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
    _filterref = _device.firstChildElement("filterref");
    if ( !_filterref.isNull() ) {
        QDomDocument _doc;
        _doc.setContent(QString());
        _doc.appendChild(_filterref.cloneNode());
        QString _xml = _doc.toString();
        nwFilterParams->setDataDescription(_xml);
    };
}

/* private slots */
void Bridge_to_LAN::emitCompleteSignal()
{
    bool state = ( hlpThread->connType=="qemu" );
    nwFilterParams->setVisible(state);
    if ( state ) {
        nwFilterParams->setNWFiltersList(
                    hlpThread->nwFilters);
    };
    if ( sender()==hlpThread ) {
        setEnabled(true);
        emit complete();
    };
}
