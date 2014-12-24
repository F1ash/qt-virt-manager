#include "_ipv6.h"

_IPv6::_IPv6(QWidget *parent, bool *hasDHCP) :
    _IPvX(parent, hasDHCP)
{
    setObjectName("IPv6");
    address->setPlaceholderText("2001:db8:ca2:3::1");
    prefix->setValue(64);
    gateway->setPlaceholderText("2001:db8:ca2:2::2");
}

/* public slots */
QDomDocument _IPv6::getDataDocument() const
{
    QDomDocument doc;
    return doc;
}
void _IPv6::setStaticRouteMode(bool state)
{
    QString _addr = QString("2001:db8:ca2:3::%1")
            .arg( (state )? "0":"1" );
    address->setPlaceholderText(_addr);
    setGatewayEnabled(state);
    setDHCPEnabled(!state);
}
