#include "virtvolume_dialog.h"

VirtVolumeDialog::VirtVolumeDialog(
        QWidget *parent,
        virConnectPtr conn) :
    QDialog(parent), currWorkConnection(conn)
{
    setModal(true);
    poolList = new QListWidget(this);
    setPoolList();
    volumes = new VirtStorageVolControl(this);
    storageThread = new StorageVolControlThread(this);
    connect(storageThread, SIGNAL(resultData(Result)),
            volumes, SLOT(resultReceiver(Result)));

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
    connect(volumes, SIGNAL(addNewTask(TASK)),
            this, SLOT(execAction(TASK)));
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
    int ret = virConnectListAllStoragePools(currWorkConnection, &pools, flags);
    if ( ret<0 ) {
        poolList->addItem("Not found");
    } else {
        // therefore correctly to use for() command, because pools[0] can not exist.
        for (int i = 0; i < ret; i++) {
            poolList->addItem( virStoragePoolGetName(pools[i]) );
            virStoragePoolFree(pools[i]);
        };
        free(pools);
    };
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
    QString currConnName("VirtVolumeDialog");
    // need for identification
    // see for: MainWindow::addStorageVol
    // & VirtStorageVolControl::resultReceiver
    volumes->setObjectName(
                QString("%1_%2").arg(currConnName).arg(_poolName));
    // initiate content
    volumes->setCurrentStoragePool(
                currWorkConnection, currConnName, _poolName);
}
void VirtVolumeDialog::showMsg(QString &msg)
{
    QMessageBox::information(
                this,
                "VirtVolumeDialog",
                msg,
                QMessageBox::Ok);
}
void VirtVolumeDialog::execAction(TASK _task)
{
    storageThread->execAction(0, _task);
}
