#include "mainwindow.h"

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
    closeProgress = new QProgressBar(this);
    closeProgress->setRange(0, TIMEOUT);
    statusBar()->addPermanentWidget(closeProgress);
    statusBar()->hide();
}

void MainWindow::saveSettings()
{
    taskWrHouse->saveCurrentState();
    domainsStateMonitor->saveCurrentState();
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("State", saveState());
    settings.setValue("ToolBarArea", toolBarArea(toolBar));
    settings.setValue("Visible", this->isVisible());
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
  if ( runningConnExist() && wait_thread==NULL ) {
      connListWidget->setEnabled(false);
      toolBar->setEnabled(false);
      logDock->setEnabled(false);
      domainDock->setEnabled(false);
      networkDock->setEnabled(false);
      storagePoolDock->setEnabled(false);
      domainsStateMonitor->stopMonitoring();
      taskWrHouse->stopTaskComputing();
      // close VM Displays
      QStringList keys(VM_Displayed_Map.keys());
      foreach ( QString key, keys ) {
          if ( VM_Displayed_Map.value(key, NULL)!=NULL ) {
              VM_Viewer *value = NULL;
              QString _type = VM_Displayed_Map.value(key, NULL)->TYPE.toUpper();
              if ( _type=="LXC" ) {
                  value = static_cast<LXC_Viewer*>(
                              VM_Displayed_Map.value(key, NULL));
              } else if ( _type=="SPICE" ) {
                  value = static_cast<Spice_Viewer*>(
                              VM_Displayed_Map.value(key, NULL));
              };
              if ( NULL!=value ) {
                  // close&delete into deleteVMDisplay
                  if ( value->isActive() ) value->close();
              };
              VM_Displayed_Map.remove(key);
              //qDebug()<<key<<"removed into Close";
          };
      };
      VM_Displayed_Map.clear();
      //qDebug()<<"Viewers cleared";
      wait_thread = new Wait(this, connListWidget);
      connect(wait_thread, SIGNAL(finished()),
              this, SLOT(close()));
      wait_thread->start();
      ev->ignore();
      startCloseProcess();
  } else if ( !runningConnExist() &&
              (wait_thread==NULL || !wait_thread->isRunning()) ) {
      saveSettings();
      trayIcon->hide();
      ev->accept();
  } else {
      //  ( wait_thread!=NULL || wait_thread->isRunning() )
      ev->ignore();
  };
}
void MainWindow::startCloseProcess()
{
    //qDebug()<<"startCloseProcess";
    killTimerId = startTimer(PERIOD);
    statusBar()->show();
    //qDebug()<<killTimerId<<"killTimer";
}
void MainWindow::timerEvent(QTimerEvent *ev)
{
    if ( ev->timerId()==killTimerId ) {
        counter++;
        closeProgress->setValue(counter*PERIOD);
        if ( TIMEOUT<counter*PERIOD ) {
            killTimer(killTimerId);
            killTimerId = 0;
            counter = 0;
        };
    };
}
void MainWindow::initTaskWareHouse()
{
    taskWrHouse = new TaskWareHouse(this);
    connect(taskWrHouse, SIGNAL(taskMsg(QString&)),
            this, SLOT(writeToErrorLog(QString&)));
}
void MainWindow::initDomainStateMonitor()
{
    domainsStateMonitor = new DomainStateMonitor(this);
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
    };
}
void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason r)
{
    if (r==QSystemTrayIcon::Trigger) changeVisibility();
}
void MainWindow::initConnListWidget()
{
  connListWidget = new ConnectionList(this);
  settings.beginGroup("ConnectListColumns");
  connListWidget->setColumnWidth(0, settings.value("column0", 132).toInt());
  connListWidget->setColumnWidth(1, settings.value("column1", 32).toInt());
  connListWidget->setColumnWidth(2, settings.value("column2", 32).toInt());
  settings.endGroup();
  settings.beginGroup("Connects");
  QStringList groups = settings.childGroups();
  settings.endGroup();
  QList<QString>::const_iterator i;
  for (i=groups.constBegin(); i!=groups.constEnd(); ++i) {
      QString s = (*i);
      connListWidget->addConnItem(s);
  };
  connListWidget->searchLocalhostConnections();
  setCentralWidget(connListWidget);
  connect(connListWidget, SIGNAL(removeConnection(QString&)),
          this, SLOT(removeConnItem(QString&)));
  connect(connListWidget, SIGNAL(messageShowed()),
          this, SLOT(mainWindowUp()));
  connect(connListWidget, SIGNAL(warning(QString&)),
          this, SLOT(writeToErrorLog(QString&)));
  connect(connListWidget, SIGNAL(connPtr(virConnect*, QString&)),
          this, SLOT(receiveConnPtr(virConnect*, QString&)));
  connect(connListWidget, SIGNAL(connClosed(virConnect*)),
          this, SLOT(stopConnProcessing(virConnect*)));
  connect(connListWidget, SIGNAL(connToClose(int)),
          this, SLOT(closeConnStorageOverview(int)));
}
void MainWindow::initToolBar()
{
  toolBar = new ToolBar(this);
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
    logHeadWdg->setTabBarName("document-properties");
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
    connect(domainDockContent, SIGNAL(displayRequest(virConnect*,QString,QString)),
            this, SLOT(invokeVMDisplay(virConnect*,QString,QString)));
    connect(domainDockContent, SIGNAL(addToStateMonitor(virConnectPtr,QString&,QString&)),
            domainsStateMonitor, SLOT(setNewMonitoredDomain(virConnectPtr,QString&,QString&)));
    connect(domainDockContent, SIGNAL(domainClosed(QString,QString)),
            this, SLOT(deleteVMDisplay(QString,QString)));
    connect(domainDockContent, SIGNAL(migrateToConnect(TASK)),
            this, SLOT(buildMigrateArgs(TASK)));
    connect(domainDockContent, SIGNAL(addNewTask(TASK)),
            taskWrHouse, SLOT(addNewTask(TASK)));
    connect(taskWrHouse, SIGNAL(domResult(Result)),
            domainDockContent, SLOT(resultReceiver(Result)));
    connect(connListWidget, SIGNAL(domResult(Result)),
            domainDockContent, SLOT(resultReceiver(Result)));

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
    connect(storagePoolDockContent, SIGNAL(currPool(virConnect*,QString&,QString&)),
            this, SLOT(addStorageVol(virConnect*,QString&,QString&)));

    secretDock = new DockWidget(this);
    secretDock->setObjectName("secretDock");
    secretDock->setWindowTitle("Secret");
    secretDock->setFeatures(
        QDockWidget::DockWidgetMovable   |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar
    );
    scrtHeadWdg = new DockHeadWidget(this, "Secret");
    scrtHeadWdg->setTabBarName("security-high");
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

    domainDockContent->setEnabled(false);
    networkDockContent->setEnabled(false);
    storagePoolDockContent->setEnabled(false);
    secretDockContent->setEnabled(false);
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
void MainWindow::closeConnStorageOverview(int i)
{
    ConnItemIndex *idx = static_cast<ConnItemIndex*>(
                connListWidget->connItemModel->
                connItemDataList.at(i));
    if ( idx!=NULL ) {
        QString conn_to_close = idx->getName();
        foreach (QString key, storageMap.keys()) {
            if ( key.startsWith(conn_to_close) ) {
                storageMap.value(key)->close();
                storageMap.remove(key);
            };
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
    logDockContent->appendErrorMsg(msg);
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
void MainWindow::receiveConnPtr(virConnect *conn, QString &name)
{
    // send connect ptr to all related virtual resources for operating
    if ( domainDockContent->setCurrentWorkConnect(conn) )
        domainDockContent->setListHeader(name);
    if ( networkDockContent->setCurrentWorkConnect(conn) )
        networkDockContent->setListHeader(name);
    if ( storagePoolDockContent->setCurrentWorkConnect(conn) )
        storagePoolDockContent->setListHeader(name);
    if ( secretDockContent->setCurrentWorkConnect(conn) )
        secretDockContent->setListHeader(name);
}
void MainWindow::stopConnProcessing(virConnect *conn)
{
    // clear Overview Docks if closed connect is overviewed
    if ( NULL!=conn && conn==domainDockContent->getConnection() ) {
        stopProcessing();
    };
}
void MainWindow::stopProcessing()
{
    // stop processing of all virtual resources
    domainDockContent->stopProcessing();
    networkDockContent->stopProcessing();
    storagePoolDockContent->stopProcessing();
    secretDockContent->stopProcessing();
    //domainDockContent->getThreadState();
    //networkDockContent->getThreadState() ;
    //storagePoolDockContent->getThreadState();
}
void MainWindow::invokeVMDisplay(virConnect *conn, QString connName, QString domName)
{
    QString type;
    VM_Viewer *value = NULL;
    if ( conn!=NULL ) {
        type = QString::fromUtf8(virConnectGetType(conn));
    } else type.clear();
    QString key = QString("%1_%2").arg(connName).arg(domName);
    if ( !VM_Displayed_Map.contains(key) ) {
        //qDebug()<<key<<"vm invoked"<<"new";
        if ( type.isEmpty() ) {
            QMessageBox::information(this, "VM Viewer", "Job empty.");
        } else if ( type.toLower()=="lxc" ) {
            value = new LXC_Viewer(this, conn, connName, domName);
        } else if ( type.toLower()=="qemu" || type.toLower()=="xen" ) {
            value = new Spice_Viewer(this, conn, connName, domName);
        } else
            QMessageBox::information(
                        this,
                        "VM Viewer",
                        QString("Not implemented type: %1").arg(type));
        if ( value==NULL ) return;
        VM_Displayed_Map.insert(key, value);
        connect(value, SIGNAL(finished(QString&)),
                this, SLOT(deleteVMDisplay(QString&)));
        connect(value, SIGNAL(errorMsg(QString&)),
                logDockContent, SLOT(appendErrorMsg(QString&)));
        connect(value, SIGNAL(addNewTask(TASK)),
                taskWrHouse, SLOT(addNewTask(TASK)));
        value->show();
    } else {
        //qDebug()<<key<<"vm invoked"<<"exist";
        if ( type.isEmpty() ) {
            QMessageBox::information(this, "VM Viewer", "Job empty.");
        } else if ( type.toLower()=="lxc" ) {
            value = static_cast<LXC_Viewer*>(
                        VM_Displayed_Map.value(key, NULL));
        } else if ( type.toLower()=="qemu" || type.toLower()=="xen" ) {
            value = static_cast<Spice_Viewer*>(
                        VM_Displayed_Map.value(key, NULL));
        };
        if ( value!=NULL ) value->show();
        else VM_Displayed_Map.remove(key);
    };
}
void MainWindow::deleteVMDisplay(QString &key)
{
    if ( VM_Displayed_Map.contains(key) ) {
        // disconnect before close for don't loop it
        VM_Viewer *value = NULL;
        QString _type = VM_Displayed_Map.value(key, NULL)->TYPE.toUpper();
        if ( _type=="LXC" ) {
            value = static_cast<LXC_Viewer*>(
                        VM_Displayed_Map.value(key, NULL));
        } else if ( _type=="SPICE" ) {
            value = static_cast<Spice_Viewer*>(
                        VM_Displayed_Map.value(key, NULL));
        };
        if ( NULL!=value ) {
            disconnect(value, SIGNAL(finished(QString&)),
                       this, SLOT(deleteVMDisplay(QString&)));
            disconnect(value, SIGNAL(errorMsg(QString&)),
                       logDockContent, SLOT(appendErrorMsg(QString&)));
            disconnect(value, SIGNAL(addNewTask(TASK)),
                       taskWrHouse, SLOT(addNewTask(TASK)));
            delete value;
            value = NULL;
        };
        VM_Displayed_Map.remove(key);
        // reload domains state, because VM_Viewer is finished
        //domainDockContent->reloadState();
    }
}
void MainWindow::deleteVMDisplay(QString connName, QString domName)
{
    QString key = QString("%1_%2").arg(connName).arg(domName);
    if ( VM_Displayed_Map.contains(key) ) {
        VM_Viewer *value = NULL;
        QString _type = VM_Displayed_Map.value(key, NULL)->TYPE.toUpper();
        if ( _type=="LXC" ) {
            value = static_cast<LXC_Viewer*>(
                        VM_Displayed_Map.value(key, NULL));
        } else if ( _type=="SPICE" ) {
            value = static_cast<Spice_Viewer*>(
                        VM_Displayed_Map.value(key, NULL));
        };
        if ( NULL!=value ) {
            if ( value->isActive() ) value->close();
            // don't anymore, because VM_Viewer emit finished signal and
            // VM_Viewer will cleaned from VM list
            // in <MainWindow::deleteVMDisplay(QString&)>
        };
    };
}
void MainWindow::buildMigrateArgs(TASK _task)
{
    virConnectPtr namedConnect = connListWidget->getConnection(_task.args.path);
    if ( NULL!=namedConnect ) {
        domainDockContent->execMigrateAction(namedConnect, _task);
    }
}

void MainWindow::addStorageVol(virConnect *conn, QString &connName, QString &poolName)
{
    QString key = QString("%1_%2").arg(connName).arg(poolName);
    if ( !storageMap.contains(key) ) {
        storageMap.insert(key, new VirtStorageVolControl(this));
        storageMap.value(key)->setObjectName(key);
        storageMap.value(key)->setWindowTitle(QString("%1 Pool").arg(key));
        connect(storageMap.value(key), SIGNAL(entityMsg(QString&)),
                this, SLOT(writeToErrorLog(QString&)));
        connect(storageMap.value(key), SIGNAL(addNewTask(TASK)),
                taskWrHouse, SLOT(addNewTask(TASK)));
        connect(taskWrHouse, SIGNAL(volResult(Result)),
                storageMap.value(key), SLOT(resultReceiver(Result)));
        storageMap.value(key)->setCurrentStoragePool(conn, connName, poolName);
    };
    storageMap.value(key)->show();
    storageMap.value(key)->setFocus();
}
