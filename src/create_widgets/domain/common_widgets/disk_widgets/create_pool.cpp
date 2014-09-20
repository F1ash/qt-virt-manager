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
    info->addWidget(new Dir_Pool_Stuff(this));
    info->addWidget(new Fs_Pool_Stuff(this));
    info->addWidget(new NetFs_Pool_Stuff(this));
    info->addWidget(new Logical_Pool_Stuff(this));
    info->addWidget(new Disk_Pool_Stuff(this));
    info->addWidget(new iSCSI_Pool_Stuff(this));
    info->addWidget(new SCSI_Pool_Stuff(this));
    info->addWidget(new MPath_Pool_Stuff(this));
    info->addWidget(new RBD_Pool_Stuff(this));
    info->addWidget(new SheepDog_Pool_Stuff(this));
    info->addWidget(new Gluster_Pool_Stuff(this));
    info->addWidget(new ZFS_Pool_Stuff(this));

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

    commonLayout->insertWidget(commonLayout->count()-1, uuidWdg, -1);
    commonLayout->insertWidget(commonLayout->count()-1, infoWidget, -1);
}

/* public slots */
QString CreatePool::getStorageXMLDescFileName() const
{
    _Pool_Stuff *wdg = static_cast<_Pool_Stuff*>(info->currentWidget());
    return wdg->getStorageXMLDescFileName();
}
