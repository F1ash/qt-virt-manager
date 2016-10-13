#include "domain_control.h"

VirtDomainControl::VirtDomainControl(QWidget *parent) :
    VirtEntityControl(parent)
{
    setObjectName("VirtDomainControl");
    setWindowTitle("Domain Control");
    setWindowIcon(QIcon::fromTheme("domain"));
    domainModel = new DomainModel();
    connect(entityList, SIGNAL(doubleClicked(const QModelIndex&)),
            this, SLOT(entityDoubleClicked(const QModelIndex&)));
    connect(entityList, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(entityClicked(const QPoint&)));
    entityList->setModel(domainModel);
    domListDlg = new DomainListDelegate();
    entityList->setItemDelegate(domListDlg);
    settings.beginGroup("VirtDomainControl");
    entityList->setColumnWidth(0, settings.value("column0", 132).toInt());
    entityList->setColumnWidth(1, settings.value("column1", 32).toInt());
    entityList->setColumnWidth(2, settings.value("column2", 32).toInt());
    entityList->setColumnWidth(3, settings.value("column3", 32).toInt());
    int area_int = settings.value("ToolBarArea", 4).toInt();
    settings.endGroup();
    toolBar = new DomainToolBar(this);
    addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
    connect(toolBar, SIGNAL(fileForMethod(const Act_Param&)),
            this, SLOT(newVirtEntityFromXML(const Act_Param&)));
    connect(toolBar, SIGNAL(execMethod(const Act_Param&)),
            this, SLOT(execAction(const Act_Param&)));
}
VirtDomainControl::~VirtDomainControl()
{
    settings.beginGroup("VirtDomainControl");
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
void VirtDomainControl::stopProcessing()
{
    setEnabled(false);
    // clear Domain list
    while ( domainModel->DataList.count() ) {
        domainModel->removeRow(0);
    };
    domainModel->setHeaderData(
                0,
                Qt::Horizontal,
                QString("Name"),
                Qt::EditRole);
}
bool VirtDomainControl::setCurrentWorkConnect(virConnectPtr *connPtrPtr)
{
    stopProcessing();
    ptr_ConnPtr = connPtrPtr;
    toolBar->enableAutoReload();
    return true;
}
void VirtDomainControl::setListHeader(const QString &connName)
{
    domainModel->setHeaderData(
                0,
                Qt::Horizontal,
                QString("VM in [ %1 ]").arg(connName),
                Qt::EditRole);
    currConnName = connName;
    setEnabled(true);
    reloadState();
}
virConnectPtr* VirtDomainControl::getPtr_connectionPtr()
{
    return ptr_ConnPtr;
}
void VirtDomainControl::execMigrateAction(virConnectPtr *conn, TASK *_task)
{
    _task->args.destConnPtr = conn;
    _task->args.path.clear();
    emit addNewTask(_task);
}
void VirtDomainControl::resultReceiver(Result *data)
{
    //qDebug()<<data->number<<data->action<<data->msg<<"result";
    if ( data->action == GET_ALL_ENTITY_STATE ) {
        if ( data->data.count() > domainModel->DataList.count() ) {
            int _diff = data->data.count()
                    - domainModel->DataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                domainModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( domainModel->DataList.count() > data->data.count() ) {
            int _diff = domainModel->DataList.count()
                    - data->data.count();
            for ( int i = 0; i<_diff; i++ ) {
                domainModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QVariantMap _data, data->data) {
            if (_data.isEmpty()) continue;
            domainModel->setData(
                            domainModel->index(i, 0),
                            _data.value("name", ""),
                            Qt::EditRole);
            domainModel->setData(
                            domainModel->index(i, 1),
                            _data.value("active", false),
                            Qt::EditRole);
            domainModel->setData(
                            domainModel->index(i, 1),
                            _data.value("state", ""),
                            Qt::DisplayRole);
            domainModel->setData(
                            domainModel->index(i, 2),
                            _data.value("auto", false),
                            Qt::EditRole);
            domainModel->setData(
                            domainModel->index(i, 3),
                            _data.value("persistent", false),
                            Qt::EditRole);
            i++;
        };
        entityList->setEnabled(true);
        emit entityListUpdated();
    } else if ( data->action == GET_XML_DESCRIPTION ) {
        QString xml = data->fileName;
        data->msg.append(QString("to <a href='%1'>%1</a>").arg(xml));
        QString msg = data->msg.join(" ");
        msgRepeater(msg);
        if ( data->result )
            QDesktopServices::openUrl(QUrl(xml));
    } else if ( data->action == EDIT_ENTITY ) {
        if ( !data->msg.isEmpty() ) {
            QString msg = data->msg.join(" ");
            msgRepeater(msg);
        };
        if ( data->result ) {
            // show SRC Creator widget in Edit-mode
            TASK task;
            task.type       = VIRT_DOMAIN;
            task.srcConnPtr = ptr_ConnPtr;
            task.srcConName = currConnName;
            task.object     = data->name;
            task.args.path  = data->fileName;
            task.method     = editEntity;
            task.action     = DEFINE_ENTITY;
            emit domainToEditor(&task);
        };
    } else if ( data->action == GET_ALL_ENTITY_DATA0 ) {
        if ( !data->data.isEmpty() ) {
            TASK task;
            task.srcConnPtr = ptr_ConnPtr;
            task.srcConName = currConnName;
            task.object     = data->name;
            task.args.object= data->data.first().value("DomainType").toString();
            task.args.state = data->data.first().value("DisplayType").toString();
            emit displayRequest(&task);
        } else
            msgRepeater(data->err);
    } else if ( data->action == GET_ALL_ENTITY_DATA1 ) {
        if ( !data->data.isEmpty() ) {
            QUrl url(data->data.first().value("URL", "EMPTY_URL").toString());
            QDesktopServices::openUrl(url);
        } else
            msgRepeater(data->err);
    } else if ( data->action < GET_XML_DESCRIPTION ) {
        if ( !data->msg.isEmpty() ) {
            QString msg = data->msg.join(" ");
            msgRepeater(msg);
        };
        if ( data->result ) {
            reloadState();
            /*
            switch (data->action) {
            case DESTROY_ENTITY:
                // don't use because implemented domainEnd signal
                break;
            default:
                break;
            }
            */
        };
    };
}

/* private slots */
void VirtDomainControl::reloadState()
{
    entityList->setEnabled(false);
    entityList->clearSelection();
    TASK task;
    task.type       = VIRT_DOMAIN;
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    task.action     = GET_ALL_ENTITY_STATE;
    task.method     = reloadEntity;
    emit addNewTask(&task);
}
void VirtDomainControl::changeDockVisibility()
{
    toolBar->setEnabled( !toolBar->isEnabled() );
    entityList->setEnabled( !entityList->isEnabled() );
}
void VirtDomainControl::entityClicked(const QPoint &p)
{
    //qDebug()<<"custom Menu request";
    QModelIndex idx = entityList->indexAt(p);
    QVariantMap params;
    if ( idx.isValid() && domainModel->DataList.count()>idx.row() ) {
        //qDebug()<<domainModel->DataList.at(idx.row())->getName();
        params.insert(
                    "name",
                    domainModel->DataList.at(idx.row())->getName());
        params.insert(
                    "active",
                    domainModel->DataList.at(idx.row())->getState());
        params.insert(
                    "auto",
                    domainModel->DataList.at(idx.row())->getAutostart());
        params.insert(
                    "persistent",
                    domainModel->DataList.at(idx.row())->getPersistent());
    } else {
        entityList->clearSelection();
    };
    bool state = toolBar->getAutoReloadState();
    DomainControlMenu *domControlMenu =
            new DomainControlMenu(this, params, state);
    connect(domControlMenu, SIGNAL(execMethod(const Act_Param&)),
            this, SLOT(execAction(const Act_Param&)));
    domControlMenu->move(QCursor::pos());
    domControlMenu->exec();
    disconnect(domControlMenu, SIGNAL(execMethod(const Act_Param&)),
               this, SLOT(execAction(const Act_Param&)));
    domControlMenu->deleteLater();
}
void VirtDomainControl::entityDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        QString _domainName = domainModel->DataList.at(index.row())->getName();
        //emit addToStateMonitor(ptr_ConnPtr, currConnName, _domainName);
        //emit displayRequest(ptr_ConnPtr, currConnName, _domainName);
        TASK task;
        task.type       = VIRT_DOMAIN;
        task.srcConnPtr = ptr_ConnPtr;
        task.srcConName = currConnName;
        task.object     = _domainName;
        task.method     = displayVirtDomain;
        task.action     = GET_ALL_ENTITY_DATA0;
        emit addNewTask(&task);
    }
}
void VirtDomainControl::execAction(const Act_Param &param)
{
    QModelIndex idx = entityList->currentIndex();
    if ( idx.isValid() && domainModel->DataList.count()>idx.row() ) {
        QString domainName = domainModel->DataList.at(idx.row())->getName();
        TASK task;
        task.type       = VIRT_DOMAIN;
        task.srcConnPtr = ptr_ConnPtr;
        task.srcConName = currConnName;
        task.object     = domainName;
        task.method     = param.method;
        if        ( param.method==startEntity ) {
            task.action     = START_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==pauseEntity ) {
            task.action     = PAUSE_ENTITY;
            task.args.state = domainModel->DataList
                    .at(idx.row())->getState_EXT();
            emit addNewTask(&task);
        } else if ( param.method==destroyEntity ) {
            task.action     = DESTROY_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==editEntity ) {
            task.action     = EDIT_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==resetVirtDomain ) {
            task.action     = RESET_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==rebootVirtDomain ) {
            task.action     = REBOOT_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==shutdownVirtDomain ) {
            task.action     = SHUTDOWN_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==saveVirtDomain ) {
            QString to =
                    QFileDialog::getSaveFileName(
                        this, "Save to", "~");
            if ( !to.isEmpty() ) {
                task.action     = SAVE_ENTITY;
                task.args.path  = to;
                task.args.state = domainModel->DataList
                        .at(idx.row())->getState_EXT();
                emit addNewTask(&task);
            };
        } else if ( param.method==restoreVirtDomain ) {
            QString from =
                    QFileDialog::getOpenFileName(
                        this, "Restore from", "~");
            if ( !from.isEmpty() ) {
                task.action     = RESTORE_ENTITY;
                task.args.path  = from;
                emit addNewTask(&task);
            };
        } else if ( param.method==undefineEntity ) {
            task.action     = UNDEFINE_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==setAutostartEntity ) {
            /* set the opposite value */
            uint autostartState =
                (domainModel->DataList.at(idx.row())->getAutostart())
                 ? 0 : 1;
            task.action     = CHANGE_ENTITY_AUTOSTART;
            task.args.sign  = autostartState;
            emit addNewTask(&task);
        } else if ( param.method==migrateVirtDomain ) {
            if ( nullptr!=ptr_ConnPtr && nullptr!=*ptr_ConnPtr ) {
                MigrateDialog *migrateDialog =
                        new MigrateDialog(
                            this, domainName, ptr_ConnPtr);
                int exitCode = migrateDialog->exec();
                MIGR_ARGS migrArgs = migrateDialog->getMigrateArgs();
                migrateDialog->deleteLater();
                task.action      = MIGRATE_ENTITY;
                task.args.sign   = migrArgs.flags;
                task.args.object = migrArgs.new_name;
                task.args.size   = migrArgs.bandwidth;
                task.args.offset = migrArgs.maxDownTime;
                //qDebug()<<exitCode<<migrArgs.new_name
                //       <<migrArgs.connName<<task.object;
                if ( exitCode ) {
                    if ( migrArgs.connName.isEmpty() ) {
                        // migrate useing specified URI
                        task.args.path = migrArgs.uri;
                        emit addNewTask(&task);
                    } else {
                        // migrate useing specified connect
                        task.args.path = migrArgs.connName;
                        emit migrateToConnect(&task);
                    };
                };
            } else
                emit ptrIsNull();
        } else if ( param.method==getEntityXMLDesc ) {
            task.action     = GET_XML_DESCRIPTION;
            task.args.sign  = (param.context==DO_AsIs)
                    ? 0:VIR_DOMAIN_XML_INACTIVE;
            emit addNewTask(&task);
        } else if ( param.method==displayVirtDomain ) {
            task.action     = GET_ALL_ENTITY_DATA0;
            emit addNewTask(&task);
        } else if ( param.method==displayVirtDomainInExternalViewer ) {
            task.action     = GET_ALL_ENTITY_DATA1;
            emit addNewTask(&task);
        } else if ( param.method==monitorVirtDomain ) {
            // send signal with Connection & Domain Names
            // to add into Domain State Monitor
            emit addToStateMonitor(ptr_ConnPtr, currConnName, domainName);
        } else if ( param.method==reloadEntity ) {
            reloadState();
        } else if ( param.method==createVirtDomainSnapshot ) {
            //qDebug()<<"createVirtDomainSnapshot";
            bool state = domainModel->DataList
                    .at(idx.row())->getState();
            CreateSnapshotDialog *_dialog =
                    new CreateSnapshotDialog(
                        this, domainName, currConnName, state, ptr_ConnPtr);
            connect(_dialog, SIGNAL(errMsg(const QString&)),
                    this, SLOT(msgRepeater(const QString&)));
            int exitCode = _dialog->exec();
            disconnect(_dialog, SIGNAL(errMsg(const QString&)),
                       this, SLOT(msgRepeater(const QString&)));
            _dialog->deleteLater();
            if ( exitCode ) {
                task.action      = CREATE_DOMAIN_SNAPSHOT;
                task.args.object = _dialog->getSnapshotXMLDesc();
                task.args.sign   = _dialog->getSnapshotFlags();
                emit addNewTask(&task);
            };
        } else if ( param.method==moreSnapshotActions ) {
            //qDebug()<<"moreSnapshotActions";
            SnapshotActionDialog *_dialog =
                    new SnapshotActionDialog(
                        this, ptr_ConnPtr, domainName, currConnName);
            int exitCode = _dialog->exec();
            _dialog->deleteLater();
            if ( exitCode ) {
                Act_Param params = _dialog->getParameters();
                task.action      = params.act;
                Methods method;
                if ( params.act==REVERT_TO_DOMAIN_SNAPSHOT ) {
                    method = revertVirtDomainSnapshot;
                } else if ( params.act==DELETE_DOMAIN_SNAPSHOT ) {
                    method = deleteVirtDomainSnapshot;
                };
                task.method      = method;
                task.args.object = params.path;
                task.args.sign   = _dialog->getSnapshotFlags();
                emit addNewTask(&task);
            };
        };
    } else if ( param.method==reloadEntity ) {
        reloadState();
    };
}
void VirtDomainControl::newVirtEntityFromXML(const Act_Param &args)
{
    TASK task;
    task.type = VIRT_DOMAIN;
    Methods method;
    if ( args.act==CREATE_ENTITY ) {
        method = createEntity;
    } else {
        method = defineEntity;
    };
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    task.method     = method;
    task.action     = args.act;
    task.args.path  = args.path;
    if ( args.context==DO_AsIs ) {
        emit addNewTask(&task);
    } else {
        emit domainToEditor(&task);
    };
}
