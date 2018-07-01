#include "direct_attachment.h"

dirAttach_HlpThread::dirAttach_HlpThread(
        QObject         *parent,
        virConnectPtr   *connPtrPtr) :
    qwdHelpThread(parent, connPtrPtr)
{
    qRegisterMetaType<QStringList>("QStringList&");
}
void dirAttach_HlpThread::run()
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
    QStringList       devices;
    virNodeDevicePtr *nodeDevices;
    unsigned int flags =
            VIR_CONNECT_LIST_NODE_DEVICES_CAP_NET;
    int ret = virConnectListAllNodeDevices(
                *ptr_ConnPtr, &nodeDevices, flags);
    if ( ret<0 ) {
        sendConnErrors();
    } else {
        // therefore correctly to use for() command,
        // because networks[0] can not exist.
        int i = 0;
        while ( nodeDevices[i] != nullptr ) {
            QDomDocument doc;
            QString _dev, _interface;
            // flags: extra flags; not used yet,
            // so callers should always pass 0
            _dev = QString(virNodeDeviceGetXMLDesc(
                               nodeDevices[i], 0));
            //qDebug()<<_dev;
            doc.setContent(_dev);
            _interface = doc.
                    firstChildElement("device").
                    firstChildElement("capability").
                    firstChildElement("interface").
                    firstChild().toText().data();
            if ( _interface.compare("lo")!=0 )
                devices.append(_interface);
            virNodeDeviceFree(nodeDevices[i]);
            i++;
        };
        if (nodeDevices) free(nodeDevices);
    };

    virNWFilterPtr *filters = nullptr;
    //extra flags; not used yet, so callers should always pass 0
    flags = 0;
    ret = virConnectListAllNWFilters(
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
    //int devs = virNodeNumOfDevices(ptr_ConnPtr, nullptr, 0);
    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
    emit result(devices);
}

DirectAttachment::DirectAttachment(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    _QWidget_Threaded(parent, connPtrPtr)
{
    netSourceLabel = new QLabel(tr("Network source:"), this);
    sourceModeLabel = new QLabel(tr("Source mode:"), this);
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
    nwFilterParams = new NWFilter_Params(
                this,
                tr("Network Filter on Interface"));
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(virtPort);
    commonLayout->addWidget(addr);
    commonLayout->addWidget(nwFilterParams);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    hlpThread = new dirAttach_HlpThread(this, connPtrPtr);
    connect(hlpThread, SIGNAL(result(QStringList&)),
            this, SLOT(setAvailableSources(QStringList&)));
    connect(hlpThread, SIGNAL(errorMsg(const QString&, const uint)),
            this, SIGNAL(errorMsg(const QString&)));
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(emitCompleteSignal()));
    hlpThread->start();
    // dataChanged connections
    connect(netSource, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(sourceMode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(virtPort, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(sourceMode, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sourceModeChanged(QString)));
    connect(nwFilterParams, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
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
    QString _dev = netSource->itemData(
                netSource->currentIndex(), Qt::UserRole).toString();
    if ( !_dev.isEmpty() ) _source.setAttribute("dev", _dev);
    _source.setAttribute("mode", sourceMode->currentText().toLower());
    _devDesc.appendChild(_source);

    VirtPortParamList p = virtPort->getParameterList();
    if ( !p.isEmpty() ) {
        _virtualport = doc.createElement("virtualport");
        _parameters = doc.createElement("parameters");
        foreach (QString key, p.keys()) {
            if ( !key.isEmpty() ) {
                if ( key.compare("type")==0 ) {
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

    _devDesc.setAttribute("type", "direct");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void DirectAttachment::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device, _source, _virtport, _addr, _filterref;
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
        idx = addr->type->findData(
                    "pci",
                    Qt::UserRole,
                    Qt::MatchContains);
        addr->type->setCurrentIndex( (idx<0)? 0:idx );
        addr->type->setEnabled(false);
        PciAddr *wdg = static_cast<PciAddr*>(addr->getCurrentAddrWidget());
        if ( wdg!=nullptr ) {
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
void DirectAttachment::sourceModeChanged(QString _mode)
{
    bool state = ( hlpThread->connType.compare("qemu")==0
                   && _mode.compare("Bridge")==0);
    nwFilterParams->setUsage(false);
    nwFilterParams->setVisible(state);
}
void DirectAttachment::setAvailableSources(QStringList &_devs)
{
    foreach (QString _interface, _devs) {
        netSource->insertItem(
                0,
                QString(tr("Host Device '%1' : macvtap"))
                .arg(_interface),
                _interface);
    };
    virtPort->type->setCurrentIndex(
                virtPort->type->findData(
                    "802.1Qbh", Qt::UserRole, Qt::MatchExactly));
}
void DirectAttachment::emitCompleteSignal()
{
    sourceModeChanged(sourceMode->currentText());
    if ( hlpThread->connType.compare("qemu")==0 ) {
        nwFilterParams->setNWFiltersList(
                    hlpThread->nwFilters);
    };
    if ( sender()==hlpThread ) {
        setEnabled(true);
        emit complete();
    };
}
