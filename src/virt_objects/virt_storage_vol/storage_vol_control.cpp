#include "storage_vol_control.h"

VirtStorageVolControl::VirtStorageVolControl(QWidget *parent) :
    QMainWindow(parent)
{
    setObjectName("VirtStorageVolControl");
    setWindowTitle("StorageVol Control");
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    setWindowIcon(QIcon::fromTheme("storageVol"));
    storageVolModel = new StorageVolModel();
    storageVolList = new QTreeView(this);
    storageVolList->setItemsExpandable(false);
    storageVolList->setRootIsDecorated(false);
    storageVolList->setModel(storageVolModel);
    storageVolList->setFocus();
    storageVolList->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(storageVolList, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(storageVolDoubleClicked(const QModelIndex&)));
    connect(storageVolList, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(storageVolClicked(const QPoint&)));
    setCentralWidget(storageVolList);
    settings.beginGroup("VirtStorageVolControl");
    storageVolList->setColumnWidth(0, settings.value("column0", 132).toInt());
    storageVolList->setColumnWidth(1, settings.value("column1", 32).toInt());
    storageVolList->setColumnWidth(2, settings.value("column2", 32).toInt());
    storageVolList->setColumnWidth(3, settings.value("column3", 32).toInt());
    int area_int = settings.value("ToolBarArea", 4).toInt();
    settings.endGroup();
    toolBar = new StorageVolToolBar(this);
    addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
    connect(toolBar, SIGNAL(fileForMethod(const QStringList&)), this, SLOT(newVirtStorageVolFromXML(const QStringList&)));
    connect(toolBar, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
    stVolControlThread = new StorageVolControlThread(this);
    connect(stVolControlThread, SIGNAL(started()), this, SLOT(changeDockVisibility()));
    connect(stVolControlThread, SIGNAL(finished()), this, SLOT(changeDockVisibility()));
    connect(stVolControlThread, SIGNAL(resultData(StorageVolActions, QStringList)), this, SLOT(resultReceiver(StorageVolActions, QStringList)));
    connect(stVolControlThread, SIGNAL(errorMsg(QString)), this, SLOT(msgRepeater(QString)));
}
VirtStorageVolControl::~VirtStorageVolControl()
{
    settings.beginGroup("VirtStorageVolControl");
    settings.setValue("column0", storageVolList->columnWidth(0));
    settings.setValue("column1", storageVolList->columnWidth(1));
    settings.setValue("column2", storageVolList->columnWidth(2));
    settings.setValue("column3", storageVolList->columnWidth(3));
    settings.setValue("ToolBarArea", toolBarArea(toolBar));
    settings.endGroup();
    settings.sync();
    //disconnect(storageVolList, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(storageVolDoubleClicked(const QModelIndex&)));
    disconnect(storageVolList, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(storageVolClicked(const QPoint&)));
    disconnect(toolBar, SIGNAL(fileForMethod(const QStringList&)), this, SLOT(newVirtStorageVolFromXML(const QStringList&)));
    disconnect(toolBar, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
    disconnect(stVolControlThread, SIGNAL(started()), this, SLOT(changeDockVisibility()));
    disconnect(stVolControlThread, SIGNAL(finished()), this, SLOT(changeDockVisibility()));
    disconnect(stVolControlThread, SIGNAL(resultData(StorageVolActions, QStringList)), this, SLOT(resultReceiver(StorageVolActions, QStringList)));
    disconnect(stVolControlThread, SIGNAL(errorMsg(QString)), this, SLOT(msgRepeater(QString)));

    stopProcessing();
    stVolControlThread->terminate();
    delete stVolControlThread;
    stVolControlThread = 0;

    if ( currWorkConnect!=NULL ) {
        virConnectClose(currWorkConnect);
        currWorkConnect = NULL;
    };

    delete toolBar;
    toolBar = 0;

    if (storageVolModel!=NULL) {
        delete storageVolModel;
        storageVolModel = 0;
    };

    if (storageVolList!=NULL) {
        delete storageVolList;
        storageVolList = 0;
    };
}

/* public slots */
bool VirtStorageVolControl::getThreadState() const
{
    return stVolControlThread->isFinished() || !stVolControlThread->isRunning();
}
void VirtStorageVolControl::stopProcessing()
{
    setEnabled(false);
    toolBar->stopProcessing();
    if ( stVolControlThread!=NULL ) {
        stVolControlThread->stop();
    };

    if ( currWorkConnect!=NULL ) {
        virConnectClose(currWorkConnect);
        currWorkConnect = NULL;
    };

    // clear StorageVol list
    while ( storageVolModel->DataList.count() ) {
        storageVolModel->removeRow(0);
    };
    storageVolModel->setHeaderData(0, Qt::Horizontal, QString("Name"), Qt::EditRole);
}
bool VirtStorageVolControl::setCurrentStoragePool(virConnect *conn, QString &connName, QString &poolName)
{
    stopProcessing();
    currWorkConnect = conn;
    int ret = virConnectRef(currWorkConnect);
    if ( ret<0 ) {
        virErrorPtr virtErrors = virGetLastError();
        if ( virtErrors!=NULL && virtErrors->code>0 ) {
            QString time = QTime::currentTime().toString();
            QString msg = QString("%3 VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message).arg(time);
            emit storageVolMsg( msg );
            virResetError(virtErrors);
        };
        currWorkConnect = NULL;
        return false;
    } else {
        setEnabled(true);
        currConnName = connName;
        currPoolName = poolName;
        storageVolModel->setHeaderData(0, Qt::Horizontal, QString("Name (Pool: \"%1\")").arg(poolName), Qt::EditRole);
        stVolControlThread->setCurrentStoragePoolName(currWorkConnect, currPoolName);
        toolBar->enableAutoReload();
        // for initiation content
        stVolControlThread->execAction(GET_ALL_StVOL, QStringList());
        return true;
    };
}
QString VirtStorageVolControl::getCurrentVolumeName() const
{
    QModelIndex index = storageVolList->currentIndex();
    return storageVolModel->DataList.at(index.row())->getName();
}

/* private slots */
void VirtStorageVolControl::resultReceiver(StorageVolActions act, QStringList data)
{
    //qDebug()<<act<<data<<"result";
    if ( act == GET_ALL_StVOL ) {
        int chain  = storageVolModel->columnCount();
        int chains = data.count()/chain;
        if ( chains > storageVolModel->DataList.count() ) {
            int _diff = chains - storageVolModel->DataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                storageVolModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( storageVolModel->DataList.count() > chains ) {
            int _diff = storageVolModel->DataList.count() - chains;
            for ( int i = 0; i<_diff; i++ ) {
                storageVolModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        for (int i = 0; i<chains; i++) {
            for (int j = 0; j<chain; j++) {
                storageVolModel->setData(storageVolModel->index(i,j), data.at(i*chain+j), Qt::EditRole);
            };
        };
    } else if ( act == CREATE_StVOL ) {
        if ( !data.isEmpty() ) {
            msgRepeater(data.join(" "));
            stVolControlThread->execAction(GET_ALL_StVOL, QStringList());
        };
    } else if ( act == DELETE_StVOL ) {
        if ( !data.isEmpty() ) {
            msgRepeater(data.join(" "));
            stVolControlThread->execAction(GET_ALL_StVOL, QStringList());
        };
    } else if ( act == DOWNLOAD_StVOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == UPLOAD_StVOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == RESIZE_StVOL ) {
        if ( !data.isEmpty() ) {
            msgRepeater(data.join(" "));
            stVolControlThread->execAction(GET_ALL_StVOL, QStringList());
        };
    } else if ( act == WIPE_StVOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == GET_StVOL_XML_DESC ) {
        if ( !data.isEmpty() ) {
            QString xml = data.first();
            data.removeFirst();
            data.append(QString("to <a href='%1'>%1</a>").arg(xml));
            msgRepeater(data.join(" "));
            QDesktopServices::openUrl(QUrl(xml));
        };
    };
}
void VirtStorageVolControl::msgRepeater(QString msg)
{
    QString time = QTime::currentTime().toString();
    QString title = QString("Conn '%1'").arg(currConnName);
    QString errorMsg = QString("<b>%1 %2:</b><br>%3").arg(time).arg(title).arg(msg);
    emit storageVolMsg(errorMsg);
}
void VirtStorageVolControl::changeDockVisibility()
{
    toolBar->setEnabled( !toolBar->isEnabled() );
    storageVolList->setEnabled( !storageVolList->isEnabled() );
}

void VirtStorageVolControl::storageVolClicked(const QPoint &p)
{
    //qDebug()<<"custom Menu request";
    QModelIndex idx = storageVolList->indexAt(p);
    QStringList params;
    if ( idx.isValid() ) {
        //qDebug()<<storageVolModel->DataList.at(idx.row())->getName();
        params<<storageVolModel->DataList.at(idx.row())->getName();
        //params<<storageVolModel->DataList.at(idx.row())->getPath();
        //params<<storageVolModel->DataList.at(idx.row())->getType();
        //params<<storageVolModel->DataList.at(idx.row())->getCurrSize();
        //params<<storageVolModel->DataList.at(idx.row())->getLogicSize();
    } else {
        storageVolList->clearSelection();
    };
    bool state = toolBar->getAutoReloadState();
    StorageVolControlMenu *storageVolControlMenu = new StorageVolControlMenu(this, params, state);
    connect(storageVolControlMenu, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
    storageVolControlMenu->move(QCursor::pos());
    storageVolControlMenu->exec();
    disconnect(storageVolControlMenu, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
    storageVolControlMenu->deleteLater();
}
void VirtStorageVolControl::storageVolDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        qDebug()<<storageVolModel->DataList.at(index.row())->getName();
    }
}
void VirtStorageVolControl::execAction(const QStringList &l)
{
    QStringList args;
    QModelIndex idx = storageVolList->currentIndex();
    if ( idx.isValid() && storageVolModel->DataList.count() ) {
        QString storageVolName = storageVolModel->DataList.at(idx.row())->getName();
        args.append(storageVolName);
        if        ( l.first()=="getVirtStorageVolList" ) {
            stVolControlThread->execAction(GET_ALL_StVOL, args);
        } else if ( l.first()=="deleteVirtStorageVol" ) {
            stVolControlThread->execAction(DELETE_StVOL, args);
        } else if ( l.first()=="downloadVirtStorageVol" ) {
            QString path = QFileDialog::getSaveFileName(this, "Save to", "~");
            if ( !path.isEmpty() ) {
                args.append(path);
                args.append(storageVolModel->DataList.at(idx.row())->getCurrSize());
                stVolControlThread->execAction(DOWNLOAD_StVOL, args);
            } else return;
        } else if ( l.first()=="resizeVirtStorageVol" ) {
            ResizeDialog *resizeDialog = new ResizeDialog(this,
                storageVolModel->DataList.at(idx.row())->getLogicSize().toULongLong());
            int res = resizeDialog->exec();
            unsigned long long size = resizeDialog->getNewSize();
            resizeDialog->deleteLater();
            if ( res ) {
                args.append( QString("%1").arg(size) );
            } else {
                return;
            };
            stVolControlThread->execAction(RESIZE_StVOL, args);
        } else if ( l.first()=="uploadVirtStorageVol" ) {
            QString path = QFileDialog::getOpenFileName(this, "Read from", "~");
            if ( !path.isEmpty() ) {
                args.append(path);
                stVolControlThread->execAction(UPLOAD_StVOL, args);
            } else return;
        } else if ( l.first()=="wipeVirtStorageVol" ) {
            args.append( (l.count()>1) ? l.at(1) : "0" );
            stVolControlThread->execAction(WIPE_StVOL, args);
        } else if ( l.first()=="getVirtStorageVolXMLDesc" ) {
            stVolControlThread->execAction(GET_StVOL_XML_DESC, args);
        } else if ( l.first()=="stopOverViewVirtStoragePool" ) {
            stopProcessing();
        } else if ( l.first()=="reloadVirtStoragePool" ) {
            stVolControlThread->execAction(GET_ALL_StVOL, args);
        };
    } else if ( l.first()=="stopOverViewVirtStoragePool" ) {
        stopProcessing();
    } else if ( l.first()=="reloadVirtStoragePool" ) {
        stVolControlThread->execAction(GET_ALL_StVOL, args);
    };
}
void VirtStorageVolControl::newVirtStorageVolFromXML(const QStringList &_args)
{
    StorageVolActions act;
    if ( !_args.isEmpty() ) {
        if ( _args.first().startsWith("create") ) act = CREATE_StVOL;
        else act = StVOL_EMPTY_ACTION;
        QStringList args = _args;
        args.removeFirst();
        if ( !args.isEmpty() ) {
            if ( args.first()=="manually" ) {
                args.removeFirst();
                QString source = args.first();
                args.removeFirst();
                QString path;
                // show SRC Creator widget
                // get path for method
                QMessageBox::information(this, "INFO", QString("Manual settings for %2(%1) not implemented yet.").arg(act).arg(source), QMessageBox::Ok);
                args.prepend(path);
            };
            stVolControlThread->execAction(act, args);
        };
    };
}
