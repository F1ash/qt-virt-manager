#include "dir_disk.h"

Dir_Disk::Dir_Disk(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    _Disk(parent, connPtrPtr)
{
    browse = new QPushButton(tr("Directory:"), this);
    path = new QLineEdit(this);
    path->setPlaceholderText("/var/lib/xen/images/");

    baseLayout->addWidget(browse, 0, 0);
    baseLayout->addWidget(path, 0, 1);

    connect(browse, SIGNAL(clicked()),
            this, SLOT(setDirPath()));
    // dataChanged connections
    connect(path, SIGNAL(textChanged(QString)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Dir_Disk::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _target, _device, _devDesc,
            _encrypt, _secret;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("disk");

    _source = doc.createElement("source");
    _source.setAttribute("dir", path->text());
    if ( startupPolicy->isUsed() )
        _source.setAttribute("startupPolicy",
                             startupPolicy->getStartupPolicy());
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

    _devDesc.setAttribute("type", "dir");
    _devDesc.setAttribute("device", devType->getDeviceType());
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void Dir_Disk::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device, _source, _target, _encrypt,
            _secret, _readOnly, _secLabel, _addr, _driver;
    _device = doc.firstChildElement("device")
            .firstChildElement("disk");
    _source = _device.firstChildElement("source");
    _target = _device.firstChildElement("target");
    _secLabel = _device.firstChildElement("seclabel");
    _readOnly = _device.firstChildElement("readonly");
    _addr = _device.firstChildElement("address");
    _driver = _device.firstChildElement("driver");
    QString _attr;
    int idx;
    _attr = _device.attribute("device");
    devType->setDeviceType(_attr);
    _attr = _source.attribute("dir");
    path->setText(_attr);
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
void Dir_Disk::setDirPath()
{
    QString dirPath = QFileDialog::getExistingDirectory(
                this,
                tr("Directory Path"),
                "/");
    if ( !dirPath.isEmpty() ) path->setText(dirPath);
}
