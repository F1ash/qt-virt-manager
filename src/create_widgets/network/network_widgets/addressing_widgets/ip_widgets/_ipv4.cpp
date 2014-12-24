#include "_ipv4.h"

_IPv4::_IPv4(QWidget *parent, bool *hasDHCP) :
    _IPvX(parent, hasDHCP)
{
    setObjectName("IPv4");
    address->setPlaceholderText("192.168.152.1");
    prefix->setValue(24);
    gateway->setPlaceholderText("192.168.122.2");
}

/* public slots */
QDomDocument _IPv4::getDataDocument() const
{
    QDomDocument doc;
    return doc;
}
void _IPv4::setStaticRouteMode(bool state)
{
    QString _addr = QString("192.168.152.%1")
            .arg( (state )? "0":"1" );
    address->setPlaceholderText(_addr);
    setGatewayEnabled(state);
    setDHCPEnabled(!state);
}
