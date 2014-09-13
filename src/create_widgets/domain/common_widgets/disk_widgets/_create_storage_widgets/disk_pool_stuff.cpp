#include "disk_pool_stuff.h"

Disk_Pool_Stuff::Disk_Pool_Stuff(QWidget *parent) :
    _Pool_Stuff(parent)
{
}

/* public slots */
QString Disk_Pool_Stuff::getStorageXMLDescFileName() const
{
    return QString();
}
