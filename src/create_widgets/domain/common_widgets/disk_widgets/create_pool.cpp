#include "create_pool.h"

/*
 * http://libvirt.org/formatstorage.html
 */

#define POOL_TYPES QStringList()\
    <<"Dir"<<"Fs"<<"NetFs"<<"Disk"\
    <<"iSCSI"<<"Logical"<<"SCSI"\
    <<"MPath"<<"RBD"<<"SheepDog"\
    <<"Gluster"

CreatePool::CreatePool(QWidget *parent) :
    _CreateStorage(parent)
{
    type->addItems(POOL_TYPES);
}

/* public slots */
QString CreatePool::getStorageXMLDescFileName() const
{
    return QString();
}

/* private slots */
void CreatePool::typeChanged(QString _type)
{

}
