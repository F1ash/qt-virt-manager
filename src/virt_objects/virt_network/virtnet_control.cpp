#include "virtnet_control.h"

VirtNetControl::VirtNetControl(QWidget *parent) :
    QMainWindow(parent)
{
    setObjectName("VirtNetControl");
    setWindowTitle("VirtNet Control");
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    setWindowIcon(QIcon::fromTheme("network"));
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
    settings.endGroup();
    toolBar = new VirtNetToolBar(this);
    addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
    connect(toolBar, SIGNAL(fileForMethod(const QStringList&)), this, SLOT(newVirtNetworkFromXML(const QStringList&)));
    connect(toolBar, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
    netControlThread = new NetControlThread(this);
    connect(netControlThread, SIGNAL(started()), this, SLOT(changeDockVisibility()));
    connect(netControlThread, SIGNAL(finished()), this, SLOT(changeDockVisibility()));
    connect(netControlThread, SIGNAL(resultData(NetActions, QStringList)), this, SLOT(resultReceiver(NetActions, QStringList)));
    connect(netControlThread, SIGNAL(errorMsg(QString)), this, SLOT(msgRepeater(QString)));
}
VirtNetControl::~VirtNetControl()
{
    settings.beginGroup("VirtNetControl");
    settings.setValue("column0", virtNetList->columnWidth(0));
    settings.setValue("column1", virtNetList->columnWidth(1));
    settings.setValue("column2", virtNetList->columnWidth(2));
    settings.setValue("column3", virtNetList->columnWidth(3));
    settings.setValue("ToolBarArea", toolBarArea(toolBar));
    settings.endGroup();
    settings.sync();
    //disconnect(virtNetList, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(networkDoubleClicked(const QModelIndex&)));
    disconnect(virtNetList, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(networkClicked(const QPoint&)));
    disconnect(toolBar, SIGNAL(fileForMethod(const QStringList&)), this, SLOT(newVirtNetworkFromXML(const QStringList&)));
    disconnect(toolBar, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
    disconnect(netControlThread, SIGNAL(started()), this, SLOT(changeDockVisibility()));
    disconnect(netControlThread, SIGNAL(finished()), this, SLOT(changeDockVisibility()));
    disconnect(netControlThread, SIGNAL(resultData(NetActions, QStringList)), this, SLOT(resultReceiver(NetActions, QStringList)));
    disconnect(netControlThread, SIGNAL(errorMsg(QString)), this, SLOT(msgRepeater(QString)));

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
    if ( netControlThread!=NULL ) {
        netControlThread->stop();
    };

    if ( currWorkConnect!=NULL ) {
        virConnectClose(currWorkConnect);
        currWorkConnect = NULL;
    };

    // clear Network list
    while ( virtNetModel->DataList.count() ) {
        virtNetModel->removeRow(0);
    };
    virtNetModel->setHeaderData(0, Qt::Horizontal, QString("Name"), Qt::EditRole);
    setEnabled(false);

}
bool VirtNetControl::setCurrentWorkConnect(virConnect *conn)
{
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
        toolBar->enableAutoReload();
        // for initiation content
        netControlThread->execAction(GET_ALL_NETWORK, QStringList());
        return true;
    };
}
void VirtNetControl::setListHeader(QString &connName)
{
    virtNetModel->setHeaderData(0, Qt::Horizontal, QString("Name (Conn: \"%1\")").arg(connName), Qt::EditRole);
    currConnName = connName;
    setEnabled(true);
}

/* private slots */
void VirtNetControl::resultReceiver(NetActions act, QStringList data)
{
    //qDebug()<<act<<data<<"result";
    if ( act == GET_ALL_NETWORK ) {
        if ( data.count() > virtNetModel->DataList.count() ) {
            int _diff = data.count() - virtNetModel->DataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtNetModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( virtNetModel->DataList.count() > data.count() ) {
            int _diff = virtNetModel->DataList.count() - data.count();
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
    } else if ( act == CREATE_NETWORK ) {
        if ( !data.isEmpty() ) {
            msgRepeater(data.join(" "));
            netControlThread->execAction(GET_ALL_NETWORK, QStringList());
        };
    } else if ( act == DEFINE_NETWORK ) {
        if ( !data.isEmpty() ) {
            msgRepeater(data.join(" "));
            netControlThread->execAction(GET_ALL_NETWORK, QStringList());
        };
    } else if ( act == START_NETWORK ) {
        if ( !data.isEmpty() ) {
            msgRepeater(data.join(" "));
            netControlThread->execAction(GET_ALL_NETWORK, QStringList());
        };
    } else if ( act == DESTROY_NETWORK ) {
        if ( !data.isEmpty() ) {
            msgRepeater(data.join(" "));
            netControlThread->execAction(GET_ALL_NETWORK, QStringList());
        };
    } else if ( act == UNDEFINE_NETWORK ) {
        if ( !data.isEmpty() ) {
            msgRepeater(data.join(" "));
            netControlThread->execAction(GET_ALL_NETWORK, QStringList());
        };
    } else if ( act == CHANGE_NET_AUTOSTART ) {
        if ( !data.isEmpty() ) {
            msgRepeater(data.join(" "));
            netControlThread->execAction(GET_ALL_NETWORK, QStringList());
        };
    } else if ( act == GET_NET_XML_DESC ) {
        if ( !data.isEmpty() ) {
            QString xml = data.first();
            data.removeFirst();
            data.append(QString("in <a href='%1'>%1</a>").arg(xml));
            msgRepeater(data.join(" "));
            QDesktopServices::openUrl(QUrl(xml));
        };
    };
}
void VirtNetControl::msgRepeater(QString msg)
{
    QString time = QTime::currentTime().toString();
    QString title = QString("Connect '%1'").arg(currConnName);
    QString errorMsg = QString("<b>%1 %2:</b><br>%3").arg(time).arg(title).arg(msg);
    emit netMsg(errorMsg);
}
void VirtNetControl::changeDockVisibility()
{
    toolBar->setEnabled( !toolBar->isEnabled() );
    virtNetList->setEnabled( !virtNetList->isEnabled() );
}

void VirtNetControl::networkClicked(const QPoint &p)
{
    //qDebug()<<"custom Menu request";
    QModelIndex idx = virtNetList->indexAt(p);
    QStringList params;
    if ( idx.isValid() ) {
        //qDebug()<<virtNetModel->DataList.at(idx.row())->getName();
        params<<virtNetModel->DataList.at(idx.row())->getName();
        params<<virtNetModel->DataList.at(idx.row())->getState();
        params<<virtNetModel->DataList.at(idx.row())->getAutostart();
        params<<virtNetModel->DataList.at(idx.row())->getPersistent();
    } else {
        virtNetList->clearSelection();
    };
    bool state = toolBar->getAutoReloadState();
    VirtNetControlMenu *netControlMenu = new VirtNetControlMenu(this, params, state);
    connect(netControlMenu, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
    netControlMenu->move(QCursor::pos());
    netControlMenu->exec();
    disconnect(netControlMenu, SIGNAL(execMethod(const QStringList&)), this, SLOT(execAction(const QStringList&)));
    netControlMenu->deleteLater();
}
void VirtNetControl::networkDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        qDebug()<<virtNetModel->DataList.at(index.row())->getName();
    }
}
void VirtNetControl::execAction(const QStringList &l)
{
    QStringList args;
    QModelIndex idx = virtNetList->currentIndex();
    if ( idx.isValid() ) {
        QString networkName = virtNetModel->DataList.at(idx.row())->getName();
        args.append(networkName);
        if        ( l.first()=="startVirtNetwork" ) {
            netControlThread->execAction(START_NETWORK, args);
        } else if ( l.first()=="destroyVirtNetwork" ) {
            netControlThread->execAction(DESTROY_NETWORK, args);
        } else if ( l.first()=="undefineVirtNetwork" ) {
            netControlThread->execAction(UNDEFINE_NETWORK, args);
        } else if ( l.first()=="setAutostartVirtNetwork" ) {
            /* set the opposite value */
            QString autostartState =
                (virtNetModel->DataList.at(idx.row())->getAutostart()=="yes")
                 ? "0" : "1";
            args.append(autostartState);
            netControlThread->execAction(CHANGE_NET_AUTOSTART, args);
        } else if ( l.first()=="getVirtNetXMLDesc" ) {
            netControlThread->execAction(GET_NET_XML_DESC, args);
        } else if ( l.first()=="reloadVirtNetwork" ) {
            netControlThread->execAction(GET_ALL_NETWORK, args);
        };
    } else if ( l.first()=="reloadVirtNetwork" ) {
        netControlThread->execAction(GET_ALL_NETWORK, args);
    };
}
void VirtNetControl::newVirtNetworkFromXML(const QStringList &_args)
{
    NetActions act;
    if ( !_args.isEmpty() ) {
        if ( _args.first().startsWith("create") ) act = CREATE_NETWORK;
        else act = DEFINE_NETWORK;
        QStringList args = _args;
        args.removeFirst();
        if ( !args.isEmpty() ) {
            if ( args.first()=="manually" ) {
                args.removeFirst();
                //QString source = args.first();
                args.removeFirst();
                QString xml;
                // show SRC Creator widget
                CreateVirtNetwork *createVirtNet = new CreateVirtNetwork(this);
                int result = createVirtNet->exec();
                if ( createVirtNet!=NULL && result ) {
                    // get path for method
                    xml = createVirtNet->getXMLDescFileName();
                    QStringList data;
                    data.append("New Network XML'ed");
                    data.append(QString("in <a href='%1'>%1</a>").arg(xml));
                    msgRepeater(data.join(" "));
                    QDesktopServices::openUrl(QUrl(xml));
                };
                delete createVirtNet;
                createVirtNet = 0;
                //qDebug()<<xml<<"path"<<result;
                args.prepend(xml);
            };
            netControlThread->execAction(act, args);
        };
    };
}
