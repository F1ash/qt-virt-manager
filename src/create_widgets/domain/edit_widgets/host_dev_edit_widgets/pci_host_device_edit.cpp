#include "pci_host_device_edit.h"

PCI_Host_Device_Edit::PCI_Host_Device_Edit(QWidget *parent) :
    _QWidget(parent)
{
    addr = new DeviceAddress(this);
    int idx = addr->type->findText("pci", Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    addr->setCurrentAddrWidget(idx);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(addr);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument PCI_Host_Device_Edit::getDataDocument() const
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
    _devDesc.setAttribute("type", "pci");
    _devDesc.setAttribute("mode", "subsystem");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void PCI_Host_Device_Edit::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("hostdev");
    _source = _device.firstChildElement("source");
    _addr = _source.firstChildElement("address");
    addr->use->setChecked(!_addr.isNull());
    if ( !_addr.isNull() ) {
        PciAddr *wdg = static_cast<PciAddr*>( addr->getCurrentAddrWidget() );
        wdg->domain->setText( _addr.attribute("domain") );
        wdg->bus->setText( _addr.attribute("bus") );
        wdg->slot->setText( _addr.attribute("slot") );
        wdg->function->setValue( _addr.attribute("function").toInt() );
    };
}
