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
    //bool check = settings.value("CheckList", false).toBool();
    interval = settings.value("UpdateTime", 3).toInt();
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
    //settings.setValue("UpdateTime", docContent->getUpdateTime());
    //settings.setValue("CheckList", docContent->getCheckList());
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
    if ( timerId ) {
        killTimer(timerId);
        timerId = 0;
    }
    if ( stVolControlThread!=NULL ) {
        stVolControlThread->stop();
    };

    if ( currWorkConnect!=NULL ) {
        virConnectClose(currWorkConnect);
        currWorkConnect = NULL;
    };

    // clear StorageVol list
    while ( storageVolModel->virtStorageVolDataList.count() ) {
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
        if ( virtErrors!=NULL ) {
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
        timerId = startTimer(interval*1000);
        return true;
    };
}

/* private slots */
void VirtStorageVolControl::timerEvent(QTimerEvent *event)
{
    int _timerId = event->timerId();
    if ( _timerId && timerId==_timerId && isVisible() ) {
        //qDebug()<<"get stVol list";
        stVolControlThread->execAction(GET_ALL_StVOL, QStringList());
    };
}
void VirtStorageVolControl::resultReceiver(StorageVolActions act, QStringList data)
{
    //qDebug()<<act<<data<<"result";
    if ( act == GET_ALL_StVOL ) {
        if ( data.count() > storageVolModel->virtStorageVolDataList.count() ) {
            int _diff = data.count() - storageVolModel->virtStorageVolDataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                storageVolModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( storageVolModel->virtStorageVolDataList.count() > data.count() ) {
            int _diff = storageVolModel->virtStorageVolDataList.count() - data.count();
            for ( int i = 0; i<_diff; i++ ) {
                storageVolModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QString _data, data) {
            QStringList chain = _data.split(" ");
            if (chain.isEmpty()) continue;
            int count = chain.size();
            for (int j=0; j<count; j++) {
                storageVolModel->setData(storageVolModel->index(i,j), chain.at(j), Qt::EditRole);
            };
            i++;
        };
    } else if ( act == CREATE_StVOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == DOWNLOAD_StVOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == UPLOAD_StVOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == DELETE_StVOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == RESIZE_StVOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == GET_StVOL_XML_DESC ) {
        if ( !data.isEmpty() ) {
            QString xml = data.first();
            data.removeFirst();
            data.append(QString("in <a href='%1'>%1</a>").arg(xml));
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
    if ( idx.isValid() ) {
        //qDebug()<<storageVolModel->virtStorageVolDataList.at(idx.row())->getName();
        QStringList params;
        params<<storageVolModel->virtStorageVolDataList.at(idx.row())->getName();
        params<<storageVolModel->virtStorageVolDataList.at(idx.row())->getState();
        params<<storageVolModel->virtStorageVolDataList.at(idx.row())->getAutostart();
        params<<storageVolModel->virtStorageVolDataList.at(idx.row())->getPersistent();
        StorageVolControlMenu *storageVolControlMenu = new StorageVolControlMenu(this, params);
        connect(storageVolControlMenu, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
        storageVolControlMenu->move(QCursor::pos());
        storageVolControlMenu->exec();
        disconnect(storageVolControlMenu, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
        storageVolControlMenu->deleteLater();
    } else {
        storageVolList->clearSelection();
    }
}
void VirtStorageVolControl::storageVolDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        qDebug()<<storageVolModel->virtStorageVolDataList.at(index.row())->getName();
    }
}
void VirtStorageVolControl::execAction(const QStringList &l)
{
    QStringList args;
    QModelIndex idx = storageVolList->currentIndex();
    if ( idx.isValid() ) {
        QString storageVolName = storageVolModel->virtStorageVolDataList.at(idx.row())->getName();
        args.append(storageVolName);
        if        ( l.first()=="getVirtStorageVolList" ) {
            stVolControlThread->execAction(GET_ALL_StVOL, args);
        } else if ( l.first()=="uploadVirtStorageVol" ) {
            stVolControlThread->execAction(UPLOAD_StVOL, args);
        } else if ( l.first()=="downloadVirtStorageVol" ) {
            stVolControlThread->execAction(DOWNLOAD_StVOL, args);
        } else if ( l.first()=="deleteVirtStorageVol" ) {
            stVolControlThread->execAction(DELETE_StVOL, args);
        } else if ( l.first()=="resizeVirtStorageVol" ) {
            stVolControlThread->execAction(RESIZE_StVOL, args);
        } else if ( l.first()=="getVirtStorageVolXMLDesc" ) {
            stVolControlThread->execAction(GET_StVOL_XML_DESC, args);
        } else if ( l.first()=="stopOverViewVirtStoragePool" ) {
            stopProcessing();
        };
    } else if ( l.first()=="stopOverViewVirtStoragePool" ) {
        stopProcessing();
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
