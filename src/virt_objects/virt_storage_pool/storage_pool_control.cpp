#include "storage_pool_control.h"

VirtStoragePoolControl::VirtStoragePoolControl(QWidget *parent) :
    QMainWindow(parent)
{
    setObjectName("VirtStoragePoolControl");
    setWindowTitle("StoragePool Control");
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    setWindowIcon(QIcon::fromTheme("storagePool"));
    storagePoolModel = new StoragePoolModel();
    storagePoolList = new QTreeView(this);
    storagePoolList->setItemsExpandable(false);
    storagePoolList->setRootIsDecorated(false);
    storagePoolList->setModel(storagePoolModel);
    storagePoolList->setFocus();
    storagePoolList->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(storagePoolList, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(storagePoolDoubleClicked(const QModelIndex&)));
    connect(storagePoolList, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(storagePoolClicked(const QPoint&)));
    setCentralWidget(storagePoolList);
    settings.beginGroup("VirtStoragePoolControl");
    storagePoolList->setColumnWidth(0, settings.value("column0", 132).toInt());
    storagePoolList->setColumnWidth(1, settings.value("column1", 32).toInt());
    storagePoolList->setColumnWidth(2, settings.value("column2", 32).toInt());
    storagePoolList->setColumnWidth(3, settings.value("column3", 32).toInt());
    int area_int = settings.value("ToolBarArea", 4).toInt();
    //bool check = settings.value("CheckList", false).toBool();
    interval = settings.value("UpdateTime", 3).toInt();
    settings.endGroup();
    toolBar = new StoragePoolToolBar(this);
    addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
    connect(toolBar, SIGNAL(fileForMethod(const QStringList&)), this, SLOT(newVirtStoragePoolFromXML(const QStringList&)));
    connect(toolBar, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
    stPoolControlThread = new StoragePoolControlThread(this);
    connect(stPoolControlThread, SIGNAL(started()), this, SLOT(changeDockVisibility()));
    connect(stPoolControlThread, SIGNAL(finished()), this, SLOT(changeDockVisibility()));
    connect(stPoolControlThread, SIGNAL(resultData(StoragePoolActions, QStringList)), this, SLOT(resultReceiver(StoragePoolActions, QStringList)));
    connect(stPoolControlThread, SIGNAL(errorMsg(QString)), this, SLOT(msgRepeater(QString)));
}
VirtStoragePoolControl::~VirtStoragePoolControl()
{
    settings.beginGroup("VirtStoragePoolControl");
    settings.setValue("column0", storagePoolList->columnWidth(0));
    settings.setValue("column1", storagePoolList->columnWidth(1));
    settings.setValue("column2", storagePoolList->columnWidth(2));
    settings.setValue("column3", storagePoolList->columnWidth(3));
    //settings.setValue("UpdateTime", docContent->getUpdateTime());
    //settings.setValue("CheckList", docContent->getCheckList());
    settings.setValue("ToolBarArea", toolBarArea(toolBar));
    settings.endGroup();
    settings.sync();
    //disconnect(storagePoolList, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(storagePoolDoubleClicked(const QModelIndex&)));
    disconnect(storagePoolList, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(storagePoolClicked(const QPoint&)));
    disconnect(toolBar, SIGNAL(fileForMethod(const QStringList&)), this, SLOT(newVirtStoragePoolFromXML(const QStringList&)));
    disconnect(toolBar, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
    disconnect(stPoolControlThread, SIGNAL(started()), this, SLOT(changeDockVisibility()));
    disconnect(stPoolControlThread, SIGNAL(finished()), this, SLOT(changeDockVisibility()));
    disconnect(stPoolControlThread, SIGNAL(resultData(StoragePoolActions, QStringList)), this, SLOT(resultReceiver(StoragePoolActions, QStringList)));
    disconnect(stPoolControlThread, SIGNAL(errorMsg(QString)), this, SLOT(msgRepeater(QString)));

    stopProcessing();
    stPoolControlThread->terminate();
    delete stPoolControlThread;
    stPoolControlThread = 0;

    delete toolBar;
    toolBar = 0;

    if (storagePoolModel!=NULL) {
        delete storagePoolModel;
        storagePoolModel = 0;
    };

    if (storagePoolList!=NULL) {
        delete storagePoolList;
        storagePoolList = 0;
    };
}

/* public slots */
bool VirtStoragePoolControl::getThreadState() const
{
    return stPoolControlThread->isFinished() || !stPoolControlThread->isRunning();
}
void VirtStoragePoolControl::stopProcessing()
{
    if ( timerId ) {
        killTimer(timerId);
        timerId = 0;
    }
    if ( stPoolControlThread!=NULL ) {
        stPoolControlThread->stop();
    };

    if ( currWorkConnect!=NULL ) {
        virConnectClose(currWorkConnect);
        currWorkConnect = NULL;
    };

    // clear StoragePool list
    while ( storagePoolModel->virtStoragePoolDataList.count() ) {
        storagePoolModel->removeRow(0);
    };
    storagePoolModel->setHeaderData(0, Qt::Horizontal, QString("Name"), Qt::EditRole);

}
bool VirtStoragePoolControl::setCurrentWorkConnect(virConnect *conn)
{
    stopProcessing();
    currWorkConnect = conn;
    int ret = virConnectRef(currWorkConnect);
    if ( ret<0 ) {
        virErrorPtr virtErrors = virGetLastError();
        if ( virtErrors!=NULL ) {
            QString time = QTime::currentTime().toString();
            QString msg = QString("%3 VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message).arg(time);
            emit storagePoolMsg( msg );
            virResetError(virtErrors);
        };
        currWorkConnect = NULL;
        return false;
    } else {
        stPoolControlThread->setCurrentWorkConnect(currWorkConnect);
        timerId = startTimer(interval*1000);
        return true;
    };
}
void VirtStoragePoolControl::setListHeader(QString &connName)
{
    storagePoolModel->setHeaderData(0, Qt::Horizontal, QString("Name (Conn: \"%1\")").arg(connName), Qt::EditRole);
    currConnName = connName;
}

/* private slots */
void VirtStoragePoolControl::timerEvent(QTimerEvent *event)
{
    int _timerId = event->timerId();
    if ( _timerId && timerId==_timerId ) {
        stPoolControlThread->execAction(GET_ALL_StPOOL, QStringList());
    };
}
void VirtStoragePoolControl::resultReceiver(StoragePoolActions act, QStringList data)
{
    //qDebug()<<act<<data<<"result";
    if ( act == GET_ALL_StPOOL ) {
        if ( data.count() > storagePoolModel->virtStoragePoolDataList.count() ) {
            int _diff = data.count() - storagePoolModel->virtStoragePoolDataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                storagePoolModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( storagePoolModel->virtStoragePoolDataList.count() > data.count() ) {
            int _diff = storagePoolModel->virtStoragePoolDataList.count() - data.count();
            for ( int i = 0; i<_diff; i++ ) {
                storagePoolModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QString _data, data) {
            QStringList chain = _data.split(" ");
            if (chain.isEmpty()) continue;
            int count = chain.size();
            for (int j=0; j<count; j++) {
                storagePoolModel->setData(storagePoolModel->index(i,j), chain.at(j), Qt::EditRole);
            };
            i++;
        };
    } else if ( act == CREATE_StPOOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == DEFINE_StPOOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == START_StPOOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == DESTROY_StPOOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == UNDEFINE_StPOOL ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == CHANGE_StPOOL_AUTOSTART ) {
        if ( !data.isEmpty() ) msgRepeater(data.join(" "));
    } else if ( act == GET_StPOOL_XML_DESC ) {
        if ( !data.isEmpty() ) {
            QString xml = data.first();
            data.removeFirst();
            data<<"in"<<xml;
            msgRepeater(data.join(" "));
            // exec xdg-open temporarily XML file
            // TODO: make a cross-platform
            QString command = QString("xdg-open %1").arg(xml);
            QProcess::startDetached(command);
        };
    };
}
void VirtStoragePoolControl::msgRepeater(QString msg)
{
    QString time = QTime::currentTime().toString();
    QString title = QString("Connect '%1'").arg(currConnName);
    QString errorMsg = QString("<b>%1 %2:</b><br>%3").arg(time).arg(title).arg(msg);
    emit storagePoolMsg(errorMsg);
}
void VirtStoragePoolControl::changeDockVisibility()
{
    toolBar->setEnabled( !toolBar->isEnabled() );
    storagePoolList->setEnabled( !storagePoolList->isEnabled() );
}

void VirtStoragePoolControl::storagePoolClicked(const QPoint &p)
{
    //qDebug()<<"custom Menu request";
    QModelIndex idx = storagePoolList->indexAt(p);
    if ( idx.isValid() ) {
        //qDebug()<<storagePoolModel->virtStoragePoolDataList.at(idx.row())->getName();
        QStringList params;
        params<<storagePoolModel->virtStoragePoolDataList.at(idx.row())->getName();
        params<<storagePoolModel->virtStoragePoolDataList.at(idx.row())->getState();
        params<<storagePoolModel->virtStoragePoolDataList.at(idx.row())->getAutostart();
        params<<storagePoolModel->virtStoragePoolDataList.at(idx.row())->getPersistent();
        StoragePoolControlMenu *domControlMenu = new StoragePoolControlMenu(this, params);
        connect(domControlMenu, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
        domControlMenu->move(QCursor::pos());
        domControlMenu->exec();
        disconnect(domControlMenu, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
        domControlMenu->deleteLater();
    } else {
        storagePoolList->clearSelection();
    }
}
void VirtStoragePoolControl::storagePoolDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        qDebug()<<storagePoolModel->virtStoragePoolDataList.at(index.row())->getName();
    }
}
void VirtStoragePoolControl::execAction(const QStringList &l)
{
    QStringList args;
    QModelIndex idx = storagePoolList->currentIndex();
    if ( idx.isValid() ) {
        QString storagePoolName = storagePoolModel->virtStoragePoolDataList.at(idx.row())->getName();
        args.append(storagePoolName);
        if        ( l.first()=="startVirtStoragePool" ) {
            stPoolControlThread->execAction(START_StPOOL, args);
        } else if ( l.first()=="destroyVirtStoragePool" ) {
            stPoolControlThread->execAction(DESTROY_StPOOL, args);
        } else if ( l.first()=="undefineVirtStoragePool" ) {
            stPoolControlThread->execAction(UNDEFINE_StPOOL, args);
        } else if ( l.first()=="setAutostartVirtStoragePool" ) {
            /* set the opposite value */
            QString autostartState =
                (storagePoolModel->virtStoragePoolDataList.at(idx.row())->getAutostart()=="yes")
                 ? "0" : "1";
            args.append(autostartState);
            stPoolControlThread->execAction(CHANGE_StPOOL_AUTOSTART, args);
        } else if ( l.first()=="getVirtDomXMLDesc" ) {
            stPoolControlThread->execAction(GET_StPOOL_XML_DESC, args);
        };
    }
}
void VirtStoragePoolControl::newVirtStoragePoolFromXML(const QStringList &_args)
{
    StoragePoolActions act;
    if ( !_args.isEmpty() ) {
        if ( _args.first().startsWith("create") ) act = CREATE_StPOOL;
        else act = DEFINE_StPOOL;
        QStringList args = _args;
        args.removeFirst();
        if ( !args.isEmpty() ) stPoolControlThread->execAction(act, args);
    };
}
