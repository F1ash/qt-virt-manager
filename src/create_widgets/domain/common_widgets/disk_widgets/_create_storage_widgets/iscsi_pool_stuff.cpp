#include "iscsi_pool_stuff.h"

iSCSI_Pool_Stuff::iSCSI_Pool_Stuff(QWidget *parent) :
    _Pool_Stuff(parent)
{
}

/* public slots */
QString iSCSI_Pool_Stuff::getStorageXMLDescFileName() const
{
    return QString();
}
