#include "scsi_host_device_edit.h"

SCSI_Host_Device_Edit::SCSI_Host_Device_Edit(
        QWidget *parent, virConnectPtr *connPtr) :
    _QWidget(parent, connPtr)
{
    info = new QLabel(this);
    info->setPixmap(QIcon::fromTheme("dialog-warning")
                    .pixmap(this->fontInfo().pixelSize()));
    infoText = new QLabel(this);
    infoText->setText("For SCSI devices, user\nis responsible to make sure\nthe device is not used by host");
    scsiType = new QComboBox(this);
    scsiType->addItem("SCSI Adapter Name", "scsi");
    scsiType->addItem("iSCSI Device Name", "iscsi");
    devName = new QLineEdit(this);
    devName->setPlaceholderText("scsi_hostN");
    scsiDevice = new AdapterAddress(this);
    iscsiDevice = new ISCSI_Device(this, currConnPtr);
    scsiDevices = new QStackedWidget(this);
    scsiDevices->addWidget(scsiDevice);
    scsiDevices->addWidget(iscsiDevice);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(info, 0, 0);
    baseLayout->addWidget(infoText, 0, 1);
    baseLayout->addWidget(scsiType, 1, 0);
    baseLayout->addWidget(devName, 1, 1);
    baseLayout->addWidget(scsiDevices, 2, 0, 3, 2);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    addr = new DeviceAddress(this);
    int idx = addr->type->findData(
                "drive",
                Qt::UserRole,
                Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    addr->setCurrentAddrWidget(idx);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(addr);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(scsiType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(scsiTypeChanged(int)));
    connect(scsiType, SIGNAL(currentIndexChanged(int)),
            scsiDevices, SLOT(setCurrentIndex(int)));
    // dataChanged signals
    connect(scsiType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(devName, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    for (int i=0; i<scsiDevices->count(); i++) {
        connect(scsiDevices->widget(i), SIGNAL(dataChanged()),
                this, SLOT(stateChanged()));
    };
}

/* public slots */
QDomDocument SCSI_Host_Device_Edit::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("hostdev");
    _source = doc.createElement("source");
    QString _protocol = scsiType->itemData(
                scsiType->currentIndex(),
                Qt::UserRole).toString();
    if ( _protocol=="iscsi" ) {
        QDomElement _host, _auth, _secret;
        _source.setAttribute("protocol", "iscsi");
        _source.setAttribute(
                    "name", devName->text());
        _host = doc.createElement("host");
        _host.setAttribute(
                    "name",
                    iscsiDevice->host->text());
        _host.setAttribute(
                    "port",
                    iscsiDevice->port->text());
        _source.appendChild(_host);
        if ( iscsiDevice->auth->auth->isChecked() ) {
            _auth = doc.createElement("auth");
            _source.appendChild(_auth);
            _auth.setAttribute(
                        "username",
                        iscsiDevice->auth->userName->text());
            _secret = doc.createElement("secret");
            _auth.appendChild(_secret);
            _secret.setAttribute("type", "iscsi");
            QString _tag =
                    iscsiDevice->auth->usageType->currentText()
                    .toLower();
            _secret.setAttribute(
                        _tag,
                        iscsiDevice->auth->usage->text());
        };
    } else {
        QDomElement _adapter, _scsiAddr;
        _adapter = doc.createElement("adapter");
        _adapter.setAttribute(
                    "name", devName->text());
        _source.appendChild(_adapter);
        if ( scsiDevice->adapterAddr->isChecked() ) {
            _scsiAddr = doc.createElement("address");
            _scsiAddr.setAttribute("type", "scsi");
            _scsiAddr.setAttribute(
                        "bus",
                        scsiDevice->bus->text());
            _scsiAddr.setAttribute(
                        "target",
                        scsiDevice->target->text());
            _scsiAddr.setAttribute(
                        "unit",
                        scsiDevice->unit->text());
            _source.appendChild(_scsiAddr);
        };
    };
    _devDesc.appendChild(_source);
    AttrList l = addr->getAttrList();
    if ( addr->use->isChecked() && !l.isEmpty() ) {
        QDomElement _address = doc.createElement("address");
        foreach (QString key, l.keys()) {
            if ( !key.isEmpty() )
                _address.setAttribute(key, l.value(key));
        };
        _devDesc.appendChild(_address);
    };
    _devDesc.setAttribute("type", "scsi");
    _devDesc.setAttribute("mode", "subsystem");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void SCSI_Host_Device_Edit::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("hostdev");
    _source = _device.firstChildElement("source");
    QString _protocol = _source.attribute("protocol");
    int idx = scsiType->findData(
                _protocol,
                Qt::UserRole,
                Qt::MatchContains);
    scsiType->setCurrentIndex( (idx<0)? 0:idx );
    if ( _protocol=="iscsi" ) {
        devName->setText(
                    _source
                    .attribute("name"));
        QDomElement _host, _auth, _secret;
        _host = _source.firstChildElement("host");
        if ( !_host.isNull() ) {
            iscsiDevice->host->setText(
                        _host.attribute("name"));
            iscsiDevice->port->setValue(
                        _host.attribute("port").toInt());
        };
        _auth = _source.firstChildElement("auth");
        iscsiDevice->auth->auth->setChecked(
                    !_auth.isNull());
        if ( !_auth.isNull() ) {
            iscsiDevice->auth->userName->setText(
                        _auth.attribute("username"));
            _secret = _auth.firstChildElement("secret");
            if ( !_secret.isNull() ) {
                QString _secretId;
                if ( _secret.hasAttribute("usage") ) {
                    iscsiDevice->auth->usageType->setCurrentIndex(0);
                    _secretId = _secret.attribute("usage");
                } else if ( _secret.hasAttribute("uuid") ) {
                    iscsiDevice->auth->usageType->setCurrentIndex(1);
                    _secretId = _secret.attribute("uuid");
                };
                iscsiDevice->auth->usage->setText(_secretId);
            };
        };
    } else {
        QDomElement _adapter, _scsiAddr;
        _adapter = _source.firstChildElement("adapter");
        if ( !_adapter.isNull() ) {
            devName->setText(
                        _adapter.attribute("name"));
        };
        _scsiAddr = _source
                .firstChildElement("address");
        scsiDevice->adapterAddr->setChecked(
                    !_scsiAddr.isNull());
        if ( !_scsiAddr.isNull() ) {
            scsiDevice->bus->setValue(
                        _scsiAddr
                        .attribute("bus").toInt());
            scsiDevice->target->setValue(
                        _scsiAddr
                        .attribute("target").toInt());
            scsiDevice->unit->setValue(
                        _scsiAddr
                        .attribute("unit").toInt());
        }
    };
    _addr = _device.firstChildElement("address");
    addr->use->setChecked(!_addr.isNull());
    if ( !_addr.isNull() ) {
        DriveAddr *wdg = static_cast<DriveAddr*>( addr->getCurrentAddrWidget() );
        wdg->controller->setText( _addr.attribute("controller") );
        wdg->bus->setText( _addr.attribute("bus") );
        wdg->target->setText( _addr.attribute("target") );
        wdg->unit->setText( _addr.attribute("unit") );
    };
}

/* private slots */
void SCSI_Host_Device_Edit::scsiTypeChanged(int i)
{
    QString _type = scsiType->itemData(i, Qt::UserRole).toString();
    if ( _type=="scsi" ) {
        devName->setPlaceholderText("scsi_hostN");
    } else if ( _type=="iscsi" ) {
        devName->setPlaceholderText(
                    "iqn.2014-08.com.example:iscsi-nopool/1");
    }
}
