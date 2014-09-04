#include "mac_address.h"

MAC_Address::MAC_Address(QWidget *parent) :
    QWidget(parent)
{
}

/* public slots */
QString MAC_Address::getMACAddress() const
{
    return QString();
}
