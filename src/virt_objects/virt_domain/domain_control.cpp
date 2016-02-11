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
    settings.beginGroup("VirtDomainControl");
    entityList->setColumnWidth(0, settings.value("column0", 132).toInt());
    entityList->setColumnWidth(1, settings.value("column1", 32).toInt());
    entityList->setColumnWidth(2, settings.value("column2", 32).toInt());
    entityList->setColumnWidth(3, settings.value("column3", 32).toInt());
    int area_int = settings.value("ToolBarArea", 4).toInt();
    settings.endGroup();
    toolBar = new DomainToolBar(this);
    addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
    connect(toolBar, SIGNAL(fileForMethod(const QStringList&)),
            this, SLOT(newVirtEntityFromXML(const QStringList&)));
    connect(toolBar, SIGNAL(execMethod(const QStringList&)),
            this, SLOT(execAction(const QStringList&)));
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
    domainModel->setHeaderData(0, Qt::Horizontal, QString("Name"), Qt::EditRole);
}
bool VirtDomainControl::setCurrentWorkConnect(virConnectPtr *connPtrPtr)
{
    stopProcessing();
    ptr_ConnPtr = connPtrPtr;
    toolBar->enableAutoReload();
    return true;
}
void VirtDomainControl::setListHeader(QString &connName)
{
    domainModel->setHeaderData(
                0,
                Qt::Horizontal,
                QString("Name in [ %1 ]").arg(connName),
                Qt::EditRole);
    currConnName = connName;
    setEnabled(true);
    reloadState();
}
virConnectPtr* VirtDomainControl::getPtr_connectionPtr()
{
    return ptr_ConnPtr;
}
void VirtDomainControl::execMigrateAction(virConnectPtr *conn, TASK task)
{
    task.args.destConnPtr = conn;
    task.args.path.clear();
    emit addNewTask(task);
}
void VirtDomainControl::resultReceiver(Result data)
{
    //qDebug()<<data.number<<data.action<<data.msg<<"result";
    if ( data.action == GET_ALL_ENTITY_STATE ) {
        if ( data.msg.count() > domainModel->DataList.count() ) {
            int _diff = data.msg.count() - domainModel->DataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                domainModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( domainModel->DataList.count() > data.msg.count() ) {
            int _diff = domainModel->DataList.count() - data.msg.count();
            for ( int i = 0; i<_diff; i++ ) {
                domainModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QString _data, data.msg) {
            QStringList chain = _data.split(DFR);
            if (chain.isEmpty()) continue;
            int count = chain.size();
            for (int j=0; j<count; j++) {
                domainModel->setData(domainModel->index(i,j), chain.at(j), Qt::EditRole);
            };
            i++;
        };
    } else if ( data.action == GET_XML_DESCRIPTION ) {
        QString xml = data.fileName;
        data.msg.append(QString("to <a href='%1'>%1</a>").arg(xml));
        QString msg = data.msg.join(" ");
        msgRepeater(msg);
        if ( data.result )
            QDesktopServices::openUrl(QUrl(xml));
    } else if ( data.action == EDIT_ENTITY ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
        };
        if ( data.result ) {
            // show SRC Creator widget in Edit-mode
            TASK task;
            task.type = "domain";
            task.srcConnPtr = ptr_ConnPtr;
            task.srcConName = currConnName;
            task.object     = QString("DomainEditor_%1")
                    .arg(QTime::currentTime().toString());
            task.args.path  = data.fileName;
            task.method     = "editVirtDomain";
            task.action     = DEFINE_ENTITY;
            emit domainToEditor(task);
        };
    } else if ( data.action == GET_ALL_ENTITY_DATA ) {
        if ( !data.msg.isEmpty() ) {
            TASK task;
            task.srcConnPtr = ptr_ConnPtr;
            task.srcConName = currConnName;
            task.object     = data.name;
            task.type       = data.msg.first();
            emit displayRequest(task);
        } else
            msgRepeater(data.err);
    } else if ( data.action < GET_XML_DESCRIPTION ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
        };
        if ( data.result ) {
            reloadState();
            /*
            switch (data.action) {
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
    TASK task;
    task.type = "domain";
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    task.action     = GET_ALL_ENTITY_STATE;
    task.method     = "reloadVirtDomain";
    emit addNewTask(task);
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
    QStringList params;
    if ( idx.isValid() && domainModel->DataList.count()>idx.row() ) {
        //qDebug()<<domainModel->DataList.at(idx.row())->getName();
        params<<domainModel->DataList.at(idx.row())->getName();
        params<<domainModel->DataList.at(idx.row())->getState().split(":").first();
        params<<domainModel->DataList.at(idx.row())->getAutostart();
        params<<domainModel->DataList.at(idx.row())->getPersistent();
    } else {
        entityList->clearSelection();
    };
    bool state = toolBar->getAutoReloadState();
    DomainControlMenu *domControlMenu = new DomainControlMenu(this, params, state);
    connect(domControlMenu, SIGNAL(execMethod(const QStringList&)),
            this, SLOT(execAction(const QStringList&)));
    domControlMenu->move(QCursor::pos());
    domControlMenu->exec();
    disconnect(domControlMenu, SIGNAL(execMethod(const QStringList&)),
               this, SLOT(execAction(const QStringList&)));
    domControlMenu->deleteLater();
}
void VirtDomainControl::entityDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        QString _domainName = domainModel->DataList.at(index.row())->getName();
        //emit addToStateMonitor(ptr_ConnPtr, currConnName, _domainName);
        //emit displayRequest(ptr_ConnPtr, currConnName, _domainName);
        TASK task;
        task.type = "domain";
        task.srcConnPtr = ptr_ConnPtr;
        task.srcConName = currConnName;
        task.object     = _domainName;
        task.method     = "displayVirtDomain";
        task.action     = GET_ALL_ENTITY_DATA;
        emit addNewTask(task);
    }
}
void VirtDomainControl::execAction(const QStringList &l)
{
    QModelIndex idx = entityList->currentIndex();
    if ( idx.isValid() && domainModel->DataList.count()>idx.row() ) {
        QString domainName = domainModel->DataList.at(idx.row())->getName();
        TASK task;
        task.type = "domain";
        task.srcConnPtr = ptr_ConnPtr;
        task.srcConName = currConnName;
        task.object     = domainName;
        if        ( l.first()=="startVirtDomain" ) {
            task.method     = l.first();
            task.action     = START_ENTITY;
            emit addNewTask(task);
        } else if ( l.first()=="pauseVirtDomain" ) {
            task.method     = l.first();
            task.action     = PAUSE_ENTITY;
            task.args.state = domainModel->DataList
                    .at(idx.row())->getState().split(":").last();
            emit addNewTask(task);
        } else if ( l.first()=="destroyVirtDomain" ) {
            task.method     = l.first();
            task.action     = DESTROY_ENTITY;
            emit addNewTask(task);
        } else if ( l.first()=="editVirtDomain" ) {
            task.method     = l.first();
            task.action     = EDIT_ENTITY;
            emit addNewTask(task);
        } else if ( l.first()=="resetVirtDomain" ) {
            task.method     = l.first();
            task.action     = RESET_ENTITY;
            emit addNewTask(task);
        } else if ( l.first()=="rebootVirtDomain" ) {
            task.method     = l.first();
            task.action     = REBOOT_ENTITY;
            emit addNewTask(task);
        } else if ( l.first()=="shutdownVirtDomain" ) {
            task.method     = l.first();
            task.action     = SHUTDOWN_ENTITY;
            emit addNewTask(task);
        } else if ( l.first()=="saveVirtDomain" ) {
            QString to = QFileDialog::getSaveFileName(this, "Save to", "~");
            if ( !to.isEmpty() ) {
                task.method     = l.first();
                task.action     = SAVE_ENTITY;
                task.args.path  = to;
                task.args.state = domainModel->DataList
                        .at(idx.row())->getState().split(":").last();
                emit addNewTask(task);
            };
        } else if ( l.first()=="restoreVirtDomain" ) {
            QString from = QFileDialog::getOpenFileName(this, "Restore from", "~");
            if ( !from.isEmpty() ) {
                task.method     = l.first();
                task.action     = RESTORE_ENTITY;
                task.args.path  = from;
                emit addNewTask(task);
            };
        } else if ( l.first()=="undefineVirtDomain" ) {
            task.method     = l.first();
            task.action     = UNDEFINE_ENTITY;
            emit addNewTask(task);
        } else if ( l.first()=="setAutostartVirtDomain" ) {
            /* set the opposite value */
            uint autostartState =
                (domainModel->DataList.at(idx.row())->getAutostart()=="yes")
                 ? 0 : 1;
            task.method     = l.first();
            task.action     = CHANGE_ENTITY_AUTOSTART;
            task.args.sign  = autostartState;
            emit addNewTask(task);
        } else if ( l.first()=="migrateVirtDomain" ) {
            if ( NULL!=ptr_ConnPtr && NULL!=*ptr_ConnPtr ) {
                // set Migrate parameters
                // implement in thread or in MigrateDialog  VVV
                char *hostName = virConnectGetHostname(*ptr_ConnPtr);
                const char *connType = virConnectGetType(*ptr_ConnPtr);
                //                                          ^^^
                QStringList list;
                settings.beginGroup("Connects");
                foreach (QString conn, settings.childGroups()) {
                    settings.beginGroup(conn);
                    list.append(QString("%1\t(%2)")
                                .arg(conn)
                                .arg(settings.value("Driver").toString()));
                    settings.endGroup();
                };
                settings.endGroup();
                MigrateDialog *migrateDialog = new MigrateDialog(
                            this, domainName, hostName, connType, list);
                int exitCode = migrateDialog->exec();
                MIGR_ARGS migrArgs = migrateDialog->getMigrateArgs();
                migrateDialog->deleteLater();
                task.action      = MIGRATE_ENTITY;
                task.method      = l.first();
                task.args.sign   = migrArgs.flags;
                task.args.object = migrArgs.new_name;
                task.args.size   = migrArgs.bandwidth;
                task.args.offset = migrArgs.maxDownTime;
                if ( exitCode ) {
                    if ( migrArgs.connName.isEmpty() ) {
                        // migrate useing specified URI
                        task.args.path = migrArgs.uri;
                        emit addNewTask(task);
                    } else {
                        // migrate useing specified connect
                        task.args.path = migrArgs.connName;
                        emit migrateToConnect(task);
                    };
                };
            } else
                emit ptrIsNull();
        } else if ( l.first()=="getVirtDomainXMLDesc" ) {
            task.method     = l.first();
            task.action     = GET_XML_DESCRIPTION;
            task.args.sign  = (l.at(1)=="AS_IS")? 0:VIR_DOMAIN_XML_INACTIVE;
            emit addNewTask(task);
        } else if ( l.first()=="displayVirtDomain" ) {
            // send signal with Connection & Domain Names
            // to call VM_Viewer into MainWindow widget
            // emit displayRequest(ptr_ConnPtr, currConnName, domainName);
            task.method     = l.first();
            task.action     = GET_ALL_ENTITY_DATA;
            emit addNewTask(task);
        } else if ( l.first()=="monitorVirtDomain" ) {
            // send signal with Connection & Domain Names
            // to add into Domain State Monitor
            emit addToStateMonitor(ptr_ConnPtr, currConnName, domainName);
        } else if ( l.first()=="reloadVirtDomain" ) {
            reloadState();
        } else if ( l.first()=="createVirtDomainSnapshot" ) {
            //qDebug()<<"createVirtDomainSnapshot";
            bool state = domainModel->DataList
                    .at(idx.row())->getState().startsWith("active");
            CreateSnapshotDialog *_dialog =
                    new CreateSnapshotDialog(
                        this, domainName, state, ptr_ConnPtr);
            connect(_dialog, SIGNAL(errMsg(QString&)),
                    this, SLOT(msgRepeater(QString&)));
            int exitCode = _dialog->exec();
            disconnect(_dialog, SIGNAL(errMsg(QString&)),
                       this, SLOT(msgRepeater(QString&)));
            _dialog->deleteLater();
            if ( exitCode ) {
                task.action      = CREATE_DOMAIN_SNAPSHOT;
                task.method      = "createVirtDomainSnapshot";
                task.args.object = _dialog->getSnapshotXMLDesc();
                task.args.sign   = _dialog->getSnapshotFlags();
                emit addNewTask(task);
            };
        } else if ( l.first()=="moreSnapshotActions" ) {
            //qDebug()<<"moreSnapshotActions";
            SnapshotActionDialog *_dialog =
                    new SnapshotActionDialog(this, ptr_ConnPtr, domainName);
            int exitCode = _dialog->exec();
            _dialog->deleteLater();
            if ( exitCode ) {
                QStringList params = _dialog->getParameters();
                task.action      = static_cast<Actions>(exitCode);
                task.method      = params.first();
                params.removeFirst();
                task.args.object = params.first();
                task.args.sign   = _dialog->getSnapshotFlags();
                emit addNewTask(task);
            };
        };
    } else if ( l.first()=="reloadVirtDomain" ) {
        reloadState();
    };
}
void VirtDomainControl::newVirtEntityFromXML(const QStringList &_args)
{
    QStringList args = _args;
    if ( !args.isEmpty() ) {
        TASK task;
        task.type = "domain";
        Actions act;
        QString actName;
        if ( args.first().startsWith("create") ) {
            act = CREATE_ENTITY;
            actName = "createVirtDomain";
        } else {
            act = DEFINE_ENTITY;
            actName = "defineVirtDomain";
        };
        args.removeFirst();
        if ( !args.isEmpty() ) {
            if ( args.first()=="manually" ) {
                // show SRC Creator widget
                task.srcConnPtr = ptr_ConnPtr;
                task.srcConName = currConnName;
                task.object     = QString("DomainEditor_%1")
                        .arg(QTime::currentTime().toString());
                task.method     = actName;
                task.action     = act;
                emit domainToEditor(task);
            } else {
                task.args.path  = args.first();
                task.srcConnPtr = ptr_ConnPtr;
                task.srcConName = currConnName;
                task.method     = actName;
                task.action     = act;
                emit addNewTask(task);
            };
        };
    };
}
