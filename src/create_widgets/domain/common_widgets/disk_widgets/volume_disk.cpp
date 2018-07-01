#include "volume_disk.h"

Volume_Disk::Volume_Disk(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    _Disk(parent, connPtrPtr)
{
    poolLabel = new QLabel(tr("Pool:"), this);
    pool = new QLabel(this);
    volumeLabel = new QPushButton(tr("Volume:"), this);
    volume = new QLineEdit(this);
    volume->setReadOnly(true);
    modeLabel = new QLabel(tr("Source Mode:"), this);
    modeLabel->setEnabled(false);
    mode = new QComboBox(this);
    mode->addItems(QStringList()<<"Host"<<"Direct");
    mode->setEnabled(false);
    auth = new _Storage_Auth(this, ptr_ConnPtr);
    auth->setVisible(false);

    secLabels->setVisible(true);
    encrypt->setVisible(true);

    baseLayout->addWidget(poolLabel, 0, 0);
    baseLayout->addWidget(pool, 0, 1);
    baseLayout->addWidget(volumeLabel, 1, 0);
    baseLayout->addWidget(volume, 1, 1);
    baseLayout->addWidget(modeLabel, 2, 0);
    baseLayout->addWidget(mode, 2, 1);
    baseLayout->addWidget(auth, 3, 1);

    connect(volumeLabel, SIGNAL(clicked()),
            this, SLOT(getVolumeNames()));
    // dataChanged connections
    connect(volume, SIGNAL(textChanged(QString)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Volume_Disk::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _auth, _secret, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("disk");

    _source = doc.createElement("source");
    _source.setAttribute("pool", pool->text());
    _source.setAttribute("volume", volume->text());
    // used for iscsi pool only
    if ( auth->auth->isChecked() ) {
        _auth = doc.createElement("auth");
        _auth.setAttribute(
                    "username", auth->userName->text());
        _secret = doc.createElement("secret");
        if ( auth->getSecretType().toLower().compare("iscsi")==0 ) {
            _secret.setAttribute("type", "iscsi");
            _secret.setAttribute(
                        auth->usageType->currentText().toLower(),
                        auth->usage->text());
            _auth.appendChild(_secret);
            _devDesc.appendChild(_auth);
        };
    };
    //
    if ( startupPolicy->isUsed() )
        _source.setAttribute(
                    "startupPolicy", startupPolicy->getStartupPolicy());
    if ( mode->isEnabled() )
        _source.setAttribute(
                    "mode", mode->currentText().toLower());
    _devDesc.appendChild(_source);

    _target = doc.createElement("target");
    TargetAttrs l = target->getTargetAttrs();
    foreach (QString key, l.keys()) {
        if ( !key.isEmpty() )
        _target.setAttribute(key, l.value(key));
    };
    _devDesc.appendChild(_target);

    if ( secLabels->isUsed() ) {
        QDomNodeList _l = secLabels->getDataDocument()
                .firstChildElement("data")
                .childNodes();
        int j = 0;
        int count = _l.length();
        for (int i=0; i<count;i++) {
            //qDebug()<<_l.item(j).nodeName()<<i;
            if (!_l.item(j).isNull()) _devDesc.appendChild(_l.item(j));
            else ++j;
        };
    };

    if ( encrypt->isUsed() ) {
        QDomElement _encrypt = doc.createElement("encryption");
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

    _devDesc.setAttribute("type", "volume");
    _devDesc.setAttribute("device", devType->getDeviceType());
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void Volume_Disk::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device, _source, _auth, _secret, _target,
            _readOnly, _encrypt, _secLabel, _addr, _driver;
    _device = doc.firstChildElement("device")
            .firstChildElement("disk");
    _source = _device.firstChildElement("source");
    _target = _device.firstChildElement("target");
    _secLabel = _device.firstChildElement("seclabel");
    _encrypt = _device.firstChildElement("encryption");
    _readOnly = _device.firstChildElement("readonly");
    _addr = _device.firstChildElement("address");
    _driver = _device.firstChildElement("driver");
    QString _attr;
    int idx = -1;
    _attr = _device.attribute("device");
    devType->setDeviceType(_attr);
    _attr = _source.attribute("pool");
    pool->setText(_attr);
    _attr = _source.attribute("volume");
    volume->setText(_attr);
    if ( _source.hasAttribute("mode") ) {
        idx = mode->findText(
                    _source.attribute("mode"), Qt::MatchContains);
        mode->setCurrentIndex( (idx<0)? 0:idx );
    };
    // used for iscsi pool only
    _auth = _device.firstChildElement("auth");
    if ( !_auth.isNull() ) {
        auth->setVisible(true);
        auth->auth->setChecked( true );
        auth->setSecretType("iSCSI");
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
            } else if ( _secret.hasAttribute("type") ) {
                _u = _secret.attribute("type");
                auth->setSecretType(_u.toLower());
            };
            auth->usageType->setCurrentIndex( (idx<0)? 0:idx );
            auth->usage->setText(_u);
        };
    };
    //
    if ( _source.hasAttribute("startupPolicy") ) {
        startupPolicy->setUsage(true);
        _attr = _source.attribute("startupPolicy");
        idx = startupPolicy->findPolicyIndex(_attr);
        startupPolicy->setPolicyIndex( (idx<0)? 0:idx );
    } else
        startupPolicy->setUsage(false);
    _attr = _target.attribute("dev");
    target->devName->setText(_attr);
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
    secLabels->setUsage( !_secLabel.isNull() );
    if ( !_secLabel.isNull() ) {
        QDomDocument _doc;
        _doc.setContent(QString());
        _doc.appendChild(_device);
        _device.setTagName("domain");
        QString _xml = _doc.toString();
        secLabels->readXMLDesciption(_xml);
    };
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
                    _attr, Qt::UserRole, Qt::MatchExactly);
        addr->type->setCurrentIndex( (idx<0)? 0:idx );
        if ( _attr=="pci" ) {
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
        } else if ( _attr=="drive" ) {
            DriveAddr *wdg = static_cast<DriveAddr*>( addr->getCurrentAddrWidget() );
            if ( wdg!=nullptr ) {
                wdg->controller->setText( _addr.attribute("controller") );
                wdg->bus->setText( _addr.attribute("bus") );
                wdg->target->setText( _addr.attribute("target") );
                wdg->unit->setText( _addr.attribute("unit") );
            };
        };
    };
}

/* private slots */
void Volume_Disk::getVolumeNames()
{
    VVD_Result _ret;
    VirtVolumeDialog *volumeDialog =
            new VirtVolumeDialog(this, ptr_ConnPtr);
    if ( volumeDialog->exec()==QDialog::Accepted ) {
        _ret = volumeDialog->getResult();
        pool->setText(_ret.pool);
        volume->setText(_ret.name);
        encrypt->setCurrVolumePath(_ret.path);
        if ( _ret.type.compare("iscsi")==0 ) {
            auth->setVisible(true);
            auth->setSecretType("ISCSI");
            mode->setEnabled(true);
            modeLabel->setEnabled(true);
        } else {
            auth->auth->setChecked(false);
            auth->setVisible(false);
            auth->setSecretType(NOT_VOLUME);
            mode->setEnabled(false);
            modeLabel->setEnabled(false);
        };
    };
    volumeDialog->deleteLater();
}
