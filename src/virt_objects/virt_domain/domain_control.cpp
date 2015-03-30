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
    if ( currWorkConnect!=NULL ) {
        virConnectClose(currWorkConnect);
        currWorkConnect = NULL;
    };

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
    currWorkConnect = conn;
    int ret = virConnectRef(currWorkConnect);
    if ( ret<0 ) {
        virErrorPtr virtErrors = virGetLastError();
        if ( virtErrors!=NULL && virtErrors->code>0 ) {
            QString time = QTime::currentTime().toString();
            QString msg = QString("%3 VirtError(%1) : %2")
                    .arg(virtErrors->code)
                    .arg(virtErrors->message)
                    .arg(time);
            emit entityMsg( msg );
            virResetError(virtErrors);
        };
        currWorkConnect = NULL;
        return false;
    } else {
        toolBar->enableAutoReload();
        return true;
    };
}
void VirtDomainControl::setListHeader(QString &connName)
{
    domainModel->setHeaderData(0, Qt::Horizontal, QString("Name (Conn: \"%1\")")
                               .arg(connName), Qt::EditRole);
    currConnName = connName;
    setEnabled(true);
    reloadDomainState();
}
virConnect* VirtDomainControl::getConnect() const
{
    return currWorkConnect;
}
void VirtDomainControl::execMigrateAction(virConnectPtr conn, QStringList &args)
{
    args.prepend(QString::number(MIGRATE_ENTITY));
    args.prepend(currConnName);
    emit addNewTask(currWorkConnect, args, conn);
}
void VirtDomainControl::reloadDomainState()
{
    QStringList args;
    args.append("..");
    args.prepend("reloadVirtDomain");
    args.prepend(QString::number(GET_ALL_ENTITY));
    args.prepend(currConnName);
    emit addNewTask(currWorkConnect, args);
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
            QStringList chain = _data.split(" ");
            if (chain.isEmpty()) continue;
            int count = chain.size();
            for (int j=0; j<count; j++) {
                domainModel->setData(domainModel->index(i,j), chain.at(j), Qt::EditRole);
            };
            i++;
        };
    } else if ( data.action == GET_XML_DESCRIPTION ) {
        if ( !data.msg.isEmpty() ) {
            QString xml = data.msg.first();
            data.msg.removeFirst();
            data.msg.append(QString("to <a href='%1'>%1</a>").arg(xml));
            msgRepeater(data.msg.join(" "));
            QDesktopServices::openUrl(QUrl(xml));
        };
    } else if ( data.action == EDIT_ENTITY ) {
        if ( !data.msg.isEmpty() ) {
            QString xml = data.msg.first();
            // show SRC Creator widget in Edit-mode
            createVirtDomain = new CreateVirtDomain(
                        this,
                        currWorkConnect,
                        xml);
            connect(createVirtDomain,
                    SIGNAL(errorMsg(QString)),
                    this,
                    SLOT(msgRepeater(QString)));
            int result = createVirtDomain->exec();
            if ( createVirtDomain!=NULL && result ) {
                // get path for method
                xml = createVirtDomain->getXMLDescFileName();
                bool show = createVirtDomain->getShowing();
                QStringList data;
                data.append("Edited Domain XML'ed");
                data.append(QString("to <a href='%1'>%1</a>").arg(xml));
                msgRepeater(data.join(" "));
                if ( show ) QDesktopServices::openUrl(QUrl(xml));
                QStringList args;
                args.append(xml);
                args.prepend("defineVirtDomain");
                args.prepend(QString::number(DEFINE_ENTITY));
                args.prepend(currConnName);
                emit addNewTask(currWorkConnect, args);
            };
            disconnect(createVirtDomain,
                       SIGNAL(errorMsg(QString)),
                       this,
                       SLOT(msgRepeater(QString)));
            delete createVirtDomain;
            createVirtDomain = NULL;
        };
    } else if ( data.action < GET_XML_DESCRIPTION ) {
        if ( !data.msg.isEmpty() ) msgRepeater(data.msg.join(" "));
        if ( data.result ) {
            reloadDomainState();
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
        emit addToStateMonitor(currWorkConnect, currConnName, _domainName);
    }
}
void VirtDomainControl::execAction(const QStringList &l)
{
    QStringList args;
    QModelIndex idx = entityList->currentIndex();
    if ( idx.isValid() && domainModel->DataList.count()>idx.row() ) {
        QString domainName = domainModel->DataList.at(idx.row())->getName();
        args.append(domainName);
        if        ( l.first()=="startVirtDomain" ) {
            args.prepend(l.first());
            args.prepend(QString::number(START_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="pauseVirtDomain" ) {
            args.append(domainModel->DataList.at(idx.row())->getState().split(":").last());
            args.prepend(l.first());
            args.prepend(QString::number(PAUSE_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="destroyVirtDomain" ) {
            args.prepend(l.first());
            args.prepend(QString::number(DESTROY_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="editVirtDomain" ) {
            args.prepend(l.first());
            args.prepend(QString::number(EDIT_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="resetVirtDomain" ) {
            args.prepend(l.first());
            args.prepend(QString::number(RESET_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="rebootVirtDomain" ) {
            args.prepend(l.first());
            args.prepend(QString::number(REBOOT_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="shutdownVirtDomain" ) {
            args.prepend(l.first());
            args.prepend(QString::number(SHUTDOWN_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="saveVirtDomain" ) {
            QString to = QFileDialog::getSaveFileName(this, "Save to", "~");
            if ( !to.isEmpty() ) {
                args.append(to);
                args.append(domainModel->DataList.at(idx.row())->getState().split(":").last());
                args.prepend(l.first());
                args.prepend(QString::number(SAVE_ENTITY));
                args.prepend(currConnName);
                emit addNewTask(currWorkConnect, args);
            };
        } else if ( l.first()=="restoreVirtDomain" ) {
            QString from = QFileDialog::getOpenFileName(this, "Restore from", "~");
            if ( !from.isEmpty() ) {
                args.append(from);
                args.prepend(l.first());
                args.prepend(QString::number(RESTORE_ENTITY));
                args.prepend(currConnName);
                emit addNewTask(currWorkConnect, args);
            };
        } else if ( l.first()=="undefineVirtDomain" ) {
            args.prepend(l.first());
            args.prepend(QString::number(UNDEFINE_ENTITY));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="setAutostartVirtDomain" ) {
            /* set the opposite value */
            QString autostartState =
                (domainModel->DataList.at(idx.row())->getAutostart()=="yes")
                 ? "0" : "1";
            args.append(autostartState);
            args.prepend(l.first());
            args.prepend(QString::number(CHANGE_ENTITY_AUTOSTART));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="migrateVirtDomain" ) {
            // set Migrate parameters
            char *hostName = virConnectGetHostname(currWorkConnect);
            const char *connType = virConnectGetType(currWorkConnect);
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
            MigrateDialog *migrateDialog = new MigrateDialog(this, domainName, hostName, connType, list);
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
                    emit addNewTask(currWorkConnect, args);
                } else {
                    // migrate useing specified connect
                    emit migrateToConnect(args);
                };
            };
        } else if ( l.first()=="getVirtDomainXMLDesc" ) {
            args.prepend(l.first());
            args.prepend(QString::number(GET_XML_DESCRIPTION));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        } else if ( l.first()=="displayVirtDomain" ) {
            // send signal with Connect & Domain Names to call VM_Viewer into MainWindow widget
            emit displayRequest(currWorkConnect, currConnName, domainName);
        } else if ( l.first()=="monitorVirtDomain" ) {
            // send signal with Connect & Domain Names to add into Domain State Monitor
            emit addToStateMonitor(currWorkConnect, currConnName, domainName);
        } else if ( l.first()=="reloadVirtDomain" ) {
            reloadDomainState();
        } else if ( l.first()=="createVirtDomainSnapshot" ) {
            //qDebug()<<"createVirtDomainSnapshot";
            bool state = domainModel->DataList
                    .at(idx.row())->getState().startsWith("active");
            CreateSnapshotDialog *_dialog =
                    new CreateSnapshotDialog(this, domainName, state);
            int exitCode = _dialog->exec();
            if ( exitCode ) {
                args.append(_dialog->getSnapshotFlags());
                args.append(_dialog->getSnapshotXMLDesc());
                args.prepend(l.first());
                args.prepend(QString::number(DOMAIN_SNAPSHOT));
                args.prepend(currConnName);
                emit addNewTask(currWorkConnect, args);
            };
            _dialog->deleteLater();
        } else if ( l.first()=="moreSnapshotActions" ) {
            //qDebug()<<"moreSnapshotActions";
            SnapshotActionDialog *_dialog =
                    new SnapshotActionDialog(this, currWorkConnect, domainName);
            int exitCode = _dialog->exec();
            if ( exitCode ) {
                // add parameters
                args.prepend(l.first());
                //args.prepend(QString::number(DOMAIN_SNAPSHOT));
                args.prepend(currConnName);
                //emit addNewTask(currWorkConnect, args);
            };
            _dialog->deleteLater();
        };
    } else if ( l.first()=="reloadVirtDomain" ) {
        reloadDomainState();
    };
}
void VirtDomainControl::newVirtEntityFromXML(const QStringList &_args)
{
    Actions act;
    if ( !_args.isEmpty() ) {
        if ( _args.first().startsWith("create") ) act = CREATE_ENTITY;
        else act = DEFINE_ENTITY;
        QStringList args = _args;
        args.removeFirst();
        if ( !args.isEmpty() ) {
            if ( args.first()=="manually" ) {
                args.removeFirst();
                //QString source = args.first();
                args.removeFirst();
                QString xml;
                // show SRC Creator widget
                createVirtDomain = new CreateVirtDomain(this, currWorkConnect);
                connect(createVirtDomain,
                        SIGNAL(errorMsg(QString)),
                        this,
                        SLOT(msgRepeater(QString)));
                int result = createVirtDomain->exec();
                if ( createVirtDomain!=NULL && result ) {
                    // get path for method
                    xml = createVirtDomain->getXMLDescFileName();
                    bool show = createVirtDomain->getShowing();
                    QStringList data;
                    data.append("New Domain XML'ed");
                    data.append(QString("to <a href='%1'>%1</a>").arg(xml));
                    msgRepeater(data.join(" "));
                    if ( show ) QDesktopServices::openUrl(QUrl(xml));
                };
                disconnect(createVirtDomain,
                           SIGNAL(errorMsg(QString)),
                           this,
                           SLOT(msgRepeater(QString)));
                delete createVirtDomain;
                createVirtDomain = NULL;
                //qDebug()<<xml<<"path"<<result;
                args.prepend(xml);
            };
            if ( act==CREATE_ENTITY ) {
                args.prepend("createVirtDomain");
            } else {
                args.prepend("defineVirtDomain");
            };
            args.prepend(QString::number(act));
            args.prepend(currConnName);
            emit addNewTask(currWorkConnect, args);
        };
    };
}
