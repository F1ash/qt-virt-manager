#include "volume_disk.h"

Volume_Disk::Volume_Disk(
        QWidget *parent,
        virConnectPtr conn) :
    _Disk(parent, conn)
{
    poolLabel = new QLabel("Pool:", this);
    pool = new QLabel(this);
    volumeLabel = new QPushButton("Volume:", this);
    volume = new QLineEdit(this);
    volume->setReadOnly(true);
    modeLabel = new QLabel("LUN source mode:", this);
    modeLabel->setEnabled(false);
    mode = new QComboBox(this);
    mode->addItems(QStringList()<<"Host"<<"Direct");
    mode->setEnabled(false);

    secLabels->setVisible(true);

    baseLayout->addWidget(poolLabel, 0, 0);
    baseLayout->addWidget(pool, 0, 1);
    baseLayout->addWidget(volumeLabel, 1, 0);
    baseLayout->addWidget(volume, 1, 1);
    baseLayout->addWidget(modeLabel, 2, 0);
    baseLayout->addWidget(mode, 2, 1);

    connect(devType->devType, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(changeModeVisibility(QString)));
    connect(volumeLabel, SIGNAL(clicked()),
            this, SLOT(getVolumeNames()));
    // dataChanged connections
    connect(volume, SIGNAL(textChanged(QString)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Volume_Disk::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("disk");

    _source = doc.createElement("source");
    _source.setAttribute("pool", pool->text());
    _source.setAttribute("volume", volume->text());
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
        uint j = 0;
        uint count = _l.length();
        for (uint i=0; i<count;i++) {
            //qDebug()<<_l.item(j).nodeName()<<i;
            if (!_l.item(j).isNull()) _devDesc.appendChild(_l.item(j));
            else ++j;
        };
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
void Volume_Disk::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _target,
            _readOnly, _secLabel, _addr, _driver;
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
    _attr = _source.attribute("pool");
    pool->setText(_attr);
    _attr = _source.attribute("volume");
    volume->setText(_attr);
    if ( _source.hasAttribute("mode") ) {
        idx = mode->findText(
                    _source.attribute("mode"), Qt::MatchContains);
        mode->setCurrentIndex( (idx<0)? 0:idx );
    };
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
        QString _xmlDesc = _doc.toString();
        secLabels->readXMLDesciption(_xmlDesc);
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
void Volume_Disk::changeModeVisibility(QString _devType)
{
    modeLabel->setEnabled( _devType.toLower()=="lun" );
    mode->setEnabled( _devType.toLower()=="lun" );
}
void Volume_Disk::getVolumeNames()
{
    QStringList _ret;
    if ( volumeDialog==NULL ) {
        volumeDialog = new VirtVolumeDialog(this, currWorkConnect);
    };
    if ( volumeDialog->exec()==QDialog::Accepted ) {
        _ret = volumeDialog->getResult();
        if ( !_ret.isEmpty() ) {
            pool->setText(_ret.first());
            volume->setText(_ret.last());
        };
    };
}
