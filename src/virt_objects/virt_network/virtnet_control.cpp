#include "virtnet_control.h"

VirtNetControl::VirtNetControl(QWidget *parent) :
    QMainWindow(parent)
{
    setObjectName("VirtNetControl");
    setWindowTitle("VirtNet Control");
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    setWindowIcon(QIcon::fromTheme("preferences-system-network"));
    virtNetModel = new VirtNetModel();
    virtNetList = new QTreeView(this);
    virtNetList->setItemsExpandable(false);
    virtNetList->setRootIsDecorated(false);
    virtNetList->setModel(virtNetModel);
    virtNetList->setFocus();
    virtNetList->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(virtNetList, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(networkDoubleClicked(const QModelIndex&)));
    connect(virtNetList, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(networkClicked(const QPoint&)));
    setCentralWidget(virtNetList);
    settings.beginGroup("VirtNetControl");
    virtNetList->setColumnWidth(0, settings.value("column0", 132).toInt());
    virtNetList->setColumnWidth(1, settings.value("column1", 32).toInt());
    virtNetList->setColumnWidth(2, settings.value("column2", 32).toInt());
    virtNetList->setColumnWidth(3, settings.value("column3", 32).toInt());
    int area_int = settings.value("ToolBarArea", 4).toInt();
    //bool check = settings.value("CheckList", false).toBool();
    interval = settings.value("UpdateTime", 3).toInt();
    settings.endGroup();
    toolBar = new VirtNetToolBar(this);
    addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
    //connect(toolBar, SIGNAL(fileForMethod(const QStringList&)), this, SLOT(actionActivated(const QStringList&)));
    connect(toolBar, SIGNAL(execMethod(const QStringList&)), this, SLOT(appendNetworkName(const QStringList&)));
    netControlThread = new NetControlThread(this);
    connect(netControlThread, SIGNAL(started()), this, SLOT(changeToolBarState()));
    connect(netControlThread, SIGNAL(finished()), this, SLOT(changeToolBarState()));
    connect(netControlThread, SIGNAL(resultData(QStringList)), this, SLOT(resultReceiver(QStringList)));
}
VirtNetControl::~VirtNetControl()
{
    settings.beginGroup("VirtNetControl");
    settings.setValue("column0", virtNetList->columnWidth(0));
    settings.setValue("column1", virtNetList->columnWidth(1));
    settings.setValue("column2", virtNetList->columnWidth(2));
    settings.setValue("column3", virtNetList->columnWidth(3));
    //settings.setValue("UpdateTime", docContent->getUpdateTime());
    //settings.setValue("CheckList", docContent->getCheckList());
    settings.setValue("ToolBarArea", toolBarArea(toolBar));
    settings.endGroup();
    settings.sync();
    //disconnect(virtNetList, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(networkDoubleClicked(const QModelIndex&)));
    disconnect(virtNetList, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(networkClicked(const QPoint&)));
    //disconnect(toolBar, SIGNAL(fileForMethod(const QStringList&)), this, SLOT(actionActivated(const QStringList&)));
    disconnect(toolBar, SIGNAL(execMethod(const QStringList&)), this, SLOT(appendNetworkName(const QStringList&)));
    disconnect(netControlThread, SIGNAL(started()), this, SLOT(changeToolBarState()));
    disconnect(netControlThread, SIGNAL(finished()), this, SLOT(changeToolBarState()));
    disconnect(netControlThread, SIGNAL(resultData(QStringList)), this, SLOT(resultReceiver(QStringList)));

    stopProcessing();
    netControlThread->terminate();
    delete netControlThread;
    netControlThread = 0;

    delete toolBar;
    toolBar = 0;

    if (virtNetModel!=NULL) {
        delete virtNetModel;
        virtNetModel = 0;
    };

    if (virtNetList!=NULL) {
        delete virtNetList;
        virtNetList = 0;
    };
}

/* public slots */
bool VirtNetControl::getThreadState() const
{
    return netControlThread->isFinished() || !netControlThread->isRunning();
}
void VirtNetControl::stopProcessing()
{
    if ( timerId ) {
        killTimer(timerId);
        timerId = 0;
    }
    if ( netControlThread!=NULL ) {
        netControlThread->stop();
    };

    if ( currWorkConnect!=NULL ) {
        virConnectClose(currWorkConnect);
        currWorkConnect = NULL;
    };

    // clear Network list
    for ( int i = 0; i<virtNetModel->virtNetDataList.count(); i++ ) {
        virtNetModel->removeRow(0);
    };

}
bool VirtNetControl::setCurrentWorkConnect(virConnect *conn)
{
    qDebug()<<conn;
    stopProcessing();
    currWorkConnect = conn;
    int ret = virConnectRef(currWorkConnect);
    if ( ret<0 ) {
        virErrorPtr virtErrors = virGetLastError();
        if ( virtErrors!=NULL ) {
            QString time = QTime::currentTime().toString();
            QString msg = QString("%3 VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message).arg(time);
            emit netMsg( msg );
            virResetError(virtErrors);
        };
        currWorkConnect = NULL;
        return false;
    } else {
        netControlThread->setCurrentWorkConnect(currWorkConnect);
        timerId = startTimer(interval*1000);
        return true;
    };
}

/* private slots */
void VirtNetControl::timerEvent(QTimerEvent *event)
{
    int _timerId = event->timerId();
    if ( _timerId && timerId==_timerId ) {
        netControlThread->execAction(GET_ALL_NETWORK, QStringList());
    };
}
void VirtNetControl::resultReceiver(QStringList data)
{
    bool ok;
    int method = data.first().toInt(&ok);
    data.removeFirst();
    if (!ok) method = -1;
    //qDebug()<<method<<data<<"result";
    if ( method == GET_ALL_NETWORK ) {
        if ( data.count() > virtNetModel->virtNetDataList.count() ) {
            int _diff = data.count() - virtNetModel->virtNetDataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtNetModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( virtNetModel->virtNetDataList.count() > data.count() ) {
            int _diff = virtNetModel->virtNetDataList.count() - data.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtNetModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QString _data, data) {
            QStringList chain = _data.split(" ");
            if (chain.isEmpty()) continue;
            int count = chain.size();
            for (int j=0; j<count; j++) {
                virtNetModel->setData(virtNetModel->index(i,j), chain.at(j), Qt::EditRole);
            };
            i++;
        };
    } else if ( method == CREATE_NETWORK ) {

    } else if ( method == DEFINE_NETWORK ) {

    } else if ( method == START_NETWORK ) {

    } else if ( method == DESTROY_NETWORK ) {

    } else if ( method == UNDEFINE_NETWORK ) {

    } else if ( method == CHANGE_AUTOSTART ) {

    };
}
void VirtNetControl::warningShow(QString title, QString msg)
{
    QString time = QTime::currentTime().toString();
    QString errorMsg = QString("<b>%1 %2:</b><br>%3").arg(time).arg(title).arg(msg);
    //docContent->appendErrorMsg(errorMsg);
}
void VirtNetControl::changeToolBarState()
{
    toolBar->setEnabled( !toolBar->isEnabled() );
}

void VirtNetControl::networkClicked(const QPoint &p)
{
    //qDebug()<<"custom Menu request";
    QModelIndex idx = virtNetList->indexAt(p);
    if ( idx.isValid() ) {
        //qDebug()<<virtNetModel->virtNetDataList.at(idx.row())->getName();
        QStringList params;
        params<<virtNetModel->virtNetDataList.at(idx.row())->getName();
        params<<virtNetModel->virtNetDataList.at(idx.row())->getState();
        params<<virtNetModel->virtNetDataList.at(idx.row())->getAutostart();
        params<<virtNetModel->virtNetDataList.at(idx.row())->getPersistent();
        VirtNetControlMenu *netControlMenu = new VirtNetControlMenu(this, params);
        //connect(netControlMenu, SIGNAL(execMethod(const QStringList&)), this, SLOT(actionActivated(const QStringList&)));
        netControlMenu->move(QCursor::pos());
        netControlMenu->exec();
        //disconnect(netControlMenu, SIGNAL(execMethod(const QStringList&)), this, SLOT(actionActivated(const QStringList&)));
        netControlMenu->deleteLater();
    } else {
        virtNetList->clearSelection();
    }
}
void VirtNetControl::networkDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        qDebug()<<virtNetModel->virtNetDataList.at(index.row())->getName();
    }
}
void VirtNetControl::appendNetworkName(const QStringList &l)
{
    QStringList params;
    QModelIndex idx = virtNetList->currentIndex();
    if ( idx.isValid() ) {
        QString networkName = virtNetModel->virtNetDataList.at(idx.row())->getName();
        params = l;
        params.append(networkName);
        if ( l.first()=="setAutostartVirtNetwork" ) {
            /* set the opposite value */
            QString autostartState =
                (virtNetModel->virtNetDataList.at(idx.row())->getAutostart()=="yes")
                 ? "0" : "1";
            params.append(autostartState);
        };
        netControlThread->execAction(CHANGE_AUTOSTART, params);
    }
}
