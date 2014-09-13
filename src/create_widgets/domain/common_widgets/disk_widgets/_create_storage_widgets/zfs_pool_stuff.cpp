#include "zfs_pool_stuff.h"

ZFS_Pool_Stuff::ZFS_Pool_Stuff(QWidget *parent) :
    _Pool_Stuff(parent)
{
}

/* public slots */
QString ZFS_Pool_Stuff::getStorageXMLDescFileName() const
{
    return QString();
}
