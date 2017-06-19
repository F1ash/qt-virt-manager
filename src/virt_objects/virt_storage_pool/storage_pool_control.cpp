#include "storage_pool_control.h"

VirtStoragePoolControl::VirtStoragePoolControl(QWidget *parent) :
    VirtEntityControl(parent)
{
    setObjectName("VirtStoragePoolControl");
    setWindowTitle("StoragePool Control");
    setWindowIcon(QIcon::fromTheme("storagePool"));
    storagePoolModel = new StoragePoolModel();
    entityList->setModel(storagePoolModel);
    storagePoolListDlg = new StoragePoolListDelegate();
    entityList->setItemDelegate(storagePoolListDlg);
    connect(entityList, SIGNAL(doubleClicked(const QModelIndex&)),
            this, SLOT(entityDoubleClicked(const QModelIndex&)));
    connect(entityList, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(entityClicked(const QPoint&)));
    settings.beginGroup("VirtStoragePoolControl");
    entityList->setColumnWidth(0, settings.value("column0", 132).toInt());
    entityList->setColumnWidth(1, settings.value("column1", 32).toInt());
    entityList->setColumnWidth(2, settings.value("column2", 32).toInt());
    entityList->setColumnWidth(3, settings.value("column3", 32).toInt());
    int area_int = settings.value("ToolBarArea", 4).toInt();
    settings.endGroup();
    toolBar = new StoragePoolToolBar(this);
    addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
    connect(toolBar, SIGNAL(fileForMethod(const Act_Param&)),
            this, SLOT(newVirtEntityFromXML(const Act_Param&)));
    connect(toolBar, SIGNAL(execMethod(const Act_Param&)),
            this, SLOT(execAction(const Act_Param&)));
}
VirtStoragePoolControl::~VirtStoragePoolControl()
{
    settings.beginGroup("VirtStoragePoolControl");
    settings.setValue("column0", entityList->columnWidth(0));
    settings.setValue("column1", entityList->columnWidth(1));
    settings.setValue("column2", entityList->columnWidth(2));
    settings.setValue("column3", entityList->columnWidth(3));
    settings.setValue("ToolBarArea", toolBarArea(toolBar));
    settings.endGroup();
    settings.sync();

    stopProcessing();
}

/* public slots */
void VirtStoragePoolControl::stopProcessing()
{
    setEnabled(false);
    // clear StoragePool list
    while ( storagePoolModel->DataList.count() ) {
        storagePoolModel->removeRow(0);
    };
    storagePoolModel->setHeaderData(
                0,
                Qt::Horizontal,
                QString("Name"),
                Qt::EditRole);

}
bool VirtStoragePoolControl::setCurrentWorkConnect(virConnectPtr *connPtrPtr)
{
    stopProcessing();
    ptr_ConnPtr = connPtrPtr;
    toolBar->enableAutoReload();
    return true;
}
void VirtStoragePoolControl::setListHeader(const QString &connName)
{
    storagePoolModel->setHeaderData(
                0, Qt::Horizontal,
                QString("Pool in [ %1 ]").arg(connName),
                Qt::EditRole);
    currConnName = connName;
    setEnabled(true);
    // for initiation content
    reloadState();
}
void VirtStoragePoolControl::resultReceiver(Result *data)
{
    //qDebug()<<data->action<<data->msg<<"result";
    if ( data->action == Actions::GET_ALL_ENTITY_STATE ) {
        if ( data->data.count() > storagePoolModel->DataList.count() ) {
            int _diff = data->data.count() - storagePoolModel->DataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                storagePoolModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( storagePoolModel->DataList.count() > data->data.count() ) {
            int _diff = storagePoolModel->DataList.count() - data->data.count();
            for ( int i = 0; i<_diff; i++ ) {
                storagePoolModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QVariantMap _data, data->data) {
            if (_data.isEmpty()) continue;
            storagePoolModel->setData(
                            storagePoolModel->index(i, 0),
                            _data.value("name", ""),
                            Qt::EditRole);
            storagePoolModel->setData(
                            storagePoolModel->index(i, 1),
                            _data.value("active", false),
                            Qt::EditRole);
            storagePoolModel->setData(
                            storagePoolModel->index(i, 2),
                            _data.value("auto", false),
                            Qt::EditRole);
            storagePoolModel->setData(
                            storagePoolModel->index(i, 3),
                            _data.value("persistent", false),
                            Qt::EditRole);
            i++;
        };
        entityList->setEnabled(true);
        emit entityListUpdated();
    } else if ( data->action == Actions::CREATE_ENTITY ) {
        if ( !data->msg.isEmpty() || !data->err.isEmpty() ) {
            QString msg = QString("%1<br>%2")
                    .arg(data->msg.join(" "))
                    .arg(data->err);
            msgRepeater(msg);
            reloadState();
        };
    } else if ( data->action == Actions::DEFINE_ENTITY ) {
        if ( !data->msg.isEmpty() || !data->err.isEmpty() ) {
            QString msg = QString("%1<br>%2")
                    .arg(data->msg.join(" "))
                    .arg(data->err);
            msgRepeater(msg);
            reloadState();
        };
    } else if ( data->action == Actions::START_ENTITY ) {
        if ( !data->msg.isEmpty() || !data->err.isEmpty() ) {
            QString msg = QString("%1<br>%2")
                    .arg(data->msg.join(" "))
                    .arg(data->err);
            msgRepeater(msg);
            reloadState();
        };
    } else if ( data->action == Actions::DESTROY_ENTITY ) {
        if ( !data->msg.isEmpty() || !data->err.isEmpty() ) {
            QString msg = QString("%1<br>%2")
                    .arg(data->msg.join(" "))
                    .arg(data->err);
            msgRepeater(msg);
            reloadState();
        };
    } else if ( data->action == Actions::UNDEFINE_ENTITY ) {
        if ( !data->msg.isEmpty() || !data->err.isEmpty() ) {
            QString msg = QString("%1<br>%2")
                    .arg(data->msg.join(" "))
                    .arg(data->err);
            msgRepeater(msg);
            reloadState();
        };
    } else if ( data->action == Actions::CHANGE_ENTITY_AUTOSTART ) {
        if ( !data->msg.isEmpty() || !data->err.isEmpty() ) {
            QString msg = QString("%1<br>%2")
                    .arg(data->msg.join(" "))
                    .arg(data->err);
            msgRepeater(msg);
            reloadState();
        };
    } else if ( data->action == Actions::GET_XML_DESCRIPTION ) {
        QString xml = data->fileName;
        data->msg.append(QString("to <a href='%1'>%1</a>").arg(xml));
        QString msg = QString("%1<br>%2")
                .arg(data->msg.join(" "))
                .arg(data->err);
        msgRepeater(msg);
        if ( data->result )
            QDesktopServices::openUrl(QUrl(xml));
    };
}
void VirtStoragePoolControl::stopOverView()
{
    for ( int i=0; i<storagePoolModel->DataList.count(); i++ ) {
        storagePoolModel->DataList.at(i)->setOnView(false);
    };
}
void VirtStoragePoolControl::reloadState()
{
    entityList->setEnabled(false);
    entityList->clearSelection();
    TASK task;
    task.type       = VIRT_ENTITY::VIRT_STORAGE_POOL;
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    task.action     = Actions::GET_ALL_ENTITY_STATE;
    task.method     = Methods::reloadEntity;
    emit addNewTask(&task);
}

/* private slots */
void VirtStoragePoolControl::changeDockVisibility()
{
    toolBar->setEnabled( !toolBar->isEnabled() );
    entityList->setEnabled( !entityList->isEnabled() );
}
void VirtStoragePoolControl::entityClicked(const QPoint &p)
{
    //qDebug()<<"custom Menu request";
    QModelIndex idx = entityList->indexAt(p);
    QVariantMap params;
    if ( idx.isValid() && storagePoolModel->DataList.count()>idx.row() ) {
        //qDebug()<<storagePoolModel->DataList.at(idx.row())->getName();
        params.insert(
                    "name",
                    storagePoolModel->DataList.at(idx.row())->getName());
        params.insert(
                    "active",
                    storagePoolModel->DataList.at(idx.row())->getState());
        params.insert(
                    "auto",
                    storagePoolModel->DataList.at(idx.row())->getAutostart());
        params.insert(
                    "persistent",
                    storagePoolModel->DataList.at(idx.row())->getPersistent());
    } else {
        entityList->clearSelection();
    };
    bool state = toolBar->getAutoReloadState();
    StoragePoolControlMenu *storagePoolControlMenu =
            new StoragePoolControlMenu(this, params, state);
    connect(storagePoolControlMenu, SIGNAL(execMethod(const Act_Param&)),
            this, SLOT(execAction(const Act_Param&)));
    storagePoolControlMenu->move(QCursor::pos());
    storagePoolControlMenu->exec();
    disconnect(storagePoolControlMenu, SIGNAL(execMethod(const Act_Param&)),
               this, SLOT(execAction(const Act_Param&)));
    storagePoolControlMenu->deleteLater();
}
void VirtStoragePoolControl::entityDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        //qDebug()<<storagePoolModel->DataList.at(index.row())->getName();
        QString storagePoolName = storagePoolModel->DataList.at(index.row())->getName();
        emit overviewStPool(ptr_ConnPtr, currConnName, storagePoolName);
    }
}
void VirtStoragePoolControl::execAction(const Act_Param &param)
{
    QModelIndex idx = entityList->currentIndex();
    if ( idx.isValid() && storagePoolModel->DataList.count()>idx.row() ) {
        QString storagePoolName =
                storagePoolModel->DataList.at(idx.row())->getName();
        TASK task;
        task.type       = VIRT_ENTITY::VIRT_STORAGE_POOL;
        task.srcConnPtr = ptr_ConnPtr;
        task.srcConName = currConnName;
        task.object     = storagePoolName;
        task.method     = param.method;
        if        ( param.method==Methods::startEntity ) {
            task.action     = Actions::START_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==Methods::destroyEntity ) {
            task.action     = Actions::DESTROY_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==Methods::undefineEntity ) {
            task.action     = Actions::UNDEFINE_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==Methods::setAutostartEntity ) {
            /* set the opposite value */
            uint autostartState =
                (storagePoolModel->DataList.at(idx.row())->getAutostart())
                 ? 0 : 1;
            task.action     = Actions::CHANGE_ENTITY_AUTOSTART;
            task.args.sign  = autostartState;
            emit addNewTask(&task);
        } else if ( param.method==Methods::deleteEntity ) {
            task.action     = Actions::DELETE_ENTITY;
            task.args.sign  = param.path.toUInt();
            emit addNewTask(&task);
        } else if ( param.method==Methods::getEntityXMLDesc ) {
            task.action     = Actions::GET_XML_DESCRIPTION;
            emit addNewTask(&task);
        } else if ( param.method==Methods::overviewEntity ) {
            // don't set onView state, because it can be multiplicate
            //uint row = idx.row();
            //for ( int i=0; i<storagePoolModel->DataList.count(); i++ ) {
            //    storagePoolModel->DataList.at(i)->setOnView(i==row);
            //};
            emit overviewStPool(ptr_ConnPtr, currConnName, storagePoolName);
        } else if ( param.method==Methods::reloadEntity ) {
            reloadState();
        };
    } else if ( param.method==Methods::reloadEntity ) {
        reloadState();
    };
}
void VirtStoragePoolControl::newVirtEntityFromXML(const Act_Param &args)
{
    TASK task;
    task.type = VIRT_ENTITY::VIRT_STORAGE_POOL;
    Methods method;
    if ( args.act==Actions::CREATE_ENTITY ) {
        method = Methods::createEntity;
    } else {
        method = Methods::defineEntity;
    };
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    task.method     = method;
    task.action     = args.act;
    if ( args.context==HOW_TO_DO::DO_AsIs ) {
        task.args.path  = args.path;
        emit addNewTask(&task);
    } else {
        QString path;
        bool show = false;
        CreatePool *createPoolDialog =
                new CreatePool(this, ptr_ConnPtr, currConnName, args.path);
        int result = createPoolDialog->exec();
        if ( result==QDialog::Accepted ) {
            path = createPoolDialog->getXMLDescFileName();
            show = createPoolDialog->showXMLDescription();
        };
        createPoolDialog->deleteLater();
        if ( result==QDialog::Rejected ) return;
        task.args.path = path;
        if ( show ) QDesktopServices::openUrl(QUrl(path));
        emit addNewTask(&task);
    };
}
