#include "rbd_pool_stuff.h"

RBD_Pool_Stuff::RBD_Pool_Stuff(QWidget *parent) :
    _Pool_Stuff(parent)
{
}

/* public slots */
QString RBD_Pool_Stuff::getStorageXMLDescFileName() const
{
    return QString();
}
