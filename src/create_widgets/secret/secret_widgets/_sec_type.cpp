#include "_sec_type.h"

_SecType::_SecType(QWidget *parent, virConnectPtr _conn) :
    QWidget(parent), currWorkConnection(_conn)
{
    baseLayout = new QVBoxLayout(this);
    setLayout(baseLayout);
}

QDomDocument _SecType::getSecStuff() const
{
    return QDomDocument();
}
