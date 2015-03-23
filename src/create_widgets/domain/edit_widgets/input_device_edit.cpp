#include "input_device_edit.h"

InputDevice_Edit::InputDevice_Edit(QWidget *parent) :
    InputDevice(parent)
{
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(bus, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
void InputDevice_Edit::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("input");
    QString _attr;
    _attr = _device.attribute("type");
    int idx = type->findData(
                _attr,
                Qt::UserRole,
                Qt::MatchContains);
    type->setCurrentIndex( (idx<0)? 0:idx );
    _attr = _device.attribute("bus");
    idx = bus->findData(
                _attr,
                Qt::UserRole,
                Qt::MatchContains);
    bus->setCurrentIndex( (idx<0)? 0:idx );
    _addr = _device.firstChildElement("address");
    addr->use->setChecked(!_addr.isNull());
    if ( !_addr.isNull() ) {
        PciAddr *wdg = static_cast<PciAddr*>( addr->getCurrentAddrWidget() );
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
    };
}
