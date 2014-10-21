#include "scsi_host_device_edit.h"

SCSI_Host_Device_Edit::SCSI_Host_Device_Edit(QWidget *parent) :
    _QWidget(parent)
{
    busLabel = new QLabel("Bus:", this);
    targetLabel = new QLabel("Target:", this);
    unitLabel = new QLabel("Unit:", this);
    bus = new QSpinBox(this);
    target = new QSpinBox(this);
    unit = new QSpinBox(this);
    scsiAddrLayout = new QGridLayout();
    scsiAddrLayout->addWidget(busLabel, 0, 0);
    scsiAddrLayout->addWidget(bus, 0, 1);
    scsiAddrLayout->addWidget(targetLabel, 1, 0);
    scsiAddrLayout->addWidget(target, 1, 1);
    scsiAddrLayout->addWidget(unitLabel, 2, 0);
    scsiAddrLayout->addWidget(unit, 2, 1);
    scsiAddr = new QWidget(this);
    scsiAddr->setEnabled(false);
    scsiAddr->setLayout(scsiAddrLayout);
    adapterNameLabel = new QLabel("<u>Adapter</u>:", this);
    adapterName = new QLineEdit();
    adapterName->setPlaceholderText("scsi_hostN");
    adapterAddr = new QCheckBox("SCSI Device\n address:", this);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(adapterNameLabel, 0, 0);
    baseLayout->addWidget(adapterName, 0, 1);
    baseLayout->addWidget(adapterAddr, 1, 0);
    baseLayout->addWidget(scsiAddr, 1, 1);
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
    connect(adapterAddr, SIGNAL(toggled(bool)),
            scsiAddr, SLOT(setEnabled(bool)));
    // dataChanged signals
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(adapterName, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(adapterAddr, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(bus, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(target, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(unit, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument SCSI_Host_Device_Edit::getDataDocument() const
{
    return QDomDocument();
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
