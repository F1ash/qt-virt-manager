#include "hub_device_edit.h"

HubDevice_Edit::HubDevice_Edit(QWidget *parent) :
    HubDevice(parent)
{
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
void HubDevice_Edit::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("hub");
    QString _type = _device.attribute("type", "");
    int idx = addr->type->findText(_type, Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    _addr = _device.firstChildElement("address");
    addr->use->setChecked(!_addr.isNull());
    if ( !_addr.isNull() ) {
        USBAddr *wdg = static_cast<USBAddr*>( addr->getCurrentAddrWidget() );
        if ( wdg!=nullptr ) {
            wdg->bus->setText( _addr.attribute("bus") );
            wdg->port->setText( _addr.attribute("port") );
        };
    };
}
