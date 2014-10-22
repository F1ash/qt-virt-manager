#include "scsi_host_device_edit.h"

SCSI_Host_Device_Edit::SCSI_Host_Device_Edit(QWidget *parent) :
    _QWidget(parent)
{
    scsiType = new QComboBox(this);
    scsiType->addItem("SCSI Adapter Name", "scsi");
    scsiType->addItem("iSCSI Device Name", "iscsi");
    adapterName = new QLineEdit(this);
    adapterName->setPlaceholderText("scsi_hostN");
    scsiDevice = new AdapterAddress(this);
    iscsiDevice = new ISCSI_Device(this);
    scsiDevices = new QStackedWidget(this);
    scsiDevices->addWidget(scsiDevice);
    scsiDevices->addWidget(iscsiDevice);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(scsiType, 0, 0);
    baseLayout->addWidget(adapterName, 0, 1);
    baseLayout->addWidget(scsiDevices, 1, 0, 2, 2);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    addr = new DeviceAddress(this);
    int idx = addr->type->findText("drive", Qt::MatchContains);
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
    connect(adapterName, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    for (uint i=0; i<scsiDevices->count(); i++) {
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
    AttrList l = addr->getAttrList();
    if (  addr->use->isChecked() && !l.isEmpty() ) {
        QDomElement _address = doc.createElement("address");
        foreach (QString key, l.keys()) {
            if ( !key.isEmpty() )
                _address.setAttribute(key, l.value(key));
        };
        _source.appendChild(_address);
    };
    _devDesc.appendChild(_source);
    _devDesc.setAttribute("type", "scsi");
    _devDesc.setAttribute("mode", "subsystem");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void SCSI_Host_Device_Edit::setDataDescription(QString &xmlDesc)
{
    qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("hostdev");
    _source = _device.firstChildElement("source");
    QString _protocol = _source.attribute("protocol");
    _addr = _source.firstChildElement("address");
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
        adapterName->setPlaceholderText("scsi_hostN");
    } else if ( _type=="iscsi" ) {
        adapterName->setPlaceholderText(
                    "iqn.2014-08.com.example:iscsi-nopool/1");
    }
}
