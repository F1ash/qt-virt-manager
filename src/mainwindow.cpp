#include "mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setSizePolicy(
                QSizePolicy(
                    QSizePolicy::MinimumExpanding,
                    QSizePolicy::MinimumExpanding));
    setMinimumSize(100, 100);
    setContentsMargins(0, 0, 0, 5);
    setWindowTitle(tr("Qt VirtManager"));
    QIcon::setThemeName("QtVirtManager");
    setWindowIcon(QIcon::fromTheme("virtual-engineering"));
    //setMouseTracking(true);
    setDockOptions(
                QMainWindow::AnimatedDocks |
                QMainWindow::ForceTabbedDocks);
    migrate_settings_to_INI_format();
    int _viewMode = settings.value("ViewMode", 0).toInt();
    viewMode = static_cast<VIEW_MODE>(_viewMode);
    restoreGeometry(settings.value("Geometry").toByteArray());
    proxyWdg = new ProxyWidget(nullptr);
    proxyWdg->setUsedViewMode(viewMode);
    proxyLayout = new QHBoxLayout();
    proxyWdg->setLayout(proxyLayout);
    SoftTouchedWdg = nullptr;
    reloadFlag = true; // for first initiation of connections
    initTaskWareHouse();
    initDomainStateMonitor();
    initTrayIcon();
    initMenuBar();
    initConnListWidget();
    initDockWidgets();
    this->setVisible(!settings.value("Visible", false).toBool());
    changeVisibility();
    waitAtClose = settings.value("WaitAtClose", 180).toInt();
    closeProgress = new QProgressBar(this);
    closeProgress->setRange(0, waitAtClose*1000);
    closeProgress->setToolTip(
                tr("Progress for waiting the connection close"));
    statusBar()->addPermanentWidget(closeProgress);
    statusBar()->hide();
    wait_thread = nullptr;
    initVirEventloop();
}

void MainWindow::saveSettings()
{
    taskWrHouse->saveCurrentState();
    domainsStateMonitor->saveCurrentState();
    settings.setValue("ViewMode", static_cast<int>(viewMode));
    settings.setValue("Visible", this->isVisible());
    settings.setValue("WaitAtClose", waitAtClose);
    switch (viewMode) {
    case SOFT_TOUCHED:
        settings.beginGroup("SOFT_TOUCHED_MODE");
        break;
    case HARD_CLASSIC:
    default:
        break;
    };
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("State", saveState());
    settings.setValue(
                "ToolBarArea",
                 connListWidget->toolBarArea(
                          connListWidget->toolBar));
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
    settings.beginGroup("NWFilterDock");
    settings.setValue("DockArea", dockWidgetArea(secretDock));
    settings.setValue("Visible", secretDock->isVisible());
    settings.setValue("Floating", secretDock->isFloating());
    settings.setValue("Geometry", secretDock->saveGeometry());
    settings.endGroup();
    settings.beginGroup("ConnectListColumns");
    settings.setValue("column0", connListWidget->list->columnWidth(0));
    settings.setValue("column1", connListWidget->list->columnWidth(1));
    settings.setValue("column2", connListWidget->list->columnWidth(2));
    settings.endGroup();
    switch (viewMode) {
    case SOFT_TOUCHED:
        settings.endGroup();
        break;
    case HARD_CLASSIC:
    default:
        break;
    };
    settings.sync();
}
void MainWindow::closeEvent(QCloseEvent *ev)
{
    if ( !this->isVisible() ) changeVisibility();
    if ( wait_thread==nullptr ) {
        connListWidget->list->setEnabled(false);
        connListWidget->toolBar->setEnabled(false);
        logDock->setEnabled(false);
        domainDock->setEnabled(false);
        networkDock->setEnabled(false);
        storagePoolDock->setEnabled(false);
        secretDock->setEnabled(false);
        ifaceDock->setEnabled(false);
        nwfilterDock->setEnabled(false);
        domainsStateMonitor->stopMonitoring();
        taskWrHouse->stopTaskComputing();
        menuBar->fileMenu->setEnabled(false);
        saveSettings();
        // close VM Displays
        QStringList keys(VM_Displayed_Map.keys());
        foreach ( QString key, keys ) {
            if ( VM_Displayed_Map.value(key, nullptr)!=nullptr ) {
                VM_Viewer *value = nullptr;
                QString _type =
                        VM_Displayed_Map.value(key, nullptr)->TYPE.toUpper();
                if ( _type.compare("LXC")==0 ) {
#if WITH_LXC_SUPPORT
                    value = static_cast<LXC_Viewer*>(
                                VM_Displayed_Map.value(key, nullptr));
#endif
                } else if ( _type.compare("SPICE")==0 ) {
#if WITH_SPICE_SUPPORT
                    value = static_cast<Spice_Viewer*>(
                                VM_Displayed_Map.value(key, nullptr));
#endif
                } else if ( _type.compare("VNC")==0 ) {
#if WITH_VNC_SUPPORT
                    value = static_cast<VNC_Viewer*>(
                                VM_Displayed_Map.value(key, nullptr));
#endif
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
        // close NetworkEditors
        keys = NetworkEditor_Map.keys();
        foreach ( QString key, keys ) {
            if ( NetworkEditor_Map.value(key, nullptr)!=nullptr ) {
                CreateVirtNetwork *value = nullptr;
                value = static_cast<CreateVirtNetwork*>(
                                NetworkEditor_Map.value(key, nullptr));
                if ( nullptr!=value ) value->close();
                //qDebug()<<key<<"removed into Close";
            };
        };
        NetworkEditor_Map.clear();
        //qDebug()<<"NetworkEditors cleared";
        // close NWFilterEditors
        keys = NWFilterEditor_Map.keys();
        foreach ( QString key, keys ) {
            if ( NWFilterEditor_Map.value(key, nullptr)!=nullptr ) {
                CreateVirtNWFilter *value = nullptr;
                value = static_cast<CreateVirtNWFilter*>(
                                NWFilterEditor_Map.value(key, nullptr));
                if ( nullptr!=value ) value->close();
                //qDebug()<<key<<"removed into Close";
            };
        };
        NWFilterEditor_Map.clear();
        //qDebug()<<"NWFilterEditors cleared";
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
        wait_thread = new Wait(this, connListWidget->list);
        // stop virtEventLoop after closing all connections;
        // used 'terminate', because 'virEventRunDefaultImpl'
        // not occures when libvirt unavailable.
        connect(wait_thread, SIGNAL(finished()),
                virtEventLoop, SLOT(terminate()));
        wait_thread->start();
        ev->ignore();
        startCloseProcess();
    } else if ( virtEventLoop->isRunning() ) {
        // used 'terminate', because 'virEventRunDefaultImpl'
        // not occures when libvirt unavailable.
        virtEventLoop->terminate();
        ev->ignore();
    } else if ( !runningConnExist() &&
                (wait_thread==nullptr || !wait_thread->isRunning()) ) {
        trayIcon->hide();
        foreach (QWidget *widget, qApp->allWidgets())
            widget->hide();
        ev->accept();
    } else {
        //  ( wait_thread!=nullptr || wait_thread->isRunning() )
        ev->accept();
    };
}
void MainWindow::startCloseProcess()
{
    //qDebug()<<"startCloseProcess";
    statusBar()->show();
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
    connect(taskWrHouse, SIGNAL(taskMsg(const QString&)),
            this, SLOT(writeToErrorLog(const QString&)));
}
void MainWindow::initDomainStateMonitor()
{
    domainsStateMonitor = new DomainStateMonitor();
}
void MainWindow::initTrayIcon()
{
    trayIcon = new TrayIcon(this);
    connect(trayIcon,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(trayIcon->hideAction, SIGNAL(triggered()),
            this, SLOT(changeVisibility()));
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
        trayIcon->hideAction->setText (tr("Up"));
        trayIcon->hideAction->setIcon (QIcon::fromTheme("up"));
        if ( logDock->isFloating() ) logDock->hide();
        if ( domainDock->isFloating() ) domainDock->hide();
        if ( networkDock->isFloating() ) networkDock->hide();
        if ( storagePoolDock->isFloating() ) storagePoolDock->hide();
        if ( secretDock->isFloating() ) secretDock->hide();
        if ( ifaceDock->isFloating() ) ifaceDock->hide();
        if ( nwfilterDock->isFloating() ) nwfilterDock->hide();
    } else {
        this->show();
        trayIcon->hideAction->setText (tr("Down"));
        trayIcon->hideAction->setIcon (QIcon::fromTheme("down"));
        if ( logDock->isFloating()
             && menuBar->dockMenu->logAct->isChecked() )
            logDock->show();
        if ( domainDock->isFloating()
             && menuBar->dockMenu->domainAct->isChecked() )
            domainDock->show();
        if ( networkDock->isFloating()
             && menuBar->dockMenu->networkAct->isChecked() )
            networkDock->show();
        if ( storagePoolDock->isFloating()
             && menuBar->dockMenu->storageAct->isChecked() )
            storagePoolDock->show();
        if ( secretDock->isFloating()
             && menuBar->dockMenu->secretAct->isChecked() )
            secretDock->show();
        if ( ifaceDock->isFloating()
             && menuBar->dockMenu->ifaceAct->isChecked() )
            ifaceDock->show();
        if ( nwfilterDock->isFloating()
             && menuBar->dockMenu->nwfilterAct->isChecked() )
            nwfilterDock->show();
    };
}
void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason r)
{
    if (r==QSystemTrayIcon::Trigger) changeVisibility();
}
void MainWindow::initMenuBar()
{
    menuBar = new MenuBar(this);
    switch (viewMode) {
    case SOFT_TOUCHED:
        menuBar->viewMenu->softTouched->setChecked(true);
        break;
    case HARD_CLASSIC:
    default:
        menuBar->viewMenu->hardClassic->setChecked(true);
        break;
    };
    connect(menuBar->fileMenu->newConn, SIGNAL(triggered()),
            this, SLOT(createNewConnection()));
    connect(menuBar->fileMenu->hideToTray, SIGNAL(triggered()),
            this, SLOT(changeVisibility()));
    connect(menuBar->fileMenu->restart, SIGNAL(triggered()),
            this, SLOT(restartApplication()));
    connect(menuBar->fileMenu->exitApp, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(menuBar->viewMenu->hardClassic, SIGNAL(triggered()),
            this, SLOT(all_stuff_to_original()));
    connect(menuBar->viewMenu->softTouched, SIGNAL(triggered()),
            this, SLOT(free_and_hide_all_stuff()));
    setMenuBar(menuBar);
}
void MainWindow::initConnListWidget()
{
    connListWidget = new Connections(this);
    connListWidget->setObjectName("connListDock");
    switch (viewMode) {
    case SOFT_TOUCHED:
        settings.beginGroup("SOFT_TOUCHED_MODE");
        break;
    case HARD_CLASSIC:
    default:
        proxyLayout->addWidget(connListWidget);
        break;
    };
    int area_int = settings.value("ToolBarArea", 4).toInt();
    settings.beginGroup("ConnectListColumns");
    connListWidget->list->setColumnWidth(
                0, settings.value("column0", 132).toInt());
    connListWidget->list->setColumnWidth(
                1, settings.value("column1", 32).toInt());
    connListWidget->list->setColumnWidth(
                2, settings.value("column2", 32).toInt());
    settings.endGroup();
    switch (viewMode) {
    case SOFT_TOUCHED:
        settings.endGroup();
        break;
    case HARD_CLASSIC:
    default:
        break;
    };
    connListWidget->setToolBarArea(area_int);
    setCentralWidget(proxyWdg);
    connect(connListWidget->list, SIGNAL(removeConnection(const QString&)),
            this, SLOT(removeConnItem(const QString&)));
    connect(connListWidget->list, SIGNAL(messageShowed()),
            this, SLOT(mainWindowUp()));
    connect(connListWidget->list, SIGNAL(warning(const QString&)),
            this, SLOT(writeToErrorLog(const QString&)));
    connect(connListWidget->list, SIGNAL(connToOverview(virConnectPtr*, const QString&)),
            this, SLOT(receiveConnPtrPtr(virConnectPtr*, const QString&)));
    connect(connListWidget->list, SIGNAL(connClosed(bool, const QString&)),
            this, SLOT(stopConnProcessing(bool, const QString&)));
    connect(connListWidget->list, SIGNAL(domainEnd(const QString&)),
            this, SLOT(deleteVMDisplay(const QString&)));
    connect(connListWidget->toolBar->_createAction, SIGNAL(triggered()),
            this, SLOT(createNewConnection()));
    connect(connListWidget->toolBar->_editAction, SIGNAL(triggered()),
            this, SLOT(editCurrentConnection()));
    connect(connListWidget->toolBar->_deleteAction, SIGNAL(triggered()),
            this, SLOT(deleteCurrentConnection()));
    connect(connListWidget->toolBar->_openAction, SIGNAL(triggered()),
            this, SLOT(openCurrentConnection()));
    connect(connListWidget->toolBar->_overwiewAction, SIGNAL(triggered()),
            this, SLOT(currentConnectionToOverview()));
    connect(connListWidget->toolBar->_closeAction, SIGNAL(triggered()),
            this, SLOT(closeCurrentConnection()));
    connect(connListWidget->toolBar->_closeAllAction, SIGNAL(triggered()),
            this, SLOT(closeAllConnections()));
    connect(connListWidget->toolBar->_closeOverview, SIGNAL(triggered()),
            connListWidget->list, SLOT(stopProcessing()));
    connect(connListWidget->toolBar->_closeOverview, SIGNAL(triggered()),
            this, SLOT(stopProcessing()));
    connect(connListWidget->list, SIGNAL(searchStarted()),
            this, SLOT(startRadarAnimation()));
    connect(connListWidget->list, SIGNAL(searchFinished()),
            this, SLOT(finishRadarAnimation()));
    connect(connListWidget->list, SIGNAL(searchFinished()),
            this, SLOT(enableSoftTouchedDocks()));
}
void MainWindow::initDockWidgets()
{
    logDock = new DockWidget(this);
    logDock->setObjectName("logDock");
    logDock->setWindowTitle(tr("Log"));
    logHeadWdg = new DockHeadWidget(this, tr("Events & Errors"));
    logHeadWdg->setTabBarName("utilities-log-viewer");
    logDock->setTitleBarWidget(logHeadWdg);
    connect(logHeadWdg, SIGNAL(floatChanged(bool)),
            logDock, SLOT(_setFloating(bool)));
    connect(logDock, SIGNAL(topLevelChanged(bool)),
            logHeadWdg, SLOT(floatStateChanged(bool)));
    logDockContent = new LogDock(this);
    logDock->setWidget( logDockContent );
    connect(menuBar->dockMenu->logAct, SIGNAL(toggled(bool)),
            logDock, SLOT(setVisible(bool)));
    connect(logDockContent, SIGNAL(overflow(bool)),
            logDock, SLOT(changeWarningState(bool)));
    connect(logDockContent, SIGNAL(overflow(bool)),
            trayIcon, SLOT(changeWarningState(bool)));

    // Control #1
    domainDock = new DockWidget(this);
    domainDock->setObjectName("domainDock");
    domainDock->setWindowTitle(tr("Domain"));
    domainDock->setWindowIcon(QIcon::fromTheme("domain"));
    domHeadWdg = new DockHeadWidget(this, tr("Virtual Machines"));
    domHeadWdg->setTabBarName("domain");
    domainDock->setTitleBarWidget(domHeadWdg);
    connect(domHeadWdg, SIGNAL(floatChanged(bool)),
            domainDock, SLOT(_setFloating(bool)));
    connect(domainDock, SIGNAL(topLevelChanged(bool)),
            domHeadWdg, SLOT(floatStateChanged(bool)));
    domainDockContent = new VirtDomainControl(this);
    domainDock->setWidget( domainDockContent );
    connect(menuBar->dockMenu->domainAct, SIGNAL(toggled(bool)),
            domainDock, SLOT(setVisible(bool)));
    connect(domainDockContent, SIGNAL(entityMsg(const QString&)),
            this, SLOT(writeToErrorLog(const QString&)));
    connect(domainDockContent, SIGNAL(displayRequest(TASK*)),
            this, SLOT(invokeVMDisplay(TASK*)));
    connect(domainDockContent,
            SIGNAL(addToStateMonitor(virConnectPtr*, const QString&, const QString&)),
            domainsStateMonitor,
            SLOT(setNewMonitoredDomain(virConnectPtr*, const QString&, const QString&)));
    connect(domainDockContent, SIGNAL(migrateToConnect(TASK*)),
            this, SLOT(buildMigrateArgs(TASK*)));
    connect(domainDockContent, SIGNAL(addNewTask(TASK*)),
            taskWrHouse, SLOT(addNewTask(TASK*)));
    connect(taskWrHouse, SIGNAL(domResult(Result*)),
            domainDockContent, SLOT(resultReceiver(Result*)));
    connect(connListWidget->list, SIGNAL(domStateChanged()),
            domainDockContent, SLOT(reloadState()));
    connect(domainDockContent, SIGNAL(domainToEditor(TASK*)),
            this, SLOT(invokeDomainEditor(TASK*)));
    connect(domainDockContent, SIGNAL(entityListUpdated()),
            this, SLOT(entityControlUpdated()));

    // Control #2
    networkDock = new DockWidget(this);
    networkDock->setObjectName("networkDock");
    networkDock->setWindowTitle(tr("Network"));
    networkDock->setWindowIcon(QIcon::fromTheme("network"));
    netHeadWdg = new DockHeadWidget(this, tr("Virtual Networks"));
    netHeadWdg->setTabBarName("network");
    networkDock->setTitleBarWidget(netHeadWdg);
    connect(netHeadWdg, SIGNAL(floatChanged(bool)),
            networkDock, SLOT(_setFloating(bool)));
    connect(networkDock, SIGNAL(topLevelChanged(bool)),
            netHeadWdg, SLOT(floatStateChanged(bool)));
    networkDockContent = new VirtNetControl(this);
    networkDock->setWidget( networkDockContent );
    connect(menuBar->dockMenu->networkAct, SIGNAL(toggled(bool)),
            networkDock, SLOT(setVisible(bool)));
    connect(networkDockContent, SIGNAL(entityMsg(const QString&)),
            this, SLOT(writeToErrorLog(const QString&)));
    connect(networkDockContent, SIGNAL(addNewTask(TASK*)),
            taskWrHouse, SLOT(addNewTask(TASK*)));
    connect(taskWrHouse, SIGNAL(netResult(Result*)),
            networkDockContent, SLOT(resultReceiver(Result*)));
    connect(connListWidget->list, SIGNAL(netStateChanged()),
            networkDockContent, SLOT(reloadState()));
    connect(networkDockContent, SIGNAL(networkToEditor(TASK*)),
            this, SLOT(invokeNetworkEditor(TASK*)));
    connect(networkDockContent, SIGNAL(entityListUpdated()),
            this, SLOT(entityControlUpdated()));

    // Control #3
    storagePoolDock = new DockWidget(this);
    storagePoolDock->setObjectName("storagePoolDock");
    storagePoolDock->setWindowTitle(tr("StoragePool"));
    poolHeadWdg = new DockHeadWidget(this, tr("Storage Pools"));
    poolHeadWdg->setTabBarName("storage");
    storagePoolDock->setTitleBarWidget(poolHeadWdg);
    connect(poolHeadWdg, SIGNAL(floatChanged(bool)),
            storagePoolDock, SLOT(_setFloating(bool)));
    connect(storagePoolDock, SIGNAL(topLevelChanged(bool)),
            poolHeadWdg, SLOT(floatStateChanged(bool)));
    storagePoolDockContent = new VirtStoragePoolControl(this);
    storagePoolDock->setWidget( storagePoolDockContent );
    connect(menuBar->dockMenu->storageAct, SIGNAL(toggled(bool)),
            storagePoolDock, SLOT(setVisible(bool)));
    connect(storagePoolDockContent, SIGNAL(entityMsg(const QString&)),
            this, SLOT(writeToErrorLog(const QString&)));
    connect(storagePoolDockContent, SIGNAL(addNewTask(TASK*)),
            taskWrHouse, SLOT(addNewTask(TASK*)));
    connect(taskWrHouse, SIGNAL(poolResult(Result*)),
            storagePoolDockContent, SLOT(resultReceiver(Result*)));
    connect(connListWidget->list, SIGNAL(poolStateChanged()),
            storagePoolDockContent, SLOT(reloadState()));
    connect(storagePoolDockContent,
            SIGNAL(overviewStPool(virConnectPtr*, const QString&, const QString&)),
            this,
            SLOT(overviewStoragePool(virConnectPtr*, const QString&, const QString&)));
    connect(storagePoolDockContent, SIGNAL(entityListUpdated()),
            this, SLOT(entityControlUpdated()));

    // Control #4
    secretDock = new DockWidget(this);
    secretDock->setObjectName("secretDock");
    secretDock->setWindowTitle(tr("Secret"));
    scrtHeadWdg = new DockHeadWidget(this, tr("Secrets"));
    scrtHeadWdg->setTabBarName("security");
    secretDock->setTitleBarWidget(scrtHeadWdg);
    connect(scrtHeadWdg, SIGNAL(floatChanged(bool)),
            secretDock, SLOT(_setFloating(bool)));
    connect(secretDock, SIGNAL(topLevelChanged(bool)),
            scrtHeadWdg, SLOT(floatStateChanged(bool)));
    secretDockContent = new VirtSecretControl(this);
    secretDock->setWidget( secretDockContent );
    connect(menuBar->dockMenu->secretAct, SIGNAL(toggled(bool)),
            secretDock, SLOT(setVisible(bool)));
    connect(secretDockContent, SIGNAL(entityMsg(const QString&)),
            this, SLOT(writeToErrorLog(const QString&)));
    connect(secretDockContent, SIGNAL(addNewTask(TASK*)),
            taskWrHouse, SLOT(addNewTask(TASK*)));
    connect(taskWrHouse, SIGNAL(secResult(Result*)),
            secretDockContent, SLOT(resultReceiver(Result*)));
    connect(connListWidget->list, SIGNAL(secStateChanged()),
            secretDockContent, SLOT(reloadState()));
    connect(secretDockContent, SIGNAL(entityListUpdated()),
            this, SLOT(entityControlUpdated()));

    // Control #5
    ifaceDock = new DockWidget(this);
    ifaceDock->setObjectName("ifaceDock");
    ifaceDock->setWindowTitle(tr("Interface"));
    ifaceHeadWdg = new DockHeadWidget(this, tr("Interfaces"));
    ifaceHeadWdg->setTabBarName("network-wired");
    ifaceDock->setTitleBarWidget(ifaceHeadWdg);
    connect(ifaceHeadWdg, SIGNAL(floatChanged(bool)),
            ifaceDock, SLOT(_setFloating(bool)));
    connect(ifaceDock, SIGNAL(topLevelChanged(bool)),
            ifaceHeadWdg, SLOT(floatStateChanged(bool)));
    ifaceDockContent = new VirtInterfaceControl(this);
    ifaceDock->setWidget( ifaceDockContent );
    connect(menuBar->dockMenu->ifaceAct, SIGNAL(toggled(bool)),
            ifaceDock, SLOT(setVisible(bool)));
    connect(ifaceDockContent, SIGNAL(entityMsg(const QString&)),
            this, SLOT(writeToErrorLog(const QString&)));
    connect(ifaceDockContent, SIGNAL(addNewTask(TASK*)),
            taskWrHouse, SLOT(addNewTask(TASK*)));
    connect(taskWrHouse, SIGNAL(ifaceResult(Result*)),
            ifaceDockContent, SLOT(resultReceiver(Result*)));
    connect(ifaceDockContent, SIGNAL(entityListUpdated()),
            this, SLOT(entityControlUpdated()));

    // Control #6
    nwfilterDock = new DockWidget(this);
    nwfilterDock->setObjectName("nwfilterDock");
    nwfilterDock->setWindowTitle(tr("NWFilter"));
    nwfilterHeadWdg = new DockHeadWidget(this, tr("NWFilters"));
    nwfilterHeadWdg->setTabBarName("nwfilter");
    nwfilterDock->setTitleBarWidget(nwfilterHeadWdg);
    connect(nwfilterHeadWdg, SIGNAL(floatChanged(bool)),
            nwfilterDock, SLOT(_setFloating(bool)));
    connect(nwfilterDock, SIGNAL(topLevelChanged(bool)),
            nwfilterHeadWdg, SLOT(floatStateChanged(bool)));
    nwfilterDockContent = new VirtNWFilterControl(this);
    nwfilterDock->setWidget( nwfilterDockContent );
    connect(menuBar->dockMenu->nwfilterAct, SIGNAL(toggled(bool)),
            nwfilterDock, SLOT(setVisible(bool)));
    connect(nwfilterDockContent, SIGNAL(entityMsg(const QString&)),
            this, SLOT(writeToErrorLog(const QString&)));
    connect(nwfilterDockContent, SIGNAL(addNewTask(TASK*)),
            taskWrHouse, SLOT(addNewTask(TASK*)));
    connect(taskWrHouse, SIGNAL(nwfilterResult(Result*)),
            nwfilterDockContent, SLOT(resultReceiver(Result*)));
    connect(nwfilterDockContent, SIGNAL(nwfilterToEditor(TASK*)),
            this, SLOT(invokeNWFilterEditor(TASK*)));
    connect(nwfilterDockContent, SIGNAL(entityListUpdated()),
            this, SLOT(entityControlUpdated()));

    domainDockContent->setEnabled(false);
    networkDockContent->setEnabled(false);
    storagePoolDockContent->setEnabled(false);
    secretDockContent->setEnabled(false);
    ifaceDockContent->setEnabled(false);
    nwfilterDockContent->setEnabled(false);

    switch (viewMode) {
    case SOFT_TOUCHED:
        free_and_hide_all_stuff();
        break;
    case HARD_CLASSIC:
    default:
        all_stuff_to_original();
        break;
    };
}
void MainWindow::initVirEventloop()
{
    virtEventLoop = new VirtEventLoop(this);
    // close application after closing virtEventLoop
    connect(virtEventLoop, SIGNAL(finished()),
            this, SLOT(virtEventLoopFinished()));
    connect(virtEventLoop, SIGNAL(errorMsg(const QString&, const uint)),
            this, SLOT(writeToErrorLog(const QString&, const uint)));
    connect(virtEventLoop, SIGNAL(result(bool)),
             this, SLOT(initConnections(bool)));
    virtEventLoop->start();
}
void MainWindow::virtEventLoopFinished()
{
    if ( reloadFlag ) {
        delete wait_thread;
        wait_thread = nullptr;
        delete virtEventLoop;
        virtEventLoop = nullptr;
        initVirEventloop();
        //qDebug()<<"restart Application done";
    } else {
        close();
    };
}
void MainWindow::restartApplication()
{
    startRadarAnimation();
    //qDebug()<<"restart Application";
    QString msg(tr("Restart Application."));
    QString time = QTime::currentTime().toString();
    QString title(tr("Libvirt EventLoop"));
    QString currMsg = QString(
    tr("<b>%1 %2:</b><br><font color='green'><b>ACTION</b></font>: %3"))
            .arg(time).arg(title).arg(msg);
    logDockContent->appendMsgToLog(currMsg);
    reloadFlag = true;
    closeAllConnections();
    wait_thread = new Wait(this, connListWidget->list);
    // stop virtEventLoop after closing all connections;
    // used 'terminate', because 'virEventRunDefaultImpl'
    // not occures when libvirt unavailable.
    connect(wait_thread, SIGNAL(finished()),
            virtEventLoop, SLOT(terminate()));
    wait_thread->start();
    // used 'terminate', because 'virEventRunDefaultImpl'
    // not occures when libvirt unavailable.
    //virtEventLoop->terminate();
}
void MainWindow::initConnections(bool started)
{
    menuBar->helpMenu->setLibvirtVersion(
                virtEventLoop->libVersion);
    QString time = QTime::currentTime().toString();
    QString title(tr("App initialization"));
    QString currMsg = QString(
            tr("<b>%1 %2:</b><br><font color='blue'>\
                <b>EVENT</b></font>: Libvirt service%3%4"))
            .arg(time).arg(title)
            .arg(!virtEventLoop->isSuccess()? tr(" not "):" ")
            .arg(tr("exist in system"));
    logDockContent->appendMsgToLog(currMsg);
    currMsg = QString(
            tr("<b>%1 %2:</b><br><font color='blue'>\
                <b>EVENT</b></font>: virtEventLoop%3%4"))
            .arg(time).arg(title)
            .arg(!started? tr(" not "):" ").arg(tr("started"));
    logDockContent->appendMsgToLog(currMsg);
    if ( !started || !virtEventLoop->isSuccess() ) {
        return;
    };
    settings.beginGroup("Connects");
    QStringList groups = settings.childGroups();
    settings.endGroup();
    QList<QString>::const_iterator i;
    for (i=groups.constBegin(); i!=groups.constEnd(); ++i) {
        QString s = (*i);
        connListWidget->list->addConnItem(s);
    };
    connListWidget->list->refreshLocalhostConnection();
}
void MainWindow::startRadarAnimation()
{
    menuBar->fileMenu->setEnabled(false);
    connListWidget->list->setEnabled(false);
    connListWidget->toolBar->setEnabled(false);
    connListWidget->list->loadL->start();
}
void MainWindow::finishRadarAnimation()
{
    menuBar->fileMenu->setEnabled(true);
    connListWidget->list->setEnabled(true);
    connListWidget->toolBar->setEnabled(true);
    connListWidget->list->loadL->stop();
    if ( reloadFlag )
        initConnectionsCompleted();
}
void MainWindow::initConnectionsCompleted()
{
    QString title(tr("App initialization"));
    QString time = QTime::currentTime().toString();
    QString currMsg = QString(
            tr("<b>%1 %2:</b><br><font color='blue'>\
                <b>EVENT</b></font>: %3"))
            .arg(time).arg(title).arg(tr("Connections inited"));
    logDockContent->appendMsgToLog(currMsg);
    reloadFlag = false;
}
void MainWindow::editCurrentConnection()
{
    connListWidget->list->connItemEditAction();
}
void MainWindow::createNewConnection()
{
    QString s = QString("<noname>");
    connListWidget->list->addConnItem(s);
}
void MainWindow::deleteCurrentConnection()
{
    connListWidget->list->deleteCurrentConnection();
}
void MainWindow::removeConnItem(const QString &connection)
{
    settings.beginGroup("Connects");
    settings.remove(connection);
    settings.endGroup();
    //qDebug()<<connection<<"connection deleted";
}
void MainWindow::openCurrentConnection()
{
    QModelIndex _item = connListWidget->list->currentIndex();
    if (_item.isValid()) {
        connListWidget->list->openConnection(_item);
    };
}
void MainWindow::currentConnectionToOverview()
{
    QModelIndex _item = connListWidget->list->currentIndex();
    if (_item.isValid()) {
        connListWidget->list->overviewOfConnection(_item);
    };
}
void MainWindow::closeCurrentConnection()
{
    QModelIndex _item = connListWidget->list->currentIndex();
    if (_item.isValid()) {
        closeConnection(_item.row());
    };
}
void MainWindow::closeAllConnections()
{
    const int count = connListWidget->list->getListItemCount();
    for (int i = 0; i< count; i++) closeConnection(i);
}
void MainWindow::closeConnection(int i)
{
    //qDebug()<<i<<" item to stop";
    const QModelIndex _item = connListWidget->list->getCustomIndex(i);
    if (_item.isValid()) {
        connListWidget->list->closeConnection(_item);
    };
}
void MainWindow::closeConnGenerations(const QString &_connName)
{
    // WARNING: the policy for close a Viewers, Editors :
    // close the connection -- close all of its generation
    foreach (QString key, VM_Displayed_Map.keys()) {
        if ( key.startsWith(_connName) ) {
            VM_Displayed_Map.value(key)->close();
        };
    };
    foreach (QString key, Overviewed_StPool_Map.keys()) {
        if ( key.startsWith(_connName) ) {
            Overviewed_StPool_Map.value(key)->close();
        };
    };
    foreach (QString key, DomainEditor_Map.keys()) {
        if ( key.startsWith(_connName) ) {
            DomainEditor_Map.value(key)->close();
        };
    };
    foreach (QString key, NetworkEditor_Map.keys()) {
        if ( key.startsWith(_connName) ) {
            NetworkEditor_Map.value(key)->close();
        };
    };
    foreach (QString key, NWFilterEditor_Map.keys()) {
        if ( key.startsWith(_connName) ) {
            NWFilterEditor_Map.value(key)->close();
        };
    };
}
bool MainWindow::runningConnExist()
{
    bool result = false;
    const int count = connListWidget->list->getListItemCount();
    for (int i=0; i<count; i++) {
        const ConnItemIndex *idx =
                connListWidget->list->getConnItemDataListIndex(i);
        //qDebug()<<connListWidget->list->item(i)->text()
        //<< connListWidget->list->item(i)->data(Qt::UserRole)
        //.toMap().value("isRunning").toInt();
        if ( idx!=nullptr &&
             idx->getData().value("isRunning").toInt()==RUNNING ) {
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
void MainWindow::writeToErrorLog(const QString &msg)
{
    logDockContent->appendMsgToLog(msg);
}
void MainWindow::writeToErrorLog(const QString &msg, const uint _number)
{
    Q_UNUSED(_number);
    QString time = QTime::currentTime().toString();
    QString title(tr("Libvirt EventLoop"));
    QString currMsg = QString(
    tr("<b>%1 %2:</b><br><font color='red'><b>ERROR</b></font>: %3"))
            .arg(time).arg(title).arg(msg);
    logDockContent->appendMsgToLog(currMsg);
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
void MainWindow::receiveConnPtrPtr(virConnectPtr *_connPtrPtr, const QString &name)
{
    // begin update entities list in Controls;
    entityControlIsUpdated = 0;
    connListWidget->setEnabled(false);
    //qDebug()<<"receiveConnPtrPtr:"<<(*_connPtrPtr);
    // send connection ptr to all related virtual resources for operating
    if ( domainDockContent->setCurrentWorkConnection(_connPtrPtr) )
        domainDockContent->setListHeader(name);
    if ( networkDockContent->setCurrentWorkConnection(_connPtrPtr) )
        networkDockContent->setListHeader(name);
    if ( storagePoolDockContent->setCurrentWorkConnection(_connPtrPtr) )
        storagePoolDockContent->setListHeader(name);
    if ( secretDockContent->setCurrentWorkConnection(_connPtrPtr) )
        secretDockContent->setListHeader(name);
    if ( ifaceDockContent->setCurrentWorkConnection(_connPtrPtr) )
        ifaceDockContent->setListHeader(name);
    if ( nwfilterDockContent->setCurrentWorkConnection(_connPtrPtr) )
        nwfilterDockContent->setListHeader(name);
}
void MainWindow::entityControlUpdated()
{
    //QTextStream s(stdout);
    if        ( sender()==domainDockContent ) {
        entityControlIsUpdated |= 1;
        //s<<"domainDockContent "<<entityControlIsUpdated<<endl;
    } else if ( sender()==networkDockContent ) {
        entityControlIsUpdated |= 2;
        //s<<"networkDockContent "<<entityControlIsUpdated<<endl;
    } else if ( sender()==storagePoolDockContent ) {
        entityControlIsUpdated |= 4;
        //s<<"storagePoolDockContent "<<entityControlIsUpdated<<endl;
    } else if ( sender()==secretDockContent ) {
        entityControlIsUpdated |= 8;
        //s<<"secretDockContent "<<entityControlIsUpdated<<endl;
    } else if ( sender()==ifaceDockContent ) {
        entityControlIsUpdated |= 16;
        //s<<"ifaceDockContent "<<entityControlIsUpdated<<endl;
    } else if ( sender()==nwfilterDockContent ) {
        entityControlIsUpdated |= 32;
        //s<<"nwfilterDockContent "<<entityControlIsUpdated<<endl;
    };
    // if all Controls updated, then enable connection list widget
    // 63(10)=111111(2) -- we have six control widgets
    if ( entityControlIsUpdated==63 )
        connListWidget->setEnabled(true);
}
void MainWindow::stopConnProcessing(bool onView, const QString &_connName)
{
    // clear Overview Docks if closed connection is on View
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
    nwfilterDockContent->stopProcessing();
}
void MainWindow::invokeVMDisplay(TASK *_task)
{
    virConnectPtr *connPtrPtr = _task->srcConnPtr;
    QString connName = _task->srcConName;
    QString domName = _task->object;
    QString type = _task->args.object;
    QString viewerType = _task->args.state;
    QString addrData = _task->args.path;
    // WARNING: key must starts with connection name
    // see for: MainWindow::closeConnGenerations(QString &_connName)
    QString key = QString("%1_%2").arg(connName).arg(domName);
    if ( !VM_Displayed_Map.contains(key) ) {
        //qDebug()<<key<<"vm invoked"<<"new";
        if ( type.toLower().compare("lxc")==0 ) {
#if WITH_LXC_SUPPORT
            VM_Displayed_Map.insert(
                        key,
                        new LXC_Viewer(
                            nullptr,
                            connPtrPtr,
                            connName,
                            domName));
#else
            QMessageBox::information(
                        this,
                        tr("VM Viewer"),
                        tr("Application built without LXC"));
            return;
#endif
        } else if ( viewerType.compare("vnc")==0 ) {
#if WITH_VNC_SUPPORT
            VM_Displayed_Map.insert(
                        key,
                        new VNC_Viewer(
                            nullptr,
                            connPtrPtr,
                            connName,
                            domName,
                            addrData));
#else
            QMessageBox::information(
                        this,
                        tr("VM Viewer"),
                        tr("Application built without VNC"));
            return;
#endif
        } else if ( viewerType.compare("spice")==0 ) {
#if WITH_SPICE_SUPPORT
            VM_Displayed_Map.insert(
                        key,
                        new Spice_Viewer(
                            nullptr,
                            connPtrPtr,
                            connName,
                            domName,
                            addrData));
#else
            QMessageBox::information(
                        this,
                        tr("VM Viewer"),
                        tr("Application built without SPICE"));
            return;
#endif
        } else {
            QMessageBox::information(
                        this,
                        tr("VM Viewer"),
                        tr("Not implemented type: %1\n or viewer: %2")
                        .arg(type).arg(viewerType));
            return;
        };
        VM_Displayed_Map.value(key)->setObjectName(key);
        connect(VM_Displayed_Map.value(key), SIGNAL(finished(const QString&)),
                this, SLOT(deleteVMDisplay(const QString&)));
        connect(VM_Displayed_Map.value(key), SIGNAL(errorMsg(const QString&)),
                logDockContent, SLOT(appendMsgToLog(const QString&)));
        connect(VM_Displayed_Map.value(key), SIGNAL(addNewTask(TASK*)),
                taskWrHouse, SLOT(addNewTask(TASK*)));
        // will be showed when occured successful connection to VM
        //VM_Displayed_Map.value(key)->show();
    } else {
        //qDebug()<<key<<"vm invoked"<<"exist";
        if ( VM_Displayed_Map.value(key)!=nullptr ) {
            VM_Displayed_Map.value(key)->show();
        } else {
            VM_Viewer *vm = static_cast<VM_Viewer*>(
                        VM_Displayed_Map.take(key));
            if ( vm!=nullptr ) {
                vm->deleteLater();
            };
        };
    };
}
void MainWindow::deleteVMDisplay(const QString &key)
{
    if ( VM_Displayed_Map.contains(key) ) {
        /*
        VM_Viewer *value = nullptr;
        QString _type =
                VM_Displayed_Map.value(key, nullptr)->TYPE.toUpper();
        if ( _type.compare("LXC")!=0 ) {
            value = static_cast<VM_Viewer*>(
                        VM_Displayed_Map.value(key, nullptr));
            if ( nullptr!=value ) {
                delete value;
                value = nullptr;
            };
        };
        */
        //if ( VM_Displayed_Map.value(key, nullptr)!=nullptr ) {
        //    VM_Displayed_Map.value(key, nullptr)->deleteLater();
        //};
        //VM_Displayed_Map.remove(key);
        VM_Viewer *vm = static_cast<VM_Viewer*>(
                    VM_Displayed_Map.take(key));
        if ( vm!=nullptr ) {
            vm->deleteLater();
        };
    }
}
void MainWindow::buildMigrateArgs(TASK *_task)
{
    virConnectPtr *namedConnection =
            connListWidget->list->getPtr_connectionPtr(_task->args.path);
    if ( nullptr!=namedConnection ) {
        domainDockContent->execMigrateAction(namedConnection, _task);
    } else {
        QString time = QTime::currentTime().toString();
        QString title =
                tr("Domain Migration '%1'")
                .arg(_task->object);
        QString msg =
                tr("Migation not possible to NULL connection.");
        QString currMsg = QString(
                tr("<b>%1 %2:</b><br><font color='red'><b>ERROR</b></font>: %3"))
                .arg(time).arg(title).arg(msg);
        logDockContent->appendMsgToLog(currMsg);
    }
}

void MainWindow::overviewStoragePool(
        virConnectPtr *connPtrPtr, const QString &connName, const QString &poolName)
{
    // WARNING: key must starts with connection name
    // see for: MainWindow::closeConnGenerations(QString &_connName)
    QString key = QString("%1_%2").arg(connName).arg(poolName);
    if ( !Overviewed_StPool_Map.contains(key) ) {
        Overviewed_StPool_Map.insert(key, new VirtStorageVolControl());
        Overviewed_StPool_Map.value(key)->setObjectName(key);
        Overviewed_StPool_Map.value(key)
                ->setWindowTitle(tr("%1 Pool").arg(key));
        connect(Overviewed_StPool_Map.value(key), SIGNAL(entityMsg(const QString&)),
                this, SLOT(writeToErrorLog(const QString&)));
        connect(Overviewed_StPool_Map.value(key), SIGNAL(finished(const QString&)),
                this, SLOT(deleteStPoolOverview(const QString&)));
        connect(Overviewed_StPool_Map.value(key), SIGNAL(addNewTask(TASK*)),
                taskWrHouse, SLOT(addNewTask(TASK*)));
        connect(taskWrHouse, SIGNAL(volResult(Result*)),
                Overviewed_StPool_Map.value(key),
                SLOT(resultReceiver(Result*)));
        Overviewed_StPool_Map.value(key)
                ->setCurrentStoragePool(
                    connPtrPtr, connName, poolName);
    };
    if ( Overviewed_StPool_Map.value(key)!=nullptr ) {
        Overviewed_StPool_Map.value(key)->show();
        Overviewed_StPool_Map.value(key)->setFocus();
    } else
        Overviewed_StPool_Map.value(key);
}
void MainWindow::deleteStPoolOverview(const QString &key)
{
    if ( Overviewed_StPool_Map.contains(key) ) {
        // set attribute(Qt::WA_DeleteOnClose) in widget;
        // not need to delete a VirtStorageVolControl
        Overviewed_StPool_Map.remove(key);
    };
}
void MainWindow::invokeDomainEditor(TASK *_task)
{
    QString connName = _task->srcConName;
    QString domName = _task->object;
    // WARNING: key must starts with connection name
    // see for: MainWindow::closeConnGenerations(QString &_connName)
    QString key = QString("%1_%2").arg(connName).arg(domName);
    if ( !DomainEditor_Map.contains(key) ) {
        DomainEditor_Map.insert(
                    key,
                    new CreateVirtDomain(nullptr, *_task));
        DomainEditor_Map.value(key)->setObjectName(key);
        DomainEditor_Map.value(key)->setWindowTitle(
                    tr("VM Settings / <%1> in [%2]")
                    .arg(domName).arg(connName));
        connect(DomainEditor_Map.value(key), SIGNAL(errorMsg(const QString&)),
                this, SLOT(writeToErrorLog(const QString&)));
        connect(DomainEditor_Map.value(key), SIGNAL(finished(const QString&)),
                this, SLOT(deleteDomainEditor(const QString&)));
        connect(DomainEditor_Map.value(key), SIGNAL(addNewTask(TASK*)),
                taskWrHouse, SLOT(addNewTask(TASK*)));
    };
    if ( DomainEditor_Map.value(key)!=nullptr ) {
        DomainEditor_Map.value(key)->show();
        DomainEditor_Map.value(key)->setFocus();
    } else
        DomainEditor_Map.remove(key);
}
void MainWindow::deleteDomainEditor(const QString &key)
{
    if ( DomainEditor_Map.contains(key) ) {
        // set attribute(Qt::WA_DeleteOnClose) in widget;
        // not need to delete a CreateVirtDomain
        DomainEditor_Map.remove(key);
    };
}
void MainWindow::migrate_settings_to_INI_format()
{
    //qDebug()<<"migrate_settings_to_INI_format";
    QSettings oldSettings(
                QSettings::NativeFormat,
                QSettings::UserScope,
                qApp->organizationName(),
                qApp->applicationName());
    if ( settings.allKeys().isEmpty() && !oldSettings.allKeys().isEmpty() ) {
        foreach (QString group, oldSettings.childGroups()) {
            oldSettings.beginGroup(group);
            settings.beginGroup(group);
            foreach (QString key, oldSettings.allKeys()) {
                settings.setValue(
                            key,
                            oldSettings.value(key));
            };
            settings.endGroup();
            oldSettings.endGroup();
        };
        oldSettings.clear();
        oldSettings.setValue(
                    "Cleaned",
                    "after migration to INI-format.");
        QMessageBox::information(
                    this,
                    qApp->applicationName(),
                    "Migration the settings to INI-format done successfully.");
    };
}
void MainWindow::invokeNetworkEditor(TASK *_task)
{
    QString connName = _task->srcConName;
    QString networkName = _task->object;
    // WARNING: key must starts with connection name
    // see for: MainWindow::closeConnGenerations(QString &_connName)
    QString key = QString("%1_%2").arg(connName).arg(networkName);
    if ( !NetworkEditor_Map.contains(key) ) {
        NetworkEditor_Map.insert(
                    key,
                    new CreateVirtNetwork(nullptr, *_task));
        NetworkEditor_Map.value(key)->setObjectName(key);
        NetworkEditor_Map.value(key)->setWindowTitle(
                    tr("Network Editor / <%1> in [%2]")
                    .arg(networkName).arg(connName));
        connect(NetworkEditor_Map.value(key), SIGNAL(errorMsg(const QString&)),
                this, SLOT(writeToErrorLog(const QString&)));
        connect(NetworkEditor_Map.value(key), SIGNAL(finished(const QString&)),
                this, SLOT(deleteNetworkEditor(const QString&)));
        connect(NetworkEditor_Map.value(key), SIGNAL(addNewTask(TASK*)),
                taskWrHouse, SLOT(addNewTask(TASK*)));
    };
    if ( NetworkEditor_Map.value(key)!=nullptr ) {
        NetworkEditor_Map.value(key)->show();
        NetworkEditor_Map.value(key)->setFocus();
    } else
        NetworkEditor_Map.remove(key);
}
void MainWindow::deleteNetworkEditor(const QString &key)
{
    if ( NetworkEditor_Map.contains(key) ) {
        // set attribute(Qt::WA_DeleteOnClose) in widget;
        // not need to delete a CreateVirtNetwork
        NetworkEditor_Map.remove(key);
    };
}
void MainWindow::invokeNWFilterEditor(TASK *_task)
{
    QString connName = _task->srcConName;
    QString nwfilterName = _task->object;
    // WARNING: key must starts with connection name
    // see for: MainWindow::closeConnGenerations(QString &_connName)
    QString key = QString("%1_%2").arg(connName).arg(nwfilterName);
    if ( !NWFilterEditor_Map.contains(key) ) {
        NWFilterEditor_Map.insert(
                    key,
                    new CreateVirtNWFilter(nullptr, *_task));
        NWFilterEditor_Map.value(key)->setObjectName(key);
        NWFilterEditor_Map.value(key)->setWindowTitle(
                    tr("NWFilter Editor / <%1> in [%2]")
                    .arg(nwfilterName).arg(connName));
        connect(NWFilterEditor_Map.value(key), SIGNAL(errorMsg(const QString&)),
                this, SLOT(writeToErrorLog(const QString&)));
        connect(NWFilterEditor_Map.value(key), SIGNAL(finished(const QString&)),
                this, SLOT(deleteNWFilterEditor(const QString&)));
        connect(NWFilterEditor_Map.value(key), SIGNAL(addNewTask(TASK*)),
                taskWrHouse, SLOT(addNewTask(TASK*)));
    };
    if ( NWFilterEditor_Map.value(key)!=nullptr ) {
        NWFilterEditor_Map.value(key)->show();
        NWFilterEditor_Map.value(key)->setFocus();
    } else
        NWFilterEditor_Map.remove(key);
}
void MainWindow::deleteNWFilterEditor(const QString &key)
{
    if ( NWFilterEditor_Map.contains(key) ) {
        // set attribute(Qt::WA_DeleteOnClose) in widget;
        // not need to delete a CreateVirtNWFilter
        NWFilterEditor_Map.remove(key);
    };
}

void MainWindow::free_and_hide_all_stuff()
{
    menuBar->dockMenu->setDisabled(true);
    proxyLayout->removeWidget(connListWidget);
    connListWidget->hide();
    removeDockWidget(logDock);
    removeDockWidget(domainDock);
    removeDockWidget(networkDock);
    removeDockWidget(storagePoolDock);
    removeDockWidget(secretDock);
    removeDockWidget(ifaceDock);
    removeDockWidget(nwfilterDock);
    setDockFloatible(false);
    settings.beginGroup("SOFT_TOUCHED_MODE");
    SoftTouchedWdg = new ST_StackedWidget(this);
    restoreGeometry(settings.value("Geometry").toByteArray());
    SoftTouchedWdg->addNewWidget(connListWidget);
    SoftTouchedWdg->addNewWidget(logDock);
    SoftTouchedWdg->addNewWidget(domainDock);
    SoftTouchedWdg->addNewWidget(networkDock);
    SoftTouchedWdg->addNewWidget(storagePoolDock);
    SoftTouchedWdg->addNewWidget(secretDock);
    SoftTouchedWdg->addNewWidget(ifaceDock);
    SoftTouchedWdg->addNewWidget(nwfilterDock);
    SoftTouchedWdg->hide();
    connect(proxyWdg, SIGNAL(viewDock(const QString&)),
            SoftTouchedWdg, SLOT(showDock(const QString&)));
    connect(proxyWdg, SIGNAL(viewNextDock()),
            SoftTouchedWdg, SLOT(showNextDock()));
    connect(proxyWdg, SIGNAL(viewPrevDock()),
            SoftTouchedWdg, SLOT(showPrevDock()));
    restoreState(settings.value("State").toByteArray());
    settings.endGroup();
    setDockHeaderWheelEventsEnabled(true);
    setDockUsedInSoftTouched(true);
    viewMode = SOFT_TOUCHED;
    proxyWdg->setUsedViewMode(viewMode);
    proxyWdg->update();
    proxyLayout->addWidget(SoftTouchedWdg);
}
void MainWindow::all_stuff_to_original()
{
    setDockHeaderWheelEventsEnabled(false);
    setDockUsedInSoftTouched(false);
    if ( SoftTouchedWdg!=nullptr ) {
        disconnect(proxyWdg, SIGNAL(viewDock(const QString&)),
                   SoftTouchedWdg, SLOT(showDock(const QString&)));
        disconnect(proxyWdg, SIGNAL(viewNextDock()),
                   SoftTouchedWdg, SLOT(showNextDock()));
        disconnect(proxyWdg, SIGNAL(viewPrevDock()),
                   SoftTouchedWdg, SLOT(showPrevDock()));
        proxyLayout->removeWidget(SoftTouchedWdg);
        SoftTouchedWdg->removeAllWidgets();
        //delete SoftTouchedWdg;
        //SoftTouchedWdg = nullptr;
        SoftTouchedWdg->deleteLater();
    };
    /*
     * Note: Parent object and parent widget of widget
     * will remain the QStackedWidget. If the application
     * wants to reuse the removed widget, then
     * it is recommended to re-parent it.
     */
    Qt::DockWidgetArea area;
    bool visible;
    restoreGeometry(settings.value("Geometry").toByteArray());
    connListWidget->setParent(this);
    proxyLayout->addWidget(connListWidget);
    connListWidget->show();
    logDock->setParent(this);
    settings.beginGroup("LogDock");
    logDock->setFloating(settings.value("Floating", false).toBool());
    logDock->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", true).toBool();
    logDock->setVisible(visible);
    area = getDockArea(
                settings.value(
                    "DockArea",
                    Qt::BottomDockWidgetArea)
                .toInt());
    settings.endGroup();
    addDockWidget(area, logDock);
    menuBar->dockMenu->logAct->setChecked(visible);

    domainDock->setParent(this);
    settings.beginGroup("DomainDock");
    domainDock->setFloating(settings.value("Floating", false).toBool());
    domainDock->restoreGeometry(
                settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", true).toBool();
    domainDock->setVisible(visible);
    area = getDockArea(
                settings.value(
                    "DockArea",
                    Qt::BottomDockWidgetArea)
                .toInt());
    settings.endGroup();
    addDockWidget(area, domainDock);
    tabifyDockWidget(logDock, domainDock);
    menuBar->dockMenu->domainAct->setChecked(visible);

    networkDock->setParent(this);
    settings.beginGroup("NetworkDock");
    networkDock->setFloating(settings.value("Floating", false).toBool());
    networkDock->restoreGeometry(
                settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    networkDock->setVisible(visible);
    area = getDockArea(
                settings.value(
                    "DockArea",
                    Qt::BottomDockWidgetArea)
                .toInt());
    settings.endGroup();
    addDockWidget(area, networkDock);
    tabifyDockWidget(domainDock, networkDock);
    menuBar->dockMenu->networkAct->setChecked(visible);

    storagePoolDock->setParent(this);
    settings.beginGroup("StoragePoolDock");
    storagePoolDock->setFloating(
                settings.value("Floating", false).toBool());
    storagePoolDock->restoreGeometry(
                settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    storagePoolDock->setVisible(visible);
    area = getDockArea(
                settings.value(
                    "DockArea",
                    Qt::BottomDockWidgetArea)
                .toInt());
    settings.endGroup();
    addDockWidget(area, storagePoolDock);
    tabifyDockWidget(networkDock, storagePoolDock);
    menuBar->dockMenu->storageAct->setChecked(visible);

    secretDock->setParent(this);
    settings.beginGroup("SecretDock");
    secretDock->setFloating(
                settings.value("Floating", false).toBool());
    secretDock->restoreGeometry(
                settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    secretDock->setVisible(visible);
    area = getDockArea(
                settings.value(
                    "DockArea",
                    Qt::BottomDockWidgetArea)
                .toInt());
    settings.endGroup();
    addDockWidget(area, secretDock);
    tabifyDockWidget(storagePoolDock, secretDock);
    menuBar->dockMenu->secretAct->setChecked(visible);

    ifaceDock->setParent(this);
    settings.beginGroup("IfaceDock");
    ifaceDock->setFloating(settings.value("Floating", false).toBool());
    ifaceDock->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    ifaceDock->setVisible(visible);
    area = getDockArea(
                settings.value(
                    "DockArea",
                    Qt::BottomDockWidgetArea)
                .toInt());
    settings.endGroup();
    addDockWidget(area, ifaceDock);
    tabifyDockWidget(secretDock, ifaceDock);
    menuBar->dockMenu->ifaceAct->setChecked(visible);

    nwfilterDock->setParent(this);
    settings.beginGroup("NWFilterDock");
    nwfilterDock->setFloating(settings.value("Floating", false).toBool());
    nwfilterDock->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    nwfilterDock->setVisible(visible);
    area = getDockArea(
                settings.value(
                    "DockArea",
                    Qt::BottomDockWidgetArea)
                .toInt());
    settings.endGroup();
    addDockWidget(area, nwfilterDock);
    tabifyDockWidget(ifaceDock, nwfilterDock);
    menuBar->dockMenu->nwfilterAct->setChecked(visible);

    viewMode = HARD_CLASSIC;
    proxyWdg->setUsedViewMode(viewMode);
    proxyWdg->update();
    menuBar->dockMenu->setEnabled(true);
    restoreState(settings.value("State").toByteArray());
    setDockFloatible(true);
}
void MainWindow::setDockFloatible(bool state)
{
    if ( state ) {
        logDock->setFeatures(
            QDockWidget::DockWidgetMovable   |
            QDockWidget::DockWidgetFloatable |
            QDockWidget::DockWidgetVerticalTitleBar
        );
        domainDock->setFeatures(
            QDockWidget::DockWidgetMovable   |
            QDockWidget::DockWidgetFloatable |
            QDockWidget::DockWidgetVerticalTitleBar
        );
        networkDock->setFeatures(
            QDockWidget::DockWidgetMovable   |
            QDockWidget::DockWidgetFloatable |
            QDockWidget::DockWidgetVerticalTitleBar
        );
        storagePoolDock->setFeatures(
            QDockWidget::DockWidgetMovable   |
            QDockWidget::DockWidgetFloatable |
            QDockWidget::DockWidgetVerticalTitleBar
        );
        secretDock->setFeatures(
            QDockWidget::DockWidgetMovable   |
            QDockWidget::DockWidgetFloatable |
            QDockWidget::DockWidgetVerticalTitleBar
        );
        ifaceDock->setFeatures(
            QDockWidget::DockWidgetMovable   |
            QDockWidget::DockWidgetFloatable |
            QDockWidget::DockWidgetVerticalTitleBar
        );
        nwfilterDock->setFeatures(
            QDockWidget::DockWidgetMovable   |
            QDockWidget::DockWidgetFloatable |
            QDockWidget::DockWidgetVerticalTitleBar
        );
    } else {
        logDock->setFeatures(
            QDockWidget::NoDockWidgetFeatures |
                    QDockWidget::DockWidgetVerticalTitleBar
        );
        domainDock->setFeatures(
                    QDockWidget::NoDockWidgetFeatures |
                    QDockWidget::DockWidgetVerticalTitleBar
        );
        networkDock->setFeatures(
                    QDockWidget::NoDockWidgetFeatures |
                    QDockWidget::DockWidgetVerticalTitleBar
        );
        storagePoolDock->setFeatures(
                    QDockWidget::NoDockWidgetFeatures |
                    QDockWidget::DockWidgetVerticalTitleBar
        );
        secretDock->setFeatures(
                    QDockWidget::NoDockWidgetFeatures |
                    QDockWidget::DockWidgetVerticalTitleBar
        );
        ifaceDock->setFeatures(
                    QDockWidget::NoDockWidgetFeatures |
                    QDockWidget::DockWidgetVerticalTitleBar
        );
        nwfilterDock->setFeatures(
                    QDockWidget::NoDockWidgetFeatures |
                    QDockWidget::DockWidgetVerticalTitleBar
        );
    };
    logHeadWdg->setFloatible(state);
    domHeadWdg->setFloatible(state);
    netHeadWdg->setFloatible(state);
    poolHeadWdg->setFloatible(state);
    scrtHeadWdg->setFloatible(state);
    ifaceHeadWdg->setFloatible(state);
    nwfilterHeadWdg->setFloatible(state);
}
void MainWindow::setDockHeaderWheelEventsEnabled(bool state)
{
    if ( SoftTouchedWdg!=nullptr ) {
        if ( state ) {
            connect(logHeadWdg, SIGNAL(viewNextDock()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(logHeadWdg, SIGNAL(viewPrevDock()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(domHeadWdg, SIGNAL(viewNextDock()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(domHeadWdg, SIGNAL(viewPrevDock()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(netHeadWdg, SIGNAL(viewNextDock()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(netHeadWdg, SIGNAL(viewPrevDock()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(poolHeadWdg, SIGNAL(viewNextDock()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(poolHeadWdg, SIGNAL(viewPrevDock()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(scrtHeadWdg, SIGNAL(viewNextDock()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(scrtHeadWdg, SIGNAL(viewPrevDock()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(ifaceHeadWdg, SIGNAL(viewNextDock()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(ifaceHeadWdg, SIGNAL(viewPrevDock()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(nwfilterHeadWdg, SIGNAL(viewNextDock()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(nwfilterHeadWdg, SIGNAL(viewPrevDock()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(connListWidget->toolBar, SIGNAL(viewNextDock()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(connListWidget->toolBar, SIGNAL(viewPrevDock()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
        } else {
            disconnect(logHeadWdg, SIGNAL(viewNextDock()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(logHeadWdg, SIGNAL(viewPrevDock()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(domHeadWdg, SIGNAL(viewNextDock()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(domHeadWdg, SIGNAL(viewPrevDock()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(netHeadWdg, SIGNAL(viewNextDock()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(netHeadWdg, SIGNAL(viewPrevDock()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(poolHeadWdg, SIGNAL(viewNextDock()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(poolHeadWdg, SIGNAL(viewPrevDock()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(scrtHeadWdg, SIGNAL(viewNextDock()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(scrtHeadWdg, SIGNAL(viewPrevDock()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(ifaceHeadWdg, SIGNAL(viewNextDock()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(ifaceHeadWdg, SIGNAL(viewPrevDock()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(nwfilterHeadWdg, SIGNAL(viewNextDock()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(nwfilterHeadWdg, SIGNAL(viewPrevDock()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(connListWidget->toolBar, SIGNAL(viewNextDock()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(connListWidget->toolBar, SIGNAL(viewPrevDock()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
        };
    };
}
void MainWindow::setDockUsedInSoftTouched(bool state)
{
    if ( SoftTouchedWdg!=nullptr ) {
        if ( state ) {
            connect(logDockContent->Log->nextL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(logDockContent->Log->prevL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(domainDockContent->entityList->nextL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(domainDockContent->entityList->prevL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(networkDockContent->entityList->nextL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(networkDockContent->entityList->prevL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(storagePoolDockContent->entityList->nextL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(storagePoolDockContent->entityList->prevL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(secretDockContent->entityList->nextL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(secretDockContent->entityList->prevL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(ifaceDockContent->entityList->nextL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(ifaceDockContent->entityList->prevL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(nwfilterDockContent->entityList->nextL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(nwfilterDockContent->entityList->prevL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(connListWidget->list->nextL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showNextDock()));
            connect(connListWidget->list->prevL,
                    SIGNAL(released()),
                    SoftTouchedWdg, SLOT(showPrevDock()));
            connect(connListWidget->list->homeL,
                    SIGNAL(released()),
                    this, SLOT(turnSoftTouchedToUntriggered()));
            connect(logDockContent->Log->homeL,
                    SIGNAL(released()),
                    this, SLOT(turnSoftTouchedToUntriggered()));
            connect(domainDockContent->entityList->homeL,
                    SIGNAL(released()),
                    this, SLOT(turnSoftTouchedToUntriggered()));
            connect(networkDockContent->entityList->homeL,
                    SIGNAL(released()),
                    this, SLOT(turnSoftTouchedToUntriggered()));
            connect(storagePoolDockContent->entityList->homeL,
                    SIGNAL(released()),
                    this, SLOT(turnSoftTouchedToUntriggered()));
            connect(secretDockContent->entityList->homeL,
                    SIGNAL(released()),
                    this, SLOT(turnSoftTouchedToUntriggered()));
            connect(ifaceDockContent->entityList->homeL,
                    SIGNAL(released()),
                    this, SLOT(turnSoftTouchedToUntriggered()));
            connect(nwfilterDockContent->entityList->homeL,
                    SIGNAL(released()),
                    this, SLOT(turnSoftTouchedToUntriggered()));
        } else {
            disconnect(logDockContent->Log->nextL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(logDockContent->Log->prevL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(domainDockContent->entityList->nextL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(domainDockContent->entityList->prevL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(networkDockContent->entityList->nextL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(networkDockContent->entityList->prevL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(storagePoolDockContent->entityList->nextL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(storagePoolDockContent->entityList->prevL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(secretDockContent->entityList->nextL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(secretDockContent->entityList->prevL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(ifaceDockContent->entityList->nextL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(ifaceDockContent->entityList->prevL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(nwfilterDockContent->entityList->nextL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(nwfilterDockContent->entityList->prevL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(connListWidget->list->nextL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showNextDock()));
            disconnect(connListWidget->list->homeL,
                       SIGNAL(released()),
                       this, SLOT(turnSoftTouchedToUntriggered()));
            disconnect(connListWidget->list->prevL,
                       SIGNAL(released()),
                       SoftTouchedWdg, SLOT(showPrevDock()));
            disconnect(logDockContent->Log->homeL,
                       SIGNAL(released()),
                       this, SLOT(turnSoftTouchedToUntriggered()));
            disconnect(domainDockContent->entityList->homeL,
                       SIGNAL(released()),
                       this, SLOT(turnSoftTouchedToUntriggered()));
            disconnect(networkDockContent->entityList->homeL,
                       SIGNAL(released()),
                       this, SLOT(turnSoftTouchedToUntriggered()));
            disconnect(storagePoolDockContent->entityList->homeL,
                       SIGNAL(released()),
                       this, SLOT(turnSoftTouchedToUntriggered()));
            disconnect(secretDockContent->entityList->homeL,
                       SIGNAL(released()),
                       this, SLOT(turnSoftTouchedToUntriggered()));
            disconnect(ifaceDockContent->entityList->homeL,
                       SIGNAL(released()),
                       this, SLOT(turnSoftTouchedToUntriggered()));
            disconnect(nwfilterDockContent->entityList->homeL,
                       SIGNAL(released()),
                       this, SLOT(turnSoftTouchedToUntriggered()));
        };
    };
    connListWidget->toolBar->wheelEventEnabled(state);
    connListWidget->list->setUsageInSoftTouched(state);
    domainDockContent->setUsageInSoftTouched(state);
    networkDockContent->setUsageInSoftTouched(state);
    storagePoolDockContent->setUsageInSoftTouched(state);
    secretDockContent->setUsageInSoftTouched(state);
    ifaceDockContent->setUsageInSoftTouched(state);
    nwfilterDockContent->setUsageInSoftTouched(state);
    logDockContent->setUsageInSoftTouched(state);
}
void MainWindow::turnSoftTouchedToUntriggered()
{
    SoftTouchedWdg->hide();
    proxyWdg->returnToUntriggered();
}
void MainWindow::enableSoftTouchedDocks()
{
    if ( viewMode==SOFT_TOUCHED ) {
        setDockFloatible(false);
    }
}
