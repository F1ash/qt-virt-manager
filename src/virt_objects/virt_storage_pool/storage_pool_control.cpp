#include "storage_pool_control.h"

VirtStoragePoolControl::VirtStoragePoolControl(QWidget *parent) :
    VirtEntityControl(parent)
{
    setObjectName("VirtStoragePoolControl");
    setWindowTitle("StoragePool Control");
    setWindowIcon(QIcon::fromTheme("storagePool"));
    storagePoolModel = new StoragePoolModel();
    entityList->setModel(storagePoolModel);
    //connect(entityList, SIGNAL(doubleClicked(const QModelIndex&)),
    //        this, SLOT(entityDoubleClicked(const QModelIndex&)));
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
    connect(toolBar, SIGNAL(fileForMethod(const QStringList&)),
            this, SLOT(newVirtEntityFromXML(const QStringList&)));
    connect(toolBar, SIGNAL(execMethod(const QStringList&)),
            this, SLOT(execAction(const QStringList&)));
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
    //disconnect(entityList, SIGNAL(doubleClicked(const QModelIndex&)),
    //           this, SLOT(entityDoubleClicked(const QModelIndex&)));
    disconnect(entityList, SIGNAL(customContextMenuRequested(const QPoint&)),
               this, SLOT(entityClicked(const QPoint&)));
    disconnect(toolBar, SIGNAL(fileForMethod(const QStringList&)),
               this, SLOT(newVirtEntityFromXML(const QStringList&)));
    disconnect(toolBar, SIGNAL(execMethod(const QStringList&)),
               this, SLOT(execAction(const QStringList&)));

    stopProcessing();

    delete toolBar;
    toolBar = NULL;

    if (storagePoolModel!=NULL) {
        delete storagePoolModel;
        storagePoolModel = NULL;
    };

    if (entityList!=NULL) {
        delete entityList;
        entityList = NULL;
    };
}

/* public slots */
bool VirtStoragePoolControl::getThreadState() const
{
    return true;
}
void VirtStoragePoolControl::stopProcessing()
{
    // clear StoragePool list
    while ( storagePoolModel->DataList.count() ) {
        storagePoolModel->removeRow(0);
    };
    storagePoolModel->setHeaderData(0, Qt::Horizontal, QString("Name"), Qt::EditRole);
    setEnabled(false);

}
bool VirtStoragePoolControl::setCurrentWorkConnect(virConnect *conn)
{
    stopProcessing();
    currWorkConnect = conn;
    toolBar->enableAutoReload();
    return true;
}
void VirtStoragePoolControl::setListHeader(QString &connName)
{
    storagePoolModel->setHeaderData(
                0, Qt::Horizontal,
                QString("Name (Conn: \"%1\")").arg(connName), Qt::EditRole);
    currConnName = connName;
    setEnabled(true);
    // for initiation content
    QStringList args;
    args.prepend("reloadVirtStoragePool");
    args.prepend(QString::number(GET_ALL_ENTITY));
    args.prepend(currConnName);
    emit addNewTask(currWorkConnect, args);
}
void VirtStoragePoolControl::resultReceiver(Result data)
{
    QStringList args;
    //qDebug()<<data.action<<data.msg<<"result";
    if ( data.action == GET_ALL_ENTITY ) {
        if ( data.msg.count() > storagePoolModel->DataList.count() ) {
            int _diff = data.msg.count() - storagePoolModel->DataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                storagePoolModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( storagePoolModel->DataList.count() > data.msg.count() ) {
            int _diff = storagePoolModel->DataList.count() - data.msg.count();
            for ( int i = 0; i<_diff; i++ ) {
                storagePoolModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QString _data, data.msg) {
            QStringList chain = _data.split(DFR);
            if (chain.isEmpty()) continue;
            int count = chain.size();
            for (int j=0; j<count; j++) {
                storagePoolModel->setData(storagePoolModel->index(i,j), chain.at(j), Qt::EditRole);
            };
            i++;
        };
    } else if ( data.action == CREATE_ENTITY ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
            args.prepend("reloadVirtStoragePool");
            args.prepend(QString::number(GET_ALL_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        };
    } else if ( data.action == DEFINE_ENTITY ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
            args.prepend("reloadVirtStoragePool");
            args.prepend(QString::number(GET_ALL_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        };
    } else if ( data.action == START_ENTITY ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
            args.prepend("reloadVirtStoragePool");
            args.prepend(QString::number(GET_ALL_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        };
    } else if ( data.action == DESTROY_ENTITY ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
            args.prepend("reloadVirtStoragePool");
            args.prepend(QString::number(GET_ALL_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        };
    } else if ( data.action == UNDEFINE_ENTITY ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
            args.prepend("reloadVirtStoragePool");
            args.prepend(QString::number(GET_ALL_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        };
    } else if ( data.action == CHANGE_ENTITY_AUTOSTART ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
            args.prepend("reloadVirtStoragePool");
            args.prepend(QString::number(GET_ALL_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        };
    } else if ( data.action == GET_XML_DESCRIPTION ) {
        if ( !data.msg.isEmpty() ) {
            QString xml = data.msg.first();
            data.msg.removeFirst();
            data.msg.append(QString("to <a href='%1'>%1</a>").arg(xml));
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
            QDesktopServices::openUrl(QUrl(xml));
        };
    };
}
void VirtStoragePoolControl::stopOverView()
{
    for ( int i=0; i<storagePoolModel->DataList.count(); i++ ) {
        storagePoolModel->DataList.at(i)->setOnView(false);
    };
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
    QStringList params;
    if ( idx.isValid() && storagePoolModel->DataList.count()>idx.row() ) {
        //qDebug()<<storagePoolModel->DataList.at(idx.row())->getName();
        params<<storagePoolModel->DataList.at(idx.row())->getName();
        params<<storagePoolModel->DataList.at(idx.row())->getState();
        params<<storagePoolModel->DataList.at(idx.row())->getAutostart();
        params<<storagePoolModel->DataList.at(idx.row())->getPersistent();
    } else {
        entityList->clearSelection();
    };
    bool state = toolBar->getAutoReloadState();
    StoragePoolControlMenu *storagePoolControlMenu = new StoragePoolControlMenu(this, params, state);
    connect(storagePoolControlMenu, SIGNAL(execMethod(const QStringList&)),
            this, SLOT(execAction(const QStringList&)));
    storagePoolControlMenu->move(QCursor::pos());
    storagePoolControlMenu->exec();
    disconnect(storagePoolControlMenu, SIGNAL(execMethod(const QStringList&)),
               this, SLOT(execAction(const QStringList&)));
    storagePoolControlMenu->deleteLater();
}
void VirtStoragePoolControl::entityDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        qDebug()<<storagePoolModel->DataList.at(index.row())->getName();
    }
}
void VirtStoragePoolControl::execAction(const QStringList &l)
{
    QStringList args;
    QModelIndex idx = entityList->currentIndex();
    if ( idx.isValid() && storagePoolModel->DataList.count()>idx.row() ) {
        QString storagePoolName = storagePoolModel->DataList.at(idx.row())->getName();
        args.append(storagePoolName);
        if        ( l.first()=="startVirtStoragePool" ) {
            args.prepend(l.first());
            args.prepend(QString::number(START_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="destroyVirtStoragePool" ) {
            args.prepend(l.first());
            args.prepend(QString::number(DESTROY_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="undefineVirtStoragePool" ) {
            args.prepend(l.first());
            args.prepend(QString::number(UNDEFINE_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="setAutostartVirtStoragePool" ) {
            /* set the opposite value */
            QString autostartState =
                (storagePoolModel->DataList.at(idx.row())->getAutostart()=="yes")
                 ? "0" : "1";
            args.append(autostartState);
            args.prepend(l.first());
            args.prepend(QString::number(CHANGE_ENTITY_AUTOSTART));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="deleteVirtStoragePool" ) {
            if ( l.count()>1 ) {
                args.append(l.at(1));
                //stPoolControlThread->execAction(DELETE_ENTITY, args);
                args.prepend(l.first());
                args.prepend(QString::number(DELETE_ENTITY));
                args.prepend(currConnName);
                emit addNewTask(currWorkConnect, args);
            };
        } else if ( l.first()=="getVirtStoragePoolXMLDesc" ) {
            args.prepend(l.first());
            args.prepend(QString::number(GET_XML_DESCRIPTION));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="overviewVirtStoragePool" ) {
            uint row = idx.row();
            for ( int i=0; i<storagePoolModel->DataList.count(); i++ ) {
                storagePoolModel->DataList.at(i)->setOnView(i==row);
            };
            emit currPool(currWorkConnect, currConnName, storagePoolName);
        } else if ( l.first()=="reloadVirtStoragePool" ) {
            args.prepend(l.first());
            args.prepend(QString::number(GET_ALL_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        };
    } else if ( l.first()=="reloadVirtStoragePool" ) {
        args.prepend(l.first());
        args.prepend(QString::number(GET_ALL_ENTITY));
        args.prepend(currConnName);
        emit addNewTask(currWorkConnect, args);
    };
}
void VirtStoragePoolControl::newVirtEntityFromXML(const QStringList &_args)
{
    if ( !_args.isEmpty() ) {
        Actions act;
        QString actName;
        if ( _args.first().startsWith("create") ) {
            act = CREATE_ENTITY;
            actName = "createVirtStoragePool";
        } else {
            act = DEFINE_ENTITY;
            actName = "reloadVirtStoragePool";
        };
        QStringList args = _args;
        args.removeFirst();
        if ( !args.isEmpty() ) {
                if ( args.first()=="manually" ) {
                    args.removeFirst();
                    //QString source = args.first();
                    args.removeFirst();
                    QString path;
                    bool show = false;
                    // show SRC Creator widget
                    // get path for method
                    CreatePool *createPoolDialog = new CreatePool(this);
                    if ( createPoolDialog->exec()==QDialog::Accepted ) {
                        path = createPoolDialog->getStorageXMLDescFileName();
                        show = createPoolDialog->showXMLDescription();
                    };
                    delete createPoolDialog;
                    createPoolDialog = NULL;
                    args.prepend(path);
                    if ( show ) QDesktopServices::openUrl(QUrl(path));
                };
                args.prepend(actName);
                args.prepend(QString::number(act));
                args.prepend(currConnName);
                emit addNewTask(currWorkConnect, args);
        };
    };
}
