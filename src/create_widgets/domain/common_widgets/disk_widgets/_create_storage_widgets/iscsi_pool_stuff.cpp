#include "iscsi_pool_stuff.h"

iSCSI_Pool_Stuff::iSCSI_Pool_Stuff(QWidget *parent) :
    _Pool_Stuff(parent)
{
    source->deviceLabel->setVisible(true);
    source->device->setVisible(true);
    source->hostLabel->setVisible(true);
    source->host->setVisible(true);
    source->authLabel->setVisible(true);
    source->auth->setVisible(true);
}

/* public slots */
QString iSCSI_Pool_Stuff::getStorageXMLDescFileName() const
{
    return QString();
}
