#include "network_disk.h"

#define PROTOCOLS QStringList()\
    <<"nbd"<<"iscsi"<<"rbd"<<"sheepdog"<<"gluster"

Network_Disk::Network_Disk(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _Disk(parent, connPtrPtr)
{
    protocolLabel = new QLabel("Protocol:", this);
    protocol = new QComboBox(this);
    protocol->addItems(PROTOCOLS);
    protocol->insertSeparator(protocol->count());
    protocol->insertItem(protocol->count(), "Set manually");
    sourceLabel = new QPushButton("Source:", this);
    sourceName = new QLineEdit(this);
    sourceName->setPlaceholderText("Source name or URL path");
    auth = new _Storage_Auth(this, ptr_ConnPtr);
    auth->setVisible(false);

    baseLayout->addWidget(protocolLabel, 0, 0);
    baseLayout->addWidget(protocol, 0, 1);
    baseLayout->addWidget(sourceLabel, 1, 0);
    baseLayout->addWidget(sourceName, 1, 1);
    baseLayout->addWidget(auth, 2, 0, 4, 2);

    hosts->setVisible(true);
    startupPolicy->setVisible(false);
    connect(protocol, SIGNAL(currentIndexChanged(int)),
            this, SLOT(protocolTypeChanged(int)));
    connect(protocol, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(protocolTypeChanged(QString)));
    connect(sourceLabel, SIGNAL(clicked()),
            this, SLOT(getVolumeNames()));
    // dataChanged connections
    connect(protocol, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(protocol, SIGNAL(editTextChanged(QString)),
            this, SLOT(stateChanged()));
    connect(sourceName, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(auth, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Network_Disk::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _auth, _secret, _encrypt,
            _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("disk");

    _source = doc.createElement("source");
    _source.setAttribute("protocol", protocol->currentText());
    if ( !sourceName->text().isEmpty() )
        _source.setAttribute("name", sourceName->text());

    if ( auth->auth->isChecked() ) {
        _auth = doc.createElement("auth");
        _auth.setAttribute(
                    "username", auth->userName->text());
        _secret = doc.createElement("secret");
        if ( protocol->currentText()=="iscsi" ) {
            _secret.setAttribute("type", "iscsi");
        } else
            _secret.setAttribute("type", "ceph");
        _secret.setAttribute(
                    auth->usageType->currentText().toLower(),
                    auth->usage->text());
        _auth.appendChild(_secret);
        _devDesc.appendChild(_auth);
    };
    //if ( startupPolicy->isUsed() )
    //    _source.setAttribute(
    //                "startupPolicy", startupPolicy->getStartupPolicy());
    if ( hosts->isUsed() ) {
        foreach (QString _h, hosts->getHostsList()) {
            if ( !_h.isEmpty() ) {
                QString _name, _port;
                QStringList _split;
                _split = _h.split(":");
                _name = _split.first();
                _port = _split.last();
                QDomElement _host = doc.createElement("host");
                _host.setAttribute("name", _name);
                _host.setAttribute("port", _port);
                _source.appendChild(_host);
            };
        };
    };
    _devDesc.appendChild(_source);

    _target = doc.createElement("target");
    TargetAttrs l = target->getTargetAttrs();
    foreach (QString key, l.keys()) {
        if ( !key.isEmpty() )
        _target.setAttribute(key, l.value(key));
    };
    _devDesc.appendChild(_target);

    if ( encrypt->isUsed() ) {
        _encrypt = doc.createElement("encryption");
        _encrypt.setAttribute("format", "qcow");
        _secret = doc.createElement("secret");
        _secret.setAttribute("type", "passphrase");
        _secret.setAttribute("uuid", encrypt->getSecretUUID());
        _encrypt.appendChild(_secret);
        _devDesc.appendChild(_encrypt);
    };

    if ( readOnly->state() ) {
        QDomElement _readOnly = doc.createElement("readonly");
        _devDesc.appendChild(_readOnly);
    };

    if ( addr->use->isChecked() &&
         addr->getCurrentAddrWidget()->isEnabled() ) {
        QDomElement _addr = doc.createElement("address");
        AttrList _l = addr->getAttrList();
        foreach (QString _attr, _l.keys()) {
            if ( !_attr.isEmpty() )
                _addr.setAttribute( _attr, _l.value(_attr) );
        };
        _devDesc.appendChild(_addr);
    };

    _devDesc.setAttribute("type", "network");
    _devDesc.setAttribute("device", devType->getDeviceType());
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void Network_Disk::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _host, _auth, _encrypt,
            _secret, _target, _readOnly, _addr, _driver;
    _device = doc.firstChildElement("device")
            .firstChildElement("disk");
    _source = _device.firstChildElement("source");
    _target = _device.firstChildElement("target");
    _encrypt = _device.firstChildElement("encryption");
    _readOnly = _device.firstChildElement("readonly");
    _addr = _device.firstChildElement("address");
    _driver = _device.firstChildElement("driver");
    QString _attr;
    int idx;
    _attr = _device.attribute("device");
    devType->setDeviceType(_attr);
    _attr = _source.attribute("protocol");
    idx = protocol->findText(
                _attr, Qt::MatchContains);
    if ( idx<0 ) {
        protocol->setCurrentIndex(protocol->count()-1);
        protocol->setEditText(_attr);
    } else
        protocol->setCurrentIndex(idx);
    _attr = _source.attribute("name");
    sourceName->setText(_attr);
    _auth = _device.firstChildElement("auth");
    auth->auth->setChecked( !_auth.isNull() );
    if ( !_auth.isNull() ) {
        auth->userName->setText(
                    _auth.attribute("username"));
        _secret = _auth.firstChildElement("secret");
        if ( !_secret.isNull() ) {
            QString _u;
            if ( _secret.hasAttribute("usage") ) {
                idx = auth->usageType->findText(
                            "usage", Qt::MatchContains);
                _u = _secret.attribute("usage");
            } else if ( _secret.hasAttribute("uuid") ) {
                idx = auth->usageType->findText(
                            "uuid", Qt::MatchContains);
                _u = _secret.attribute("uuid");
            };
            auth->usageType->setCurrentIndex( (idx<0)? 0:idx );
            auth->usage->setText(_u);
        };
    };
    _attr = _target.attribute("dev");
    target->devName->setText(_attr);
    _host = _source.firstChildElement("host");
    hosts->setUsage( !_host.isNull() );
    hosts->clearHostList();
    _attr.clear();
    while ( !_host.isNull() ) {
        _attr.append(_host.attribute("name"));
        _attr.append(":");
        _attr.append(_host.attribute("port"));
        hosts->setHostItem(_attr);
        _host = _host.nextSiblingElement("host");
    };
    _attr = _target.attribute("bus");
    idx = target->bus->findText(_attr, Qt::MatchContains);
    target->bus->setCurrentIndex( (idx<0)? 0:idx );
    if ( _target.hasAttribute("tray") ) {
        target->trayLabel->setChecked(true);
        _attr = _target.attribute("tray");
        idx = target->tray->findText(_attr, Qt::MatchContains);
        target->tray->setCurrentIndex( (idx<0)? 0:idx );
    } else
        target->trayLabel->setChecked(false);
    if ( _target.hasAttribute("removable") ) {
        target->removableLabel->setChecked(true);
        _attr = _target.attribute("removable");
        idx = target->removable->findText(_attr, Qt::MatchContains);
        target->removable->setCurrentIndex( (idx<0)? 0:idx );
    } else
        target->removableLabel->setChecked(false);
    encrypt->setUsage( !_encrypt.isNull() );
    if ( !_encrypt.isNull() ) {
        _secret = _encrypt.firstChildElement("secret");
        encrypt->setSecretUUID(_secret.attribute("uuid"));
    };
    readOnly->readOnly->setChecked( !_readOnly.isNull() );
    addr->use->setChecked( !_addr.isNull() );
    if ( !_addr.isNull() ) {
        _attr = _addr.attribute("type");
        idx = addr->type->findData(
                    _attr,
                    Qt::UserRole,
                    Qt::MatchContains);
        addr->type->setCurrentIndex( (idx<0)? 0:idx );
        if ( _attr=="pci" ) {
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
        } else if ( _attr=="drive" ) {
            DriveAddr *wdg = static_cast<DriveAddr*>( addr->getCurrentAddrWidget() );
            wdg->controller->setText( _addr.attribute("controller") );
            wdg->bus->setText( _addr.attribute("bus") );
            wdg->target->setText( _addr.attribute("target") );
            wdg->unit->setText( _addr.attribute("unit") );
        };
    };
}

/* private slots */
void Network_Disk::protocolTypeChanged(int i)
{
    protocol->setEditable( i==protocol->count()-1 );
    protocol->clearEditText();
}
void Network_Disk::protocolTypeChanged(QString _type)
{
    if ( _type.toLower()=="rbd" ) {
        auth->setSecretType("CEPH");
        auth->setVisible(true);
    } else if ( _type.toLower()=="iscsi" ) {
        auth->setSecretType("ISCSI");
        auth->setVisible(true);
    } else {
        auth->setSecretType(NOT_VOLUME);
        auth->setVisible(false);
    };
    auth->usage->clear();
    auth->auth->setChecked(false);
    sourceName->clear();
}
void Network_Disk::getVolumeNames()
{
    VVD_Result _ret;
    QString _type = protocol->currentText().toLower();
    VirtVolumeDialog *volumeDialog = new VirtVolumeDialog(
                    this, ptr_ConnPtr, _type);
    if ( volumeDialog->exec()==QDialog::Accepted ) {
        _ret = volumeDialog->getResult();
        if ( _ret.type=="iscsi" ) {
            auth->setVisible(true);
            auth->setSecretType("ISCSI");
        }else if ( _ret.type=="rnd" ) {
            auth->setVisible(true);
            auth->setSecretType("CEPH");
        } else {
            auth->auth->setChecked(false);
            auth->setVisible(false);
            auth->setSecretType(NOT_VOLUME);
        };
        setTypedData(_ret);
    };
    delete volumeDialog;
    volumeDialog = nullptr;
}
void Network_Disk::setTypedData(VVD_Result &_data)
{
    QString _name;
    QDomDocument doc;
    doc.setContent(_data.source);
    QDomElement _source, _host, _auth, _secret, _device;
    _source = doc.firstChildElement("source");
    if ( !_source.isNull() ) {
        _host = _source.firstChildElement("host");
        if ( !_host.isNull() ) {
            hosts->setUsage(true);
            QString _name, _port, _addr;
            _name = _host.attribute("name");
            _port = _host.attribute("port");
            _addr = QString("%1:%2").arg(_name).arg(_port);
            hosts->setHostItem(_addr);
        };
        _device = _source.firstChildElement("device");
        if        ( _data.type=="iscsi" ) {
            _auth = _source.firstChildElement("auth");
            if ( !_device.isNull() ) {
                _name.append(
                            _device.attribute("path"));
                _name.append("/");
                QString _lun = _data.path.split("-").last();
                _name.append(_lun);
            };
            if ( !_auth.isNull() ) {
                auth->auth->setChecked(true);
                auth->userName->setText(
                            _auth.attribute("username"));
                _secret = _auth.firstChildElement("secret");
                if ( !_secret.isNull() ) {
                    QString _u;
                    int idx;
                    if ( _secret.hasAttribute("usage") ) {
                        idx = auth->usageType->findText(
                                    "usage", Qt::MatchContains);
                        _u = _secret.attribute("usage");
                    } else if ( _secret.hasAttribute("uuid") ) {
                        idx = auth->usageType->findText(
                                    "uuid", Qt::MatchContains);
                        _u = _secret.attribute("uuid");
                    };
                    auth->usageType->setCurrentIndex( (idx<0)? 0:idx );
                    auth->usage->setText(_u);
                    auth->setSecretType("ISCSI");
                };
            };
        } else if ( _data.type=="rnd" ) {
            if ( !_auth.isNull() ) {
                auth->auth->setChecked(true);
                auth->userName->setText(
                            _auth.attribute("username"));
                _secret = _auth.firstChildElement("secret");
                if ( !_secret.isNull() ) {
                    QString _u;
                    int idx;
                    if ( _secret.hasAttribute("usage") ) {
                        idx = auth->usageType->findText(
                                    "usage", Qt::MatchContains);
                        _u = _secret.attribute("usage");
                    } else if ( _secret.hasAttribute("uuid") ) {
                        idx = auth->usageType->findText(
                                    "uuid", Qt::MatchContains);
                        _u = _secret.attribute("uuid");
                    };
                    auth->usageType->setCurrentIndex( (idx<0)? 0:idx );
                    auth->usage->setText(_u);
                    auth->setSecretType("CEPH");
                };
            };
        } else if ( _data.type=="nbd" ) {
        } else if ( _data.type=="sheepdog" ) {
        } else if ( _data.type=="gluster" ) {
        } else {
            _name.append(_data.name);
        };
        sourceName->setText(_name);
    };
}
