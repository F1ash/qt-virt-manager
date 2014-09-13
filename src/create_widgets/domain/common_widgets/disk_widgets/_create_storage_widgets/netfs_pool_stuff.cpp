#include "netfs_pool_stuff.h"

NetFs_Pool_Stuff::NetFs_Pool_Stuff(QWidget *parent) :
    _Pool_Stuff(parent)
{
}

/* public slots */
QString NetFs_Pool_Stuff::getStorageXMLDescFileName() const
{
    return QString();
}
