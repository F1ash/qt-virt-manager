#include "virtvolume_dialog.h"

VirtVolumeDialog::VirtVolumeDialog(
        QWidget *parent,
        virConnectPtr conn) :
    QDialog(parent), currWorkConnect(conn)
{
    setModal(true);
    poolList = new QListWidget(this);
    setPoolList();
    volumes = new VirtStorageVolControl(this);

     connect(poolList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(showVolumes(QListWidgetItem*)));
    listLayout = new QHBoxLayout(this);
    listLayout->addWidget(poolList, 2);
    listLayout->addWidget(volumes, 8);
    listWidget = new QWidget(this);
    listWidget->setLayout(listLayout);

    chooseVolume = new QPushButton(QIcon::fromTheme("dialog-ok"), "Choose Volume", this);
    cancel = new QPushButton(QIcon::fromTheme("dialog-cancel"), "Cancel", this);
    connect(chooseVolume, SIGNAL(clicked()), this, SLOT(set_Result()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    buttonLayout = new QHBoxLayout(this);
    buttonLayout->addWidget(chooseVolume);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(listWidget);
    commonLayout->addWidget(buttons);
    setLayout(commonLayout);
    connect(volumes, SIGNAL(entityMsg(QString&)),
            this, SLOT(showMsg(QString&)));
}

/* public slots */
QStringList VirtVolumeDialog::getResult() const
{
    QStringList _ret;
    QList<QListWidgetItem*> _list = poolList->selectedItems();
    if ( !_list.isEmpty() ) {
        _ret.append( _list.at(0)->text() );
        _ret.append(volumes->getCurrentVolumeName());
    };
    return _ret;
}

/* private slots */
void VirtVolumeDialog::setPoolList()
{
    virStoragePoolPtr *pools = NULL;
    unsigned int flags = VIR_CONNECT_LIST_STORAGE_POOLS_ACTIVE |
                         VIR_CONNECT_LIST_STORAGE_POOLS_INACTIVE;
    int ret = virConnectListAllStoragePools(currWorkConnect, &pools, flags);
    if ( ret<0 ) {
        poolList->addItem("Not found");
    } else {
        int i = 0;
        while ( pools[i] != NULL ) {
            poolList->addItem( virStoragePoolGetName(pools[i]) );
            virStoragePoolFree(pools[i]);
            i++;
        };
    };
    free(pools);
}
void VirtVolumeDialog::set_Result()
{
    done( (sender()==chooseVolume)?
              QDialog::Accepted :
              QDialog::Rejected);
    //qDebug()<<"done";
}
void VirtVolumeDialog::showVolumes(QListWidgetItem *_item)
{
    QString _poolName = _item->text();
    QString _connName;
    volumes->setCurrentStoragePool(
                currWorkConnect,
                _connName,
                _poolName);
}
void VirtVolumeDialog::showMsg(QString &msg)
{
    QMessageBox::information(
                this,
                "Error",
                msg,
                QMessageBox::Ok);
}
