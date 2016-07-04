#include "_ipv4.h"

_IPv4::_IPv4(QWidget *parent, bool hasDHCP, uint _ver) :
    _IPvX(parent, hasDHCP, _ver)
{
    setObjectName("IPv4");
    address->setPlaceholderText("192.168.152.1");
    gateway->setPlaceholderText("192.168.122.2");
}

/* public slots */
void _IPv4::setStaticRouteMode(bool state)
{
    QString _addr = QString("192.168.152.%1")
            .arg( (state )? "0":"1" );
    address->setPlaceholderText(_addr);
    setGatewayEnabled(state);
}
