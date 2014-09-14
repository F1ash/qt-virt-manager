#include "scsi_pool_stuff.h"

SCSI_Pool_Stuff::SCSI_Pool_Stuff(QWidget *parent) :
    _Pool_Stuff(parent)
{
    source->adapter->setVisible(true);
}

/* public slots */
QString SCSI_Pool_Stuff::getStorageXMLDescFileName() const
{
    return QString();
}
