#include "create_pool.h"

/*
 * http://libvirt.org/formatstorage.html
 */

#define POOL_TYPE_DESC QStringList()\
    <<"Directory backend"<<"Local filesystem backend"\
    <<"Network filesystem backend"<<"Logical backend"\
    <<"Disk backend"<<"iSCSI backend"<<"SCSI backend"\
    <<"Multipath backend"<<"RBD (RADOS Block Device) backend"\
    <<"Sheepdog backend"<<"Gluster backend"<<"ZFS backend"

#define POOL_TYPES QStringList()\
    <<"dir"<<"fs"<<"netfs"<<"logical"\
    <<"disk"<<"iscsi"<<"scsi"\
    <<"mpath"<<"rbd"<<"sheepdog"\
    <<"gluster"<<"zfs"

CreatePool::CreatePool(QWidget *parent) :
    _CreateStorage(parent)
{
    QStringList _text, _data;
    _text = POOL_TYPE_DESC;
    _data = POOL_TYPES;
    for (uint i = 0; i<_text.count(); i++) {
        type->addItem(_text.at(i), _data.at(i));
    };
    uuidLabel = new QLabel("UUID:", this);
    uuid = new QLineEdit(this);
    uuid->setPlaceholderText("a UUID will be generated if omitted");
    uuidLayout = new QHBoxLayout();
    uuidLayout->addWidget(uuidLabel);
    uuidLayout->addWidget(uuid);
    uuidWdg = new QWidget(this);
    uuidWdg->setLayout(uuidLayout);
    commonLayout->insertWidget(1, uuidWdg);
}

/* public slots */
QString CreatePool::getStorageXMLDescFileName() const
{
    return info->getStorageXMLDescFileName();
}

/* private slots */
void CreatePool::typeChanged(int i)
{
    if ( info!=NULL ) {
        commonLayout->removeWidget(info);
        delete info;
        info = NULL;
    };
    QString _type = type->itemData(i, Qt::UserRole).toString();
    if ( _type=="dir" ) {
        info = new Dir_Pool_Stuff(this);
    } else if ( _type=="fs" ) {
        info = new Fs_Pool_Stuff(this);
    } else if ( _type=="netfs" ) {
        info = new NetFs_Pool_Stuff(this);
    } else if ( _type=="logical" ) {
        info = new Logical_Pool_Stuff(this);
    } else if ( _type=="disk" ) {
        info = new Disk_Pool_Stuff(this);
    } else if ( _type=="iscsi" ) {
        info = new iSCSI_Pool_Stuff(this);
    } else if ( _type=="scsi" ) {
        info = new SCSI_Pool_Stuff(this);
    } else if ( _type=="mpath" ) {
        info = new MPath_Pool_Stuff(this);
    } else if ( _type=="rbd" ) {
        info = new RBD_Pool_Stuff(this);
    } else if ( _type=="sheepdog" ) {
        info = new SheepDog_Pool_Stuff(this);
    } else if ( _type=="gluster" ) {
        info = new Gluster_Pool_Stuff(this);
    } else if ( _type=="zfs" ) {
        info = new ZFS_Pool_Stuff(this);
    } else info = new _Pool_Stuff(this);
    commonLayout->insertWidget(commonLayout->count()-1, info, -1);
}
