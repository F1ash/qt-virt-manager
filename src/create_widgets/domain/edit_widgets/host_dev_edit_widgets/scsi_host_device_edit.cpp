#include "scsi_host_device_edit.h"

SCSI_Host_Device_Edit::SCSI_Host_Device_Edit(QWidget *parent) :
    _QWidget(parent)
{
    addr = new DeviceAddress(this);
    int idx = addr->type->findText("drive", Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    addr->setCurrentAddrWidget(idx);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(addr);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
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
