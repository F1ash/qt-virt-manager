#include "smartcard_device_edit.h"

SmartCardDevice_Edit::SmartCardDevice_Edit(QWidget *parent) :
    SmartCardDevice(parent)
{
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(mode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(channel, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
void SmartCardDevice_Edit::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc, channelDoc;
    doc.setContent(xmlDesc);
    QDomElement _device, _addr, _channel, _channelDev;
    _device = doc.firstChildElement("device")
            .firstChildElement("smartcard");
    QString _mode, _type, _channelXMLDesc;
    _mode = _device.attribute("mode", "host");
    int idx = mode->findText(_mode, Qt::MatchContains);
    mode->setCurrentIndex( (idx<0)? 0:idx );
    _channelDev = channelDoc.createElement("device");
    channelDoc.appendChild(_channelDev);
    _channel = channelDoc.createElement("channel");
    _channelDev.appendChild(_channel);
    _type = _device.attribute("type", "spicevmc");
    _channel.setAttribute("type", _type);
    QDomNodeList list = _device.childNodes();
    uint j = 0;
    uint count = list.length();
    for (int i=0; i<count;i++) {
        //qDebug()<<list.item(j).nodeName()<<i;
        if (!list.item(j).isNull() && list.item(j).toElement().tagName()!="address")
            _channel.appendChild(list.item(j));
        else ++j;
    };
    _channelXMLDesc = channelDoc.toString();
    channel->setDataDescription(_channelXMLDesc);
    idx = addr->type->findText("ccid", Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    _addr = _device.firstChildElement("address");
    addr->use->setChecked(!_addr.isNull());
    if ( !_addr.isNull() ) {
        CCIDAddr *wdg = static_cast<CCIDAddr*>( addr->getCurrentAddrWidget() );
        wdg->bus->setText( _addr.attribute("bus") );
        wdg->slot->setText( _addr.attribute("slot") );
    };
}
