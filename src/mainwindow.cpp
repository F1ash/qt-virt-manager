#include "mainwindow.h"
#include "version.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setSizePolicy(
                QSizePolicy(
                    QSizePolicy::MinimumExpanding,
                    QSizePolicy::MinimumExpanding));
    setMinimumSize(100, 100);
    setContentsMargins(0, 0, 0, 5);
    setWindowTitle("Qt VirtManager");
    QIcon::setThemeName("QtVirtManager");
    setWindowIcon(QIcon::fromTheme("virtual-engineering"));
    setMouseTracking(true);
    setDockOptions(
                QMainWindow::AnimatedDocks |
                QMainWindow::ForceTabbedDocks);
    restoreGeometry(settings.value("Geometry").toByteArray());
    initTaskWareHouse();
    initDomainStateMonitor();
    initTrayIcon();
    initConnListWidget();
    initToolBar();
    initDockWidgets();
    restoreState(settings.value("State").toByteArray());
    this->setVisible(!settings.value("Visible", false).toBool());
    changeVisibility();
    waitAtClose = settings.value("WaitAtClose", 180).toInt();
    closeProgress = new QProgressBar(this);
    closeProgress->setRange(0, waitAtClose*1000);
    closeProgress->setToolTip("Progress for waiting the connection close");
    initVirEventloop();
}

void MainWindow::saveSettings()
{
    taskWrHouse->saveCurrentState();
    domainsStateMonitor->saveCurrentState();
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("State", saveState());
    settings.setValue("ToolBarArea", toolBarArea(toolBar));
    settings.setValue("Visible", this->isVisible());
    settings.setValue("WaitAtClose", waitAtClose);
    settings.beginGroup("LogDock");
    settings.setValue("DockArea", dockWidgetArea(logDock));
    settings.setValue("Visible", logDock->isVisible());
    settings.setValue("Floating", logDock->isFloating());
    settings.setValue("Geometry", logDock->saveGeometry());
    settings.endGroup();
    settings.beginGroup("DomainDock");
    settings.setValue("DockArea", dockWidgetArea(domainDock));
    settings.setValue("Visible", domainDock->isVisible());
    settings.setValue("Floating", domainDock->isFloating());
    settings.setValue("Geometry", domainDock->saveGeometry());
    settings.endGroup();
    settings.beginGroup("NetworkDock");
    settings.setValue("DockArea", dockWidgetArea(networkDock));
    settings.setValue("Visible", networkDock->isVisible());
    settings.setValue("Floating", networkDock->isFloating());
    settings.setValue("Geometry", networkDock->saveGeometry());
    settings.endGroup();
    settings.beginGroup("StoragePoolDock");
    settings.setValue("DockArea", dockWidgetArea(storagePoolDock));
    settings.setValue("Visible", storagePoolDock->isVisible());
    settings.setValue("Floating", storagePoolDock->isFloating());
    settings.setValue("Geometry", storagePoolDock->saveGeometry());
    settings.endGroup();
    settings.beginGroup("SecretDock");
    settings.setValue("DockArea", dockWidgetArea(secretDock));
    settings.setValue("Visible", secretDock->isVisible());
    settings.setValue("Floating", secretDock->isFloating());
    settings.setValue("Geometry", secretDock->saveGeometry());
    settings.endGroup();
    settings.beginGroup("IfaceDock");
    settings.setValue("DockArea", dockWidgetArea(secretDock));
    settings.setValue("Visible", secretDock->isVisible());
    settings.setValue("Floating", secretDock->isFloating());
    settings.setValue("Geometry", secretDock->saveGeometry());
    settings.endGroup();
    settings.beginGroup("ConnectListColumns");
    settings.setValue("column0", connListWidget->columnWidth(0));
    settings.setValue("column1", connListWidget->columnWidth(1));
    settings.setValue("column2", connListWidget->columnWidth(2));
    settings.endGroup();
    settings.sync();
}
void MainWindow::closeEvent(QCloseEvent *ev)
{
    if ( !this->isVisible() ) changeVisibility();
    if ( runningConnExist() && wait_thread==nullptr ) {
        connListWidget->setEnabled(false);
        toolBar->setEnabled(false);
        logDock->setEnabled(false);
        domainDock->setEnabled(false);
        networkDock->setEnabled(false);
        storagePoolDock->setEnabled(false);
        secretDock->setEnabled(false);
        ifaceDock->setEnabled(false);
        domainsStateMonitor->stopMonitoring();
        taskWrHouse->stopTaskComputing();
        // close VM Displays
        QStringList keys(VM_Displayed_Map.keys());
        foreach ( QString key, keys ) {
            if ( VM_Displayed_Map.value(key, nullptr)!=nullptr ) {
                VM_Viewer *value = nullptr;
                QString _type =
                        VM_Displayed_Map.value(key, nullptr)->TYPE.toUpper();
                if ( _type=="LXC" ) {
                    value = static_cast<LXC_Viewer*>(
                                VM_Displayed_Map.value(key, nullptr));
                } else if ( _type=="SPICE" ) {
                    value = static_cast<Spice_Viewer*>(
                                VM_Displayed_Map.value(key, nullptr));
                } else if ( _type=="VNC" ) {
                    value = static_cast<VNC_Viewer*>(
                                VM_Displayed_Map.value(key, nullptr));
                };
                if ( nullptr!=value ) value->close();
                //qDebug()<<key<<"removed into Close";
            };
        };
        VM_Displayed_Map.clear();
        //qDebug()<<"Viewers cleared";
        // close StorageVolControls
        keys = Overviewed_StPool_Map.keys();
        foreach ( QString key, keys ) {
            if ( Overviewed_StPool_Map.value(key, nullptr)!=nullptr ) {
                VirtStorageVolControl *value = nullptr;
                value = static_cast<VirtStorageVolControl*>(
                                Overviewed_StPool_Map.value(key, nullptr));
                if ( nullptr!=value ) value->close();
                //qDebug()<<key<<"removed into Close";
            };
        };
        Overviewed_StPool_Map.clear();
        //qDebug()<<"StorageVolControls cleared";
        // close DomainEditors
        keys = DomainEditor_Map.keys();
        foreach ( QString key, keys ) {
            if ( DomainEditor_Map.value(key, nullptr)!=nullptr ) {
                CreateVirtDomain *value = nullptr;
                value = static_cast<CreateVirtDomain*>(
                                DomainEditor_Map.value(key, nullptr));
                if ( nullptr!=value ) value->close();
                //qDebug()<<key<<"removed into Close";
            };
        };
        DomainEditor_Map.clear();
        //qDebug()<<"DomainEditors cleared";
        wait_thread = new Wait(this, connListWidget);
        // stop virtEventLoop after closing all connections
        connect(wait_thread, SIGNAL(finished()),
                virtEventLoop, SLOT(stop()));
        wait_thread->start();
        ev->ignore();
        startCloseProcess();
    } else if ( !runningConnExist() &&
                (wait_thread==nullptr || !wait_thread->isRunning()) ) {
        saveSettings();
        taskWrHouse->hide();
        domainsStateMonitor->hide();
        trayIcon->hide();
        ev->accept();
    } else {
        //  ( wait_thread!=nullptr || wait_thread->isRunning() )
        ev->ignore();
    };
}
void MainWindow::startCloseProcess()
{
    //qDebug()<<"startCloseProcess";
    killTimerId = startTimer(PERIOD);
    //qDebug()<<killTimerId<<"killTimer";
}
void MainWindow::timerEvent(QTimerEvent *ev)
{
    if ( ev->timerId()==killTimerId ) {
        counter++;
        closeProgress->setValue(counter*PERIOD);
        // TODO: use instead 3*TIMEOUT the default from libvirt
        if ( waitAtClose*1000<counter*PERIOD ) {
            killTimer(killTimerId);
            killTimerId = 0;
            counter = 0;
            wait_thread->terminate();
        };
    };
}
void MainWindow::initTaskWareHouse()
{
    taskWrHouse = new TaskWareHouse();
    connect(taskWrHouse, SIGNAL(taskMsg(QString&)),
            this, SLOT(writeToErrorLog(QString&)));
}
void MainWindow::initDomainStateMonitor()
{
    domainsStateMonitor = new DomainStateMonitor();
}
void MainWindow::initTrayIcon()
{
    trayIcon = new TrayIcon(this);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(trayIcon->hideAction, SIGNAL(triggered()),
            this, SLOT(changeVisibility()));
    connect(trayIcon->logUpAction, SIGNAL(triggered()),
            this, SLOT(changeLogViewerVisibility()));
    connect(trayIcon->monitorAction, SIGNAL(triggered()),
            domainsStateMonitor, SLOT(changeVisibility()));
    connect(trayIcon->taskUpAction, SIGNAL(triggered()),
            taskWrHouse, SLOT(changeVisibility()));
    connect(trayIcon->closeAction, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(domainsStateMonitor, SIGNAL(visibilityChanged(bool)),
            trayIcon, SLOT(stateMonitorVisibilityChanged(bool)));
    connect(taskWrHouse, SIGNAL(visibilityChanged(bool)),
            trayIcon, SLOT(stateTaskWareHouseVisibilityChanged(bool)));
    trayIcon->stateMonitorVisibilityChanged(
                domainsStateMonitor->isVisible());
    trayIcon->stateTaskWareHouseVisibilityChanged(
                taskWrHouse->isVisible());
}
void MainWindow::mainWindowUp()
{
    if ( !this->isVisible() ) {
        changeVisibility();
        QTimer::singleShot(333, this, SLOT(autoHide()));
    };
}
void MainWindow::changeVisibility()
{
    if (this->isVisible()) {
        this->hide();
        trayIcon->hideAction->setText (QString("Up"));
        trayIcon->hideAction->setIcon (QIcon::fromTheme("up"));
        if ( domainDock->isFloating() ) domainDock->hide();
        if ( networkDock->isFloating() ) networkDock->hide();
        if ( storagePoolDock->isFloating() ) storagePoolDock->hide();
        if ( secretDock->isFloating() ) secretDock->hide();
        if ( ifaceDock->isFloating() ) secretDock->hide();
    } else {
        this->show();
        trayIcon->hideAction->setText (QString("Down"));
        trayIcon->hideAction->setIcon (QIcon::fromTheme("down"));
        if ( domainDock->isFloating() && toolBar->_domUpAction->isChecked() )
            domainDock->show();
        if ( networkDock->isFloating() && toolBar->_netUpAction->isChecked() )
            networkDock->show();
        if ( storagePoolDock->isFloating() && toolBar->_storageUpAction->isChecked() )
            storagePoolDock->show();
        if ( secretDock->isFloating() && toolBar->_secretsUpAction->isChecked() )
            secretDock->show();
        if ( ifaceDock->isFloating() && toolBar->_ifaceUpAction->isChecked() )
            ifaceDock->show();
    };
}
void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason r)
{
    if (r==QSystemTrayIcon::Trigger) changeVisibility();
}
void MainWindow::initConnListWidget()
{
    connListWidget = new ConnectionList(this);
    setCentralWidget(connListWidget);
    settings.beginGroup("ConnectListColumns");
    connListWidget->setColumnWidth(0, settings.value("column0", 132).toInt());
    connListWidget->setColumnWidth(1, settings.value("column1", 32).toInt());
    connListWidget->setColumnWidth(2, settings.value("column2", 32).toInt());
    settings.endGroup();
}
void MainWindow::initToolBar()
{
    bool again = settings.value("Donate", true).toBool();
    toolBar = new ToolBar(this, again);
    toolBar->setObjectName("toolBar");
    connect(toolBar->_hideAction, SIGNAL(triggered()),
            this, SLOT(changeVisibility()));
    connect(toolBar->_createAction, SIGNAL(triggered()),
            this, SLOT(createNewConnection()));
    connect(toolBar->_editAction, SIGNAL(triggered()),
            this, SLOT(editCurrentConnection()));
    connect(toolBar->_deleteAction, SIGNAL(triggered()),
            this, SLOT(deleteCurrentConnection()));
    connect(toolBar->_openAction, SIGNAL(triggered()),
            this, SLOT(openCurrentConnection()));
    connect(toolBar->_showAction, SIGNAL(triggered()),
            this, SLOT(showCurrentConnection()));
    connect(toolBar->_closeAction, SIGNAL(triggered()),
            this, SLOT(closeCurrentConnection()));
    connect(toolBar->_closeAllAction, SIGNAL(triggered()),
            this, SLOT(closeAllConnections()));
    connect(toolBar->_logUpAction, SIGNAL(triggered()),
            this, SLOT(changeLogViewerVisibility()));
    connect(toolBar->_closeOverview, SIGNAL(triggered()),
            connListWidget, SLOT(stopProcessing()));
    connect(toolBar->_closeOverview, SIGNAL(triggered()),
            this, SLOT(stopProcessing()));
    connect(toolBar->_exitAction, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(toolBar->_infoAction, SIGNAL(triggered()),
            this, SLOT(showAboutInfo()));
    connect(toolBar->_donateAction, SIGNAL(triggered()),
            this, SLOT(showDonateDialog()));
    connect(toolBar, SIGNAL(warningShowed()),
            this, SLOT(mainWindowUp()));
    int area_int = settings.value("ToolBarArea", 4).toInt();
    this->addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
}
void MainWindow::initDockWidgets()
{
    bool visible;
    Qt::DockWidgetArea area;
    logDock = new DockWidget(this);
    logDock->setObjectName("logDock");
    logDock->setWindowTitle("Log");
    logDock->setFeatures(
        QDockWidget::DockWidgetMovable   |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar
    );
    logHeadWdg = new DockHeadWidget(this, "Events & Errors");
    logHeadWdg->setTabBarName("utilities-log-viewer");
    logDock->setTitleBarWidget(logHeadWdg);
    connect(logHeadWdg, SIGNAL(floatChanged(bool)),
            logDock, SLOT(_setFloating(bool)));
    connect(logDock, SIGNAL(topLevelChanged(bool)),
            logHeadWdg, SLOT(floatStateChanged(bool)));
    logDockContent = new LogDock(this);
    logDock->setWidget( logDockContent );
    settings.beginGroup("LogDock");
    logDock->setFloating(settings.value("Floating", false).toBool());
    logDock->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", true).toBool();
    logDock->setVisible(visible);
    connect(logDockContent, SIGNAL(overflow(bool)),
            logDock, SLOT(changeWarningState(bool)));
    connect(logDockContent, SIGNAL(overflow(bool)),
            trayIcon, SLOT(changeWarningState(bool)));
    toolBar->_logUpAction->setChecked(visible);
    area = getDockArea(settings.value("DockArea", Qt::BottomDockWidgetArea).toInt());
    settings.endGroup();
    addDockWidget(area, logDock);

    domainDock = new DockWidget(this);
    domainDock->setObjectName("domainDock");
    domainDock->setWindowTitle("Domain");
    domainDock->setWindowIcon(QIcon::fromTheme("domain"));
    domainDock->setFeatures(
        QDockWidget::DockWidgetMovable   |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar
    );
    domHeadWdg = new DockHeadWidget(this, "Virtual Machines");
    domHeadWdg->setTabBarName("domain");
    domainDock->setTitleBarWidget(domHeadWdg);
    connect(domHeadWdg, SIGNAL(floatChanged(bool)),
            domainDock, SLOT(_setFloating(bool)));
    connect(domainDock, SIGNAL(topLevelChanged(bool)),
            domHeadWdg, SLOT(floatStateChanged(bool)));
    domainDockContent = new VirtDomainControl(this);
    domainDock->setWidget( domainDockContent );
    settings.beginGroup("DomainDock");
    domainDock->setFloating(settings.value("Floating", false).toBool());
    domainDock->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", true).toBool();
    domainDock->setVisible(visible);
    toolBar->_domUpAction->setChecked(visible);
    area = getDockArea(settings.value("DockArea", Qt::BottomDockWidgetArea).toInt());
    settings.endGroup();
    addDockWidget(area, domainDock);
    tabifyDockWidget(logDock, domainDock);
    connect(toolBar->_domUpAction, SIGNAL(triggered(bool)),
            domainDock, SLOT(setVisible(bool)));
    connect(domainDockContent, SIGNAL(entityMsg(QString&)),
            this, SLOT(writeToErrorLog(QString&)));
    connect(domainDockContent, SIGNAL(displayRequest(TASK)),
            this, SLOT(invokeVMDisplay(TASK)));
    connect(domainDockContent, SIGNAL(addToStateMonitor(virConnectPtr*,QString&,QString&)),
            domainsStateMonitor, SLOT(setNewMonitoredDomain(virConnectPtr*,QString&,QString&)));
    connect(domainDockContent, SIGNAL(migrateToConnect(TASK)),
            this, SLOT(buildMigrateArgs(TASK)));
    connect(domainDockContent, SIGNAL(addNewTask(TASK)),
            taskWrHouse, SLOT(addNewTask(TASK)));
    connect(taskWrHouse, SIGNAL(domResult(Result)),
            domainDockContent, SLOT(resultReceiver(Result)));
    connect(connListWidget, SIGNAL(domResult(Result)),
            domainDockContent, SLOT(resultReceiver(Result)));
    connect(domainDockContent, SIGNAL(domainToEditor(TASK)),
            this, SLOT(invokeDomainEditor(TASK)));

    networkDock = new DockWidget(this);
    networkDock->setObjectName("networkDock");
    networkDock->setWindowTitle("Network");
    networkDock->setWindowIcon(QIcon::fromTheme("network"));
    networkDock->setFeatures(
        QDockWidget::DockWidgetMovable   |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar
    );
    netHeadWdg = new DockHeadWidget(this, "Virtual Networks");
    netHeadWdg->setTabBarName("network");
    networkDock->setTitleBarWidget(netHeadWdg);
    connect(netHeadWdg, SIGNAL(floatChanged(bool)),
            networkDock, SLOT(_setFloating(bool)));
    connect(networkDock, SIGNAL(topLevelChanged(bool)),
            netHeadWdg, SLOT(floatStateChanged(bool)));
    networkDockContent = new VirtNetControl(this);
    networkDock->setWidget( networkDockContent );
    settings.beginGroup("NetworkDock");
    networkDock->setFloating(settings.value("Floating", false).toBool());
    networkDock->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    networkDock->setVisible(visible);
    toolBar->_netUpAction->setChecked(visible);
    area = getDockArea(settings.value("DockArea", Qt::BottomDockWidgetArea).toInt());
    settings.endGroup();
    addDockWidget(area, networkDock);
    tabifyDockWidget(domainDock, networkDock);
    connect(toolBar->_netUpAction, SIGNAL(triggered(bool)),
            networkDock, SLOT(setVisible(bool)));
    connect(networkDockContent, SIGNAL(entityMsg(QString&)),
            this, SLOT(writeToErrorLog(QString&)));
    connect(networkDockContent, SIGNAL(addNewTask(TASK)),
            taskWrHouse, SLOT(addNewTask(TASK)));
    connect(taskWrHouse, SIGNAL(netResult(Result)),
            networkDockContent, SLOT(resultReceiver(Result)));
    connect(connListWidget, SIGNAL(netResult(Result)),
            networkDockContent, SLOT(resultReceiver(Result)));

    storagePoolDock = new DockWidget(this);
    storagePoolDock->setObjectName("storagePoolDock");
    storagePoolDock->setWindowTitle("StoragePool");
    storagePoolDock->setFeatures(
        QDockWidget::DockWidgetMovable   |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar
    );
    poolHeadWdg = new DockHeadWidget(this, "Storage Pools");
    poolHeadWdg->setTabBarName("storage");
    storagePoolDock->setTitleBarWidget(poolHeadWdg);
    connect(poolHeadWdg, SIGNAL(floatChanged(bool)),
            storagePoolDock, SLOT(_setFloating(bool)));
    connect(storagePoolDock, SIGNAL(topLevelChanged(bool)),
            poolHeadWdg, SLOT(floatStateChanged(bool)));
    storagePoolDockContent = new VirtStoragePoolControl(this);
    storagePoolDock->setWidget( storagePoolDockContent );
    settings.beginGroup("StoragePoolDock");
    storagePoolDock->setFloating(settings.value("Floating", false).toBool());
    storagePoolDock->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    storagePoolDock->setVisible(visible);
    toolBar->_storageUpAction->setChecked(visible);
    area = getDockArea(settings.value("DockArea", Qt::BottomDockWidgetArea).toInt());
    settings.endGroup();
    addDockWidget(area, storagePoolDock);
    tabifyDockWidget(networkDock, storagePoolDock);
    connect(toolBar->_storageUpAction, SIGNAL(triggered(bool)),
            storagePoolDock, SLOT(setVisible(bool)));
    connect(storagePoolDockContent, SIGNAL(entityMsg(QString&)),
            this, SLOT(writeToErrorLog(QString&)));
    connect(storagePoolDockContent, SIGNAL(addNewTask(TASK)),
            taskWrHouse, SLOT(addNewTask(TASK)));
    connect(taskWrHouse, SIGNAL(poolResult(Result)),
            storagePoolDockContent, SLOT(resultReceiver(Result)));
    connect(storagePoolDockContent, SIGNAL(overviewStPool(virConnectPtr*,QString&,QString&)),
            this, SLOT(overviewStoragePool(virConnectPtr*,QString&,QString&)));

    secretDock = new DockWidget(this);
    secretDock->setObjectName("secretDock");
    secretDock->setWindowTitle("Secret");
    secretDock->setFeatures(
        QDockWidget::DockWidgetMovable   |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar
    );
    scrtHeadWdg = new DockHeadWidget(this, "Secrets");
    scrtHeadWdg->setTabBarName("security");
    secretDock->setTitleBarWidget(scrtHeadWdg);
    connect(scrtHeadWdg, SIGNAL(floatChanged(bool)),
            secretDock, SLOT(_setFloating(bool)));
    connect(secretDock, SIGNAL(topLevelChanged(bool)),
            scrtHeadWdg, SLOT(floatStateChanged(bool)));
    secretDockContent = new VirtSecretControl(this);
    secretDock->setWidget( secretDockContent );
    settings.beginGroup("SecretDock");
    secretDock->setFloating(settings.value("Floating", false).toBool());
    secretDock->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    secretDock->setVisible(visible);
    toolBar->_secretsUpAction->setChecked(visible);
    area = getDockArea(settings.value("DockArea", Qt::BottomDockWidgetArea).toInt());
    settings.endGroup();
    addDockWidget(area, secretDock);
    tabifyDockWidget(storagePoolDock, secretDock);
    connect(toolBar->_secretsUpAction, SIGNAL(triggered(bool)),
            secretDock, SLOT(setVisible(bool)));
    connect(secretDockContent, SIGNAL(entityMsg(QString&)),
            this, SLOT(writeToErrorLog(QString&)));
    connect(secretDockContent, SIGNAL(addNewTask(TASK)),
            taskWrHouse, SLOT(addNewTask(TASK)));
    connect(taskWrHouse, SIGNAL(secResult(Result)),
            secretDockContent, SLOT(resultReceiver(Result)));

    ifaceDock = new DockWidget(this);
    ifaceDock->setObjectName("ifaceDock");
    ifaceDock->setWindowTitle("Interface");
    ifaceDock->setFeatures(
        QDockWidget::DockWidgetMovable   |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar
    );
    ifaceHeadWdg = new DockHeadWidget(this, "Interfaces");
    ifaceHeadWdg->setTabBarName("network-wired");
    ifaceDock->setTitleBarWidget(ifaceHeadWdg);
    connect(ifaceHeadWdg, SIGNAL(floatChanged(bool)),
            ifaceDock, SLOT(_setFloating(bool)));
    connect(ifaceDock, SIGNAL(topLevelChanged(bool)),
            ifaceHeadWdg, SLOT(floatStateChanged(bool)));
    ifaceDockContent = new VirtInterfaceControl(this);
    ifaceDock->setWidget( ifaceDockContent );
    settings.beginGroup("IfaceDock");
    ifaceDock->setFloating(settings.value("Floating", false).toBool());
    ifaceDock->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    ifaceDock->setVisible(visible);
    toolBar->_ifaceUpAction->setChecked(visible);
    area = getDockArea(settings.value("DockArea", Qt::BottomDockWidgetArea).toInt());
    settings.endGroup();
    addDockWidget(area, ifaceDock);
    tabifyDockWidget(secretDock, ifaceDock);
    connect(toolBar->_ifaceUpAction, SIGNAL(triggered(bool)),
            ifaceDock, SLOT(setVisible(bool)));
    connect(ifaceDockContent, SIGNAL(entityMsg(QString&)),
            this, SLOT(writeToErrorLog(QString&)));
    connect(ifaceDockContent, SIGNAL(addNewTask(TASK)),
            taskWrHouse, SLOT(addNewTask(TASK)));
    connect(taskWrHouse, SIGNAL(ifaceResult(Result)),
            ifaceDockContent, SLOT(resultReceiver(Result)));

    domainDockContent->setEnabled(false);
    networkDockContent->setEnabled(false);
    storagePoolDockContent->setEnabled(false);
    secretDockContent->setEnabled(false);
    ifaceDockContent->setEnabled(false);
}
void MainWindow::initVirEventloop()
{
    virtEventLoop = new VirtEventLoop(this);
    // close application after closing virtEventLoop
    connect(virtEventLoop, SIGNAL(finished()),
            this, SLOT(close()));
    connect(virtEventLoop, SIGNAL(errorMsg(QString&,uint)),
            this, SLOT(writeToErrorLog(QString&,uint)));
    connect(virtEventLoop, SIGNAL(result(bool)),
             this, SLOT(initConnections(bool)));
    virtEventLoop->start();
}
void MainWindow::initConnections(bool started)
{
    QString time = QTime::currentTime().toString();
    QString title("App initialization");
    QString currMsg = QString("<b>%1 %2:</b><br><font color='blue'>\
                               <b>EVENT</b></font>: virtEventLoop%3%4")
            .arg(time).arg(title).arg(!started?" not":"").arg(" started");
    if ( !started ) return;
    logDockContent->appendMsgToLog(currMsg);
    settings.beginGroup("Connects");
    QStringList groups = settings.childGroups();
    settings.endGroup();
    QList<QString>::const_iterator i;
    for (i=groups.constBegin(); i!=groups.constEnd(); ++i) {
        QString s = (*i);
        connListWidget->addConnItem(s);
    };
    connListWidget->searchLocalhostConnections();
    connect(connListWidget, SIGNAL(removeConnection(QString&)),
            this, SLOT(removeConnItem(QString&)));
    connect(connListWidget, SIGNAL(messageShowed()),
            this, SLOT(mainWindowUp()));
    connect(connListWidget, SIGNAL(warning(QString&)),
            this, SLOT(writeToErrorLog(QString&)));
    connect(connListWidget, SIGNAL(connPtrPtr(virConnectPtr*, QString&)),
            this, SLOT(receiveConnPtrPtr(virConnectPtr*, QString&)));
    connect(connListWidget, SIGNAL(connClosed(bool, QString&)),
            this, SLOT(stopConnProcessing(bool, QString&)));
    connect(connListWidget, SIGNAL(connToClose(int)),
            this, SLOT(closeConnGenerations(int)));
    connect(connListWidget, SIGNAL(domainEnd(QString&)),
            this, SLOT(deleteVMDisplay(QString&)));
    currMsg = QString("<b>%1 %2:</b><br><font color='blue'>\
                       <b>EVENT</b></font>: %3")
            .arg(time).arg(title).arg("Connections inited");
    logDockContent->appendMsgToLog(currMsg);
}
void MainWindow::editCurrentConnection()
{
    connListWidget->connItemEditAction();
}
void MainWindow::createNewConnection()
{
    QString s = QString("<noname>");
    connListWidget->addConnItem(s);
}
void MainWindow::deleteCurrentConnection()
{
    connListWidget->deleteCurrentConnection();
}
void MainWindow::removeConnItem(QString &connect)
{
    settings.beginGroup("Connects");
    settings.remove(connect);
    settings.endGroup();
    //qDebug()<<connect<<"connect deleted";
}
void MainWindow::openCurrentConnection()
{
    QModelIndex _item = connListWidget->currentIndex();
    if (_item.isValid()) {
        connListWidget->openConnection(_item);
    };
}
void MainWindow::showCurrentConnection()
{
    QModelIndex _item = connListWidget->currentIndex();
    if (_item.isValid()) {
        connListWidget->showConnection(_item);
    };
}
void MainWindow::closeCurrentConnection()
{
    QModelIndex _item = connListWidget->currentIndex();
    if (_item.isValid()) {
        closeConnection(_item.row());
    };
}
void MainWindow::closeAllConnections()
{
    int count = connListWidget->connItemModel->rowCount();
    for (int i = 0; i< count; i++) closeConnection(i);
}
void MainWindow::closeConnection(int i)
{
    //qDebug()<<i<<" item to stop";
    QModelIndex _item = connListWidget->connItemModel->index(i, 0);
    if (_item.isValid()) {
        connListWidget->closeConnection(_item);
    };
}
void MainWindow::closeConnGenerations(int i)
{
    ConnItemIndex *idx = static_cast<ConnItemIndex*>(
                connListWidget->connItemModel->
                connItemDataList.at(i));
    if ( idx!=nullptr ) {
        QString conn_to_close = idx->getName();
        closeConnGenerations(conn_to_close);
    };
}
void MainWindow::closeConnGenerations(QString &_connName)
{
    // WARNING: the policy for close a Viewers, Editors :
    // close the connection -- close all of its generation
    foreach (QString key, VM_Displayed_Map.keys()) {
        if ( key.startsWith(_connName) ) {
            VM_Displayed_Map.value(key)->close();
            VM_Displayed_Map.remove(key);
        };
    };
    foreach (QString key, Overviewed_StPool_Map.keys()) {
        if ( key.startsWith(_connName) ) {
            Overviewed_StPool_Map.value(key)->close();
            Overviewed_StPool_Map.remove(key);
        };
    };
    foreach (QString key, DomainEditor_Map.keys()) {
        if ( key.startsWith(_connName) ) {
            DomainEditor_Map.value(key)->close();
            DomainEditor_Map.remove(key);
        };
    };
}
bool MainWindow::runningConnExist()
{
    bool result = false;
    int count = connListWidget->connItemModel->rowCount();
    for (int i=0; i<count; i++) {
        ConnItemIndex *item = connListWidget->connItemModel->connItemDataList.at(i);
        //qDebug()<<connListWidget->item(i)->text()<< connListWidget->item(i)->data(Qt::UserRole).toMap().value("isRunning").toInt();
        if ( item->getData().value("isRunning").toInt()==RUNNING ) {
            result = true;
            break;
        };
    };
    return result;
}
void MainWindow::autoHide()
{
    if (this->isVisible()) changeVisibility();
}
void MainWindow::writeToErrorLog(QString &msg)
{
    logDockContent->appendMsgToLog(msg);
}
void MainWindow::writeToErrorLog(QString &msg, uint _number)
{
    Q_UNUSED(_number);
    QString time = QTime::currentTime().toString();
    QString title("Libvirt EventLoop");
    QString currMsg = QString("<b>%1 %2:</b><br><font color='red'><b>ERROR</b></font>: %3")
            .arg(time).arg(title).arg(msg);
    logDockContent->appendMsgToLog(currMsg);
}
void MainWindow::changeLogViewerVisibility()
{
    QString text;
    if ( logDock->isVisible() ) {
        logDock->hide();
        text = "Show Log Viewer";
    } else {
        logDock->show();
        text = "Hide Log Viewer";
    };
    trayIcon->setLogUpActionText(text);
    toolBar->_logUpAction->setChecked(logDock->isVisible());
}

Qt::DockWidgetArea MainWindow::getDockArea(int i) const
{
    Qt::DockWidgetArea result;
    switch (i) {
    case 1:
        result = Qt::LeftDockWidgetArea;
        break;
    case 2:
        result = Qt::RightDockWidgetArea;
        break;
    case 4:
        result = Qt::TopDockWidgetArea;
        break;
    case 8:
        result = Qt::BottomDockWidgetArea;
        break;
    default:
        result = Qt::AllDockWidgetAreas;
        break;
    };
    return result;
}
void MainWindow::receiveConnPtrPtr(virConnectPtr *_connPtrPtr, QString &name)
{
    //qDebug()<<"receiveConnPtrPtr:"<<(*_connPtrPtr);
    // send connect ptr to all related virtual resources for operating
    if ( domainDockContent->setCurrentWorkConnect(_connPtrPtr) )
        domainDockContent->setListHeader(name);
    if ( networkDockContent->setCurrentWorkConnect(_connPtrPtr) )
        networkDockContent->setListHeader(name);
    if ( storagePoolDockContent->setCurrentWorkConnect(_connPtrPtr) )
        storagePoolDockContent->setListHeader(name);
    if ( secretDockContent->setCurrentWorkConnect(_connPtrPtr) )
        secretDockContent->setListHeader(name);
    if ( ifaceDockContent->setCurrentWorkConnect(_connPtrPtr) )
        ifaceDockContent->setListHeader(name);
}
void MainWindow::stopConnProcessing(bool onView, QString &_connName)
{
    // clear Overview Docks if closed connect is on View
    if ( onView ) {
        stopProcessing();
    };
    // close all of its generation
    closeConnGenerations(_connName);
}
void MainWindow::stopProcessing()
{
    // stop processing of all virtual resources
    domainDockContent->stopProcessing();
    networkDockContent->stopProcessing();
    storagePoolDockContent->stopProcessing();
    secretDockContent->stopProcessing();
    ifaceDockContent->stopProcessing();
}
void MainWindow::invokeVMDisplay(TASK _task)
{
    virConnectPtr *connPtrPtr = _task.srcConnPtr;
    QString connName = _task.srcConName;
    QString domName = _task.object;
    QString type = _task.type;
    QString viewerType = _task.method;
    // WARNING: key must starts with connection name
    // see for: MainWindow::closeConnGenerations(QString &_connName)
    QString key = QString("%1_%2").arg(connName).arg(domName);
    if ( !VM_Displayed_Map.contains(key) ) {
        //qDebug()<<key<<"vm invoked"<<"new";
        if ( type.toLower()=="lxc" ) {
            VM_Displayed_Map.insert(
                        key,
                        new LXC_Viewer(nullptr, connPtrPtr, connName, domName));
        } else if ( viewerType=="vnc" ) {
            VM_Displayed_Map.insert(
                        key,
                        new VNC_Viewer(nullptr, connPtrPtr, connName, domName));
        } else if ( viewerType=="spice" ) {
            VM_Displayed_Map.insert(
                        key,
                        new Spice_Viewer(nullptr, connPtrPtr, connName, domName));
        } else {
            QMessageBox::information(
                        this,
                        "VM Viewer",
                        QString("Not implemented type: %1\n or viewer: %2")
                        .arg(type).arg(viewerType));
            return;
        };
        VM_Displayed_Map.value(key)->setObjectName(key);
        connect(VM_Displayed_Map.value(key), SIGNAL(finished(QString&)),
                this, SLOT(deleteVMDisplay(QString&)));
        connect(VM_Displayed_Map.value(key), SIGNAL(errorMsg(QString&)),
                logDockContent, SLOT(appendMsgToLog(QString&)));
        connect(VM_Displayed_Map.value(key), SIGNAL(addNewTask(TASK)),
                taskWrHouse, SLOT(addNewTask(TASK)));
        VM_Displayed_Map.value(key)->show();
    } else {
        //qDebug()<<key<<"vm invoked"<<"exist";
        if ( VM_Displayed_Map.value(key)!=nullptr )
            VM_Displayed_Map.value(key)->show();
        else VM_Displayed_Map.remove(key);
    };
}
void MainWindow::deleteVMDisplay(QString &key)
{
    if ( VM_Displayed_Map.contains(key) ) {
        VM_Viewer *value = nullptr;
        QString _type =
                VM_Displayed_Map.value(key, nullptr)->TYPE.toUpper();
        if ( _type!="LXC" ) {
            value = static_cast<VM_Viewer*>(
                        VM_Displayed_Map.value(key, nullptr));
            if ( nullptr!=value ) {
                delete value;
                value = nullptr;
            };
        };
        VM_Displayed_Map.remove(key);
    }
}
void MainWindow::buildMigrateArgs(TASK _task)
{
    virConnectPtr *namedConnect = connListWidget->getPtr_connectionPtr(_task.args.path);
    if ( nullptr!=namedConnect ) {
        domainDockContent->execMigrateAction(namedConnect, _task);
    } else {
        QString time = QTime::currentTime().toString();
        QString title = QString("Domain Migration '%1'")
                .arg(_task.object);
        QString msg = QString("Migation not possible to NULL connection.");
        QString currMsg = QString(
                    "<b>%1 %2:</b><br><font color='red'><b>ERROR</b></font>: %3")
                .arg(time).arg(title).arg(msg);
        logDockContent->appendMsgToLog(currMsg);
    }
}

void MainWindow::overviewStoragePool(virConnectPtr *connPtrPtr, QString &connName, QString &poolName)
{
    // WARNING: key must starts with connection name
    // see for: MainWindow::closeConnGenerations(QString &_connName)
    QString key = QString("%1_%2").arg(connName).arg(poolName);
    if ( !Overviewed_StPool_Map.contains(key) ) {
        Overviewed_StPool_Map.insert(key, new VirtStorageVolControl());
        Overviewed_StPool_Map.value(key)->setObjectName(key);
        Overviewed_StPool_Map.value(key)->setWindowTitle(QString("%1 Pool").arg(key));
        connect(Overviewed_StPool_Map.value(key), SIGNAL(entityMsg(QString&)),
                this, SLOT(writeToErrorLog(QString&)));
        connect(Overviewed_StPool_Map.value(key), SIGNAL(finished(QString&)),
                this, SLOT(deleteStPoolOverview(QString&)));
        connect(Overviewed_StPool_Map.value(key), SIGNAL(addNewTask(TASK)),
                taskWrHouse, SLOT(addNewTask(TASK)));
        connect(taskWrHouse, SIGNAL(volResult(Result)),
                Overviewed_StPool_Map.value(key), SLOT(resultReceiver(Result)));
        Overviewed_StPool_Map.value(key)->setCurrentStoragePool(connPtrPtr, connName, poolName);
    };
    if ( Overviewed_StPool_Map.value(key)!=nullptr ) {
        Overviewed_StPool_Map.value(key)->show();
        Overviewed_StPool_Map.value(key)->setFocus();
    } else
        Overviewed_StPool_Map.value(key);
}
void MainWindow::deleteStPoolOverview(QString &key)
{
    if ( Overviewed_StPool_Map.contains(key) ) {
        // set attribute(Qt::WA_DeleteOnClose) in widget;
        // not need to delete a VirtStorageVolControl
        Overviewed_StPool_Map.remove(key);
    };
}
void MainWindow::invokeDomainEditor(TASK _task)
{
    QString connName = _task.srcConName;
    QString domName = _task.object;
    // WARNING: key must starts with connection name
    // see for: MainWindow::closeConnGenerations(QString &_connName)
    QString key = QString("%1_%2").arg(connName).arg(domName);
    if ( !DomainEditor_Map.contains(key) ) {
        DomainEditor_Map.insert(key, new CreateVirtDomain(nullptr, _task));
        DomainEditor_Map.value(key)->setObjectName(key);
        DomainEditor_Map.value(key)->setWindowTitle(
                    QString("VM Settings / <%1> in [%2]")
                    .arg(domName).arg(connName));
        connect(DomainEditor_Map.value(key), SIGNAL(errorMsg(QString&)),
                this, SLOT(writeToErrorLog(QString&)));
        connect(DomainEditor_Map.value(key), SIGNAL(finished(QString&)),
                this, SLOT(deleteDomainEditor(QString&)));
        connect(DomainEditor_Map.value(key), SIGNAL(addNewTask(TASK)),
                taskWrHouse, SLOT(addNewTask(TASK)));
    };
    if ( DomainEditor_Map.value(key)!=nullptr ) {
        DomainEditor_Map.value(key)->show();
        DomainEditor_Map.value(key)->setFocus();
    } else
        DomainEditor_Map.remove(key);
}
void MainWindow::deleteDomainEditor(QString &key)
{
    if ( DomainEditor_Map.contains(key) ) {
        // set attribute(Qt::WA_DeleteOnClose) in widget;
        // not need to delete a CreateVirtDomain
        DomainEditor_Map.remove(key);
    };
}
void MainWindow::showAboutInfo()
{
    QString message = QString(
                "Qt Virtual machine manager,\
                \nversion %1.\
                \nBased on Qt %2.\
                \nUsed libvirt (%3) API.\
                \nImplemented graphical consoles for\
                \nVirtual Machine displays\
                \n(by SPICE/VNC client)\
                \nand LXC terminals.")
                .arg(QString("%1.%2.%3")
                     .arg(VERSION_MAJOR)
                     .arg(VERSION_MIDDLE)
                     .arg(VERSION_MINOR))
                .arg(QT_VERSION_STR)
                .arg(virtEventLoop->libVersion);
    QMessageBox::about(
                this,
                "About QtVirtManager",
                message);
}
void MainWindow::showDonateDialog()
{
    Donate_Dialog *dd = new Donate_Dialog(this);
    dd->exec();
    bool again = dd->showAgain();
    dd->deleteLater();
    settings.setValue("Donate", again);
    toolBar->_donateAction->setVisible(again);
}
