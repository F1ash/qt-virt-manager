#include "filesystems.h"

/*
 * http://libvirt.org/formatdomain.html#elementsFilesystems
 */

#define LXC_FS_TYPES QStringList()<<"Mount"<<"File"<<"Block"<<"RAM"<<"Bind"

#define QEMU_FS_TYPES QStringList()<<"Mount"

#define XEN_FS_TYPES QStringList()<<"None"

FileSystems::FileSystems(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    typeLabel = new QLabel("Type:", this);
    type = new QComboBox(this);
    connType = QString(virConnectGetType(currWorkConnect)).toLower();
    if ( connType=="lxc" ) {
        type->addItems(LXC_FS_TYPES);
    } else if ( connType=="qemu" ) {
        type->addItems(QEMU_FS_TYPES);
    } else if ( connType=="xen" ) {
        type->addItems(XEN_FS_TYPES);
    };
    typeLayout = new QHBoxLayout(this);
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(type);
    typeWdg = new QWidget(this);
    typeWdg->setLayout(typeLayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(typeWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(typeChanged(int)));
    typeChanged(0);
}

/* public slots */
QDomDocument FileSystems::getDevDocument() const
{
    return info->getDevDocument();
}

/* private slots */
void FileSystems::typeChanged(int i)
{
    if ( info!=NULL ) {
        commonLayout->removeWidget(info);
        delete info;
        info = NULL;
    };
    QString _type = type->currentText().toLower();
    if ( _type=="mount" ) {
        info = new MountFsType(this, connType);
    } else if ( _type=="file" ) {
        info = new FileFsType(this, connType);
    } else if ( _type=="block" ) {
        info = new BlockFsType(this, connType);
    } else if ( _type=="ram" ) {
        info = new RAMFsType(this);
    } else if ( _type=="bind" ) {
        info = new BindFsType(this, connType);
    } else info = new _QWidget(this);
    commonLayout->insertWidget(1, info, -1);
}
