#include "virtvolume_dialog.h"

VirtVolumeDialog::VirtVolumeDialog(
        QWidget *parent, virConnectPtr conn, QString _type) :
    QDialog(parent), currWorkConnection(conn), type(_type)
{
    setModal(true);
    poolList = new QListWidget(this);
    volumes = new VirtStorageVolControl(this);
    storageVolThread = new StorageVolControlThread(this);
    connect(storageVolThread, SIGNAL(resultData(Result)),
            volumes, SLOT(resultReceiver(Result)));
    storagePoolThread = new StoragePoolControlThread(this);
    connect(storagePoolThread, SIGNAL(resultData(Result)),
            this, SLOT(poolThreadResult(Result)));

    connect(poolList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(showVolumes(QListWidgetItem*)));
    listLayout = new QHBoxLayout(this);
    listLayout->addWidget(poolList, 2);
    listLayout->addWidget(volumes, 8);
    listWidget = new QWidget(this);
    listWidget->setLayout(listLayout);

    chooseVolume = new QPushButton(
                QIcon::fromTheme("dialog-ok"), "Choose Volume", this);
    cancel = new QPushButton(
                QIcon::fromTheme("dialog-cancel"), "Cancel", this);
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
    setPoolList();
}

/* public slots */
VVD_Result VirtVolumeDialog::getResult() const
{
    VVD_Result _ret;
    QList<QListWidgetItem*> _list = poolList->selectedItems();
    if ( !_list.isEmpty() ) {
        _ret.pool = _list.at(0)->text();
        QStringList _data = _list.at(0)->data(Qt::UserRole).toStringList();
        if ( _data.count()>2 ) {
            _ret.type   = _data.at(0);
            _ret.source = _data.at(1);
            _ret.target = _data.at(2);
        };
        _ret.name = volumes->getCurrentVolumeName();
        _ret.path = volumes->getCurrentVolumePath();
    };
    return _ret;
}

/* private slots */
void VirtVolumeDialog::setPoolList()
{
    setEnabled(false);
    TASK _task;
    _task.type          = "pool";
    _task.sourceConn    = currWorkConnection;
    _task.action        = GET_ALL_ENTITY_DATA;
    storagePoolThread->execAction(0, _task);
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
    storageVolThread->execAction(0, _task);
}
void VirtVolumeDialog::poolThreadResult(Result data)
{
    if ( data.msg.isEmpty() ) {
        poolList->addItem("Not found");
    } else {
        foreach ( QString chain, data.msg ) {
            QStringList _data = chain.split(DFR);
            QString _type = _data.at(1);
            //qDebug()<<_data;
            if        ( type.isEmpty() ) {
                addPoolItem(_data);
            } else if ( type==_type ) {
                addPoolItem(_data);
            }
        };
    };
    setEnabled(true);
}
void VirtVolumeDialog::addPoolItem(QStringList &_data)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setText( _data.first() );
    _data.removeFirst();
    item->setData( Qt::UserRole, _data );
    poolList->addItem(item);
}
