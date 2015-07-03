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
    disconnect(entityList, SIGNAL(doubleClicked(const QModelIndex&)),
               this, SLOT(entityDoubleClicked(const QModelIndex&)));
    disconnect(entityList, SIGNAL(customContextMenuRequested(const QPoint&)),
               this, SLOT(entityClicked(const QPoint&)));
    disconnect(toolBar, SIGNAL(fileForMethod(const QStringList&)),
               this, SLOT(newVirtEntityFromXML(const QStringList&)));
    disconnect(toolBar, SIGNAL(execMethod(const QStringList&)),
               this, SLOT(execAction(const QStringList&)));

    if ( createVirtDomain!=NULL ) {
        delete createVirtDomain;
        createVirtDomain = NULL;
    };

    stopProcessing();

    delete toolBar;
    toolBar = NULL;

    if (domainModel!=NULL) {
        delete domainModel;
        domainModel = NULL;
    };

    if (entityList!=NULL) {
        delete entityList;
        entityList = NULL;
    };
}

/* public slots */
bool VirtDomainControl::getThreadState() const
{
    return true;
}
void VirtDomainControl::stopProcessing()
{
    // clear Domain list
    while ( domainModel->DataList.count() ) {
        domainModel->removeRow(0);
    };
    domainModel->setHeaderData(0, Qt::Horizontal, QString("Name"), Qt::EditRole);
    setEnabled(false);
}
bool VirtDomainControl::setCurrentWorkConnect(virConnect *conn)
{
    stopProcessing();
    currWorkConnection = conn;
    toolBar->enableAutoReload();
    return true;
}
void VirtDomainControl::setListHeader(QString &connName)
{
    domainModel->setHeaderData(0, Qt::Horizontal, QString("Name (Conn: \"%1\")")
                               .arg(connName), Qt::EditRole);
    currConnName = connName;
    setEnabled(true);
    reloadState();
}
virConnect* VirtDomainControl::getConnection() const
{
    return currWorkConnection;
}
void VirtDomainControl::execMigrateAction(virConnectPtr conn, QStringList &args)
{
    args.prepend(QString::number(MIGRATE_ENTITY));
    args.prepend(currConnName);
    emit addNewTask(currWorkConnection, args, conn);
    TASK task;
    task.type = "domain";
    task.sourceConn = currWorkConnection;
    task.srcConName = currConnName;
    task.action     = MIGRATE_ENTITY;
    task.method     = "migrateVirtDomain";
    task.ARGS.destConn = conn;
    emit addNewTask(task);
}
void VirtDomainControl::resultReceiver(Result data)
{
    //qDebug()<<data.number<<data.action<<data.msg<<"result";
    if ( data.action == GET_ALL_ENTITY ) {
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
            QString xml = data.msg.first();
            // show SRC Creator widget in Edit-mode
            createVirtDomain = new CreateVirtDomain(
                        this,
                        currWorkConnection,
                        xml);
            connect(createVirtDomain, SIGNAL(errorMsg(QString&)),
                    this, SLOT(msgRepeater(QString&)));
            int result = createVirtDomain->exec();
            if ( createVirtDomain!=NULL && result ) {
                // get path for method
                xml = createVirtDomain->getXMLDescFileName();
                bool show = createVirtDomain->getShowing();
                QStringList data;
                data.append("Edited Domain XML'ed");
                data.append(QString("to <a href='%1'>%1</a>").arg(xml));
                QString msg = data.join(" ");
                msgRepeater(msg);
                if ( show ) QDesktopServices::openUrl(QUrl(xml));
                QStringList args;
                args.append(xml);
                args.prepend("defineVirtDomain");
                args.prepend(QString::number(DEFINE_ENTITY));
                args.prepend(currConnName);
                emit addNewTask(currWorkConnection, args);
            };
            disconnect(createVirtDomain, SIGNAL(errorMsg(QString&)),
                       this, SLOT(msgRepeater(QString&)));
            delete createVirtDomain;
            createVirtDomain = NULL;
        };
    } else if ( data.action < GET_XML_DESCRIPTION ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
        };
        if ( data.result ) {
            reloadState();
            switch (data.action) {
            case DESTROY_ENTITY:
                emit domainClosed(currConnName, data.name);
                break;
            default:
                break;
            }
        };
    };
}

/* private slots */
void VirtDomainControl::reloadState()
{
    TASK task;
    task.type = "domain";
    task.sourceConn = currWorkConnection;
    task.srcConName = currConnName;
    task.action     = GET_ALL_ENTITY;
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
        emit addToStateMonitor(currWorkConnection, currConnName, _domainName);
    }
}
void VirtDomainControl::execAction(const QStringList &l)
{
    QModelIndex idx = entityList->currentIndex();
    if ( idx.isValid() && domainModel->DataList.count()>idx.row() ) {
        QString domainName = domainModel->DataList.at(idx.row())->getName();
        TASK task;
        task.type = "domain";
        task.sourceConn = currWorkConnection;
        task.srcConName = currConnName;
        task.object     = domainName;
        if        ( l.first()=="startVirtDomain" ) {
            task.method     = l.first();
            task.action     = START_ENTITY;
            emit addNewTask(task);
        } else if ( l.first()=="pauseVirtDomain" ) {
            args.append(domainModel->DataList.at(idx.row())->getState().split(":").last());
            args.prepend(l.first());
            args.prepend(QString::number(PAUSE_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnection, args);
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
                task.ARGS.path  = to;
                task.ARGS.sign  = 0;
                //args.append(domainModel->DataList.at(idx.row())->getState().split(":").last());
                emit addNewTask(task);
            };
        } else if ( l.first()=="restoreVirtDomain" ) {
            QString from = QFileDialog::getOpenFileName(this, "Restore from", "~");
            if ( !from.isEmpty() ) {
                args.append(from);
                args.prepend(l.first());
                args.prepend(QString::number(RESTORE_ENTITY));
                args.prepend(currConnName);
                emit addNewTask(currWorkConnection, args);
            };
        } else if ( l.first()=="undefineVirtDomain" ) {
            args.prepend(l.first());
            args.prepend(QString::number(UNDEFINE_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnection, args);
        } else if ( l.first()=="setAutostartVirtDomain" ) {
            /* set the opposite value */
            uint autostartState =
                (domainModel->DataList.at(idx.row())->getAutostart()=="yes")
                 ? 0 : 1;
            task.method     = l.first();
            task.action     = CHANGE_ENTITY_AUTOSTART;
            task.ARGS.sign  = autostartState;
            emit addNewTask(task);
        } else if ( l.first()=="migrateVirtDomain" ) {
            // set Migrate parameters
            char *hostName = virConnectGetHostname(currWorkConnection);
            const char *connType = virConnectGetType(currWorkConnection);
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
            QStringList migrArgs = migrateDialog->getMigrateArgs();
            migrateDialog->deleteLater();
            if ( exitCode ) {
                foreach (QString arg, migrArgs) {
                    args.append(arg);
                };
                if ( migrArgs.first().isEmpty() ) {
                    // migrate useing specified URI
                    args.prepend(l.first());
                    args.prepend(QString::number(MIGRATE_ENTITY));
                    args.prepend(currConnName);
                    emit addNewTask(currWorkConnection, args);
                } else {
                    // migrate useing specified connect
                    emit migrateToConnect(args);
                };
            };
        } else if ( l.first()=="getVirtDomainXMLDesc" ) {
            task.method     = l.first();
            task.action     = GET_XML_DESCRIPTION;
            emit addNewTask(task);
        } else if ( l.first()=="displayVirtDomain" ) {
            // send signal with Connection & Domain Names to call VM_Viewer into MainWindow widget
            emit displayRequest(currWorkConnection, currConnName, domainName);
        } else if ( l.first()=="monitorVirtDomain" ) {
            // send signal with Connection & Domain Names to add into Domain State Monitor
            emit addToStateMonitor(currWorkConnection, currConnName, domainName);
        } else if ( l.first()=="reloadVirtDomain" ) {
            reloadState();
        } else if ( l.first()=="createVirtDomainSnapshot" ) {
            //qDebug()<<"createVirtDomainSnapshot";
            bool state = domainModel->DataList
                    .at(idx.row())->getState().startsWith("active");
            CreateSnapshotDialog *_dialog =
                    new CreateSnapshotDialog(
                        this, domainName, state, currWorkConnection);
            connect(_dialog, SIGNAL(errMsg(QString&)),
                    this, SLOT(msgRepeater(QString&)));
            int exitCode = _dialog->exec();
            if ( exitCode ) {
                args.append(_dialog->getSnapshotFlags());
                args.append(_dialog->getSnapshotXMLDesc());
                args.prepend(l.first());
                args.prepend(QString::number(CREATE_DOMAIN_SNAPSHOT));
                args.prepend(currConnName);
                emit addNewTask(currWorkConnection, args);
            };
            disconnect(_dialog, SIGNAL(errMsg(QString&)),
                       this, SLOT(msgRepeater(QString&)));
            _dialog->deleteLater();
        } else if ( l.first()=="moreSnapshotActions" ) {
            //qDebug()<<"moreSnapshotActions";
            SnapshotActionDialog *_dialog =
                    new SnapshotActionDialog(this, currWorkConnection, domainName);
            int exitCode = _dialog->exec();
            if ( exitCode ) {
                // add parameters
                QStringList params = _dialog->getParameters();
                args.prepend(params.first());
                params.removeFirst();
                args.append(params);
                args.prepend(QString::number(exitCode));
                args.prepend(currConnName);
                emit addNewTask(currWorkConnection, args);
            };
            _dialog->deleteLater();
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
            QString xml;
            if ( args.first()=="manually" ) {
                // show SRC Creator widget
                createVirtDomain = new CreateVirtDomain(this, currWorkConnection);
                connect(createVirtDomain, SIGNAL(errorMsg(QString&)),
                        this, SLOT(msgRepeater(QString&)));
                int result = createVirtDomain->exec();
                if ( createVirtDomain!=NULL && result ) {
                    // get path for method
                    xml = createVirtDomain->getXMLDescFileName();
                    bool show = createVirtDomain->getShowing();
                    QStringList data;
                    data.append("New Domain XML'ed");
                    data.append(QString("to <a href='%1'>%1</a>").arg(xml));
                    QString msg = data.join(" ");
                    msgRepeater(msg);
                    if ( show ) QDesktopServices::openUrl(QUrl(xml));
                };
                disconnect(createVirtDomain, SIGNAL(errorMsg(QString&)),
                           this, SLOT(msgRepeater(QString&)));
                delete createVirtDomain;
                createVirtDomain = NULL;
                //qDebug()<<xml<<"path"<<result;
                task.ARGS.path = xml;
            } else {
                xml = args.first();
                task.ARGS.path = xml;
            };
            task.sourceConn = currWorkConnection;
            task.srcConName = currConnName;
            task.method     = actName;
            task.action     = act;
            emit addNewTask(task);
        };
    };
}
