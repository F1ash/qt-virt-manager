#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    setMinimumSize(100, 100);
    setContentsMargins(0, 0, 0, 5);
    setWindowTitle("Qt VirtManager");
    QStringList searchThemePath;
    // TODO: make a cross platform here
    searchThemePath.append(QIcon::themeSearchPaths());
    searchThemePath.insert(0, "/usr/share/sandbox-runner-data/");
    QIcon::setThemeSearchPaths(searchThemePath);
    //
    QIcon::setThemeName("icons");
    setWindowIcon(QIcon::fromTheme("virtual-engineering"));
    setMouseTracking(true);
    setDockOptions(QMainWindow::AnimatedDocks|
                   QMainWindow::ForceTabbedDocks
    );
    restoreGeometry(settings.value("Geometry").toByteArray());
    initTrayIcon();
    initConnListWidget();
    initToolBar();
    initDockWidgets();
    restoreState(settings.value("State").toByteArray());
    this->setVisible(!settings.value("Visible", false).toBool());
    changeVisibility();
}
MainWindow::~MainWindow()
{
  disconnect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, \
                    SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
  disconnect(trayIcon->hideAction, SIGNAL(triggered()), this, SLOT(changeVisibility()));
  disconnect(trayIcon->logUpAction, SIGNAL(triggered()), this, SLOT(changeLogViewerVisibility()));
  disconnect(trayIcon->closeAction, SIGNAL(triggered()), this, SLOT(closeEvent()));
  disconnect(connListWidget, SIGNAL(removeConnect(QString&)), this, SLOT(removeConnectItem(QString&)));
  disconnect(connListWidget, SIGNAL(messageShowed()), this, SLOT(mainWindowUp()));
  disconnect(connListWidget, SIGNAL(warning(QString&)), this, SLOT(writeToErrorLog(QString&)));
  disconnect(connListWidget, SIGNAL(connPtr(virConnect*)), this, SLOT(receiveConnPtr(virConnect*)));
  disconnect(connListWidget, SIGNAL(connectClosed()), this, SLOT(stopProcessing()));
  disconnect(toolBar->_hideAction, SIGNAL(triggered()), this, SLOT(changeVisibility()));
  disconnect(toolBar->_createAction, SIGNAL(triggered()), this, SLOT(createNewConnect()));
  disconnect(toolBar->_editAction, SIGNAL(triggered()), this, SLOT(editCurrentConnect()));
  disconnect(toolBar->_deleteAction, SIGNAL(triggered()), this, SLOT(deleteCurrentConnect()));
  disconnect(toolBar->_openAction, SIGNAL(triggered()), this, SLOT(openCurrentConnect()));
  disconnect(toolBar->_showAction, SIGNAL(triggered()), this, SLOT(showCurrentConnect()));
  disconnect(toolBar->_closeAction, SIGNAL(triggered()), this, SLOT(closeCurrentConnect()));
  disconnect(toolBar->_closeAllAction, SIGNAL(triggered()), this, SLOT(closeAllConnect()));
  disconnect(toolBar->_logUpAction, SIGNAL(triggered()), this, SLOT(changeLogViewerVisibility()));
  disconnect(toolBar->_exitAction, SIGNAL(triggered()), this, SLOT(closeEvent()));
  disconnect(toolBar, SIGNAL(warningShowed()), this, SLOT(mainWindowUp()));
  disconnect(toolBar->_domUpAction, SIGNAL(triggered(bool)), domainDoc, SLOT(setVisible(bool)));
  disconnect(toolBar->_netUpAction, SIGNAL(triggered(bool)), networkDoc, SLOT(setVisible(bool)));
  disconnect(toolBar->_stVolUpAction, SIGNAL(triggered(bool)), storageVolDoc, SLOT(setVisible(bool)));
  disconnect(toolBar->_stPoolUpAction, SIGNAL(triggered(bool)), storagePoolDoc, SLOT(setVisible(bool)));
  disconnect(networkDocContent, SIGNAL(netMsg(QString&)), this, SLOT(writeToErrorLog(QString&)));

  qDebug()<<"processing stopped";
  if ( wait_thread!=NULL ) {
      disconnect(wait_thread, SIGNAL(finished()), this, SLOT(closeEvent()));
      disconnect(wait_thread, SIGNAL(refreshProcessingState()), this, SLOT(stopProcessing()));
      delete wait_thread;
      wait_thread = 0;
  };

  delete logDocContent;
  logDocContent = 0;
  delete logDoc;
  logDoc = 0;

  delete domainDocContent;
  domainDocContent = 0;
  delete domainDoc;
  domainDoc = 0;

  delete networkDocContent;
  networkDocContent = 0;
  delete networkDoc;
  networkDoc = 0;

  delete storageVolDocContent;
  storageVolDocContent = 0;
  delete storageVolDoc;
  storageVolDoc = 0;

  delete storagePoolDocContent;
  storagePoolDocContent = 0;
  delete storagePoolDoc;
  storagePoolDoc = 0;

  delete connListWidget;
  connListWidget = 0;

  delete toolBar;
  toolBar = 0;

  delete trayIcon;
  trayIcon = 0;
  qDebug()<<"application stopped";
}
void MainWindow::closeEvent(QCloseEvent *ev)
{
  settings.setValue("Geometry", saveGeometry());
  settings.setValue("State", saveState());
  settings.setValue("ToolBarArea", toolBarArea(toolBar));
  settings.setValue("Visible", this->isVisible());
  settings.beginGroup("LogDoc");
  settings.setValue("DocArea", dockWidgetArea(logDoc));
  settings.setValue("Visible", logDoc->isVisible());
  settings.setValue("Floating", logDoc->isFloating());
  settings.setValue("Geometry", logDoc->saveGeometry());
  settings.endGroup();
  settings.beginGroup("DomainDoc");
  settings.setValue("DocArea", dockWidgetArea(domainDoc));
  settings.setValue("Visible", domainDoc->isVisible());
  settings.setValue("Floating", domainDoc->isFloating());
  settings.setValue("Geometry", domainDoc->saveGeometry());
  settings.endGroup();
  settings.beginGroup("NetworkDoc");
  settings.setValue("DocArea", dockWidgetArea(networkDoc));
  settings.setValue("Visible", networkDoc->isVisible());
  settings.setValue("Floating", networkDoc->isFloating());
  settings.setValue("Geometry", networkDoc->saveGeometry());
  settings.endGroup();
  settings.beginGroup("StorageVolDoc");
  settings.setValue("DocArea", dockWidgetArea(storageVolDoc));
  settings.setValue("Visible", storageVolDoc->isVisible());
  settings.setValue("Floating", storageVolDoc->isFloating());
  settings.setValue("Geometry", storageVolDoc->saveGeometry());
  settings.endGroup();
  settings.beginGroup("StoragePoolDoc");
  settings.setValue("DocArea", dockWidgetArea(storagePoolDoc));
  settings.setValue("Visible", storagePoolDoc->isVisible());
  settings.setValue("Floating", storagePoolDoc->isFloating());
  settings.setValue("Geometry", storagePoolDoc->saveGeometry());
  settings.endGroup();
  settings.beginGroup("ConnectListColumns");
  settings.setValue("column0", connListWidget->columnWidth(0));
  settings.setValue("column1", connListWidget->columnWidth(1));
  settings.setValue("column2", connListWidget->columnWidth(2));
  settings.endGroup();
  settings.sync();
  if ( !this->isVisible() ) changeVisibility();
  if ( runningConnectsExist() && wait_thread==NULL ) {
      QString q;
      q.append("Running Connects are exist.\nKill it at exit?");
      int answer = QMessageBox::question(this, "Action", q, QMessageBox::Yes, QMessageBox::Cancel);
      if ( answer == QMessageBox::Cancel ) {
          ev->ignore();
          return;
      };
      connListWidget->setEnabled(false);
      toolBar->setEnabled(false);
      logDoc->setEnabled(false);
      wait_thread = new Wait(this);
      wait_thread->setPtr(connListWidget);
      connect(wait_thread, SIGNAL(finished()), this, SLOT(closeEvent()));
      connect(wait_thread, SIGNAL(refreshProcessingState()), this, SLOT(stopProcessing()));
      wait_thread->start();
      ev->ignore();
  } else if ( !runningConnectsExist() && (wait_thread==NULL || wait_thread->isFinished()) ) {
      ev->accept();
  } else {
      ev->ignore();
  };
}
void MainWindow::closeEvent()
{
    this->close();
}
void MainWindow::initTrayIcon()
{
    trayIcon = new TrayIcon(this);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, \
                      SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(trayIcon->hideAction, SIGNAL(triggered()), this, SLOT(changeVisibility()));
    connect(trayIcon->logUpAction, SIGNAL(triggered()), this, SLOT(changeLogViewerVisibility()));
    connect(trayIcon->closeAction, SIGNAL(triggered()), this, SLOT(closeEvent()));
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
        if ( domainDoc->isFloating() ) domainDoc->hide();
        if ( networkDoc->isFloating() ) networkDoc->hide();
        if ( storageVolDoc->isFloating() ) storageVolDoc->hide();
        if ( storagePoolDoc->isFloating() ) storagePoolDoc->hide();
    } else {
        this->show();
        trayIcon->hideAction->setText (QString("Down"));
        trayIcon->hideAction->setIcon (QIcon::fromTheme("down"));
        if ( domainDoc->isFloating() && toolBar->_domUpAction->isChecked() ) domainDoc->show();
        if ( networkDoc->isFloating() && toolBar->_netUpAction->isChecked() ) networkDoc->show();
        if ( storageVolDoc->isFloating() && toolBar->_stVolUpAction->isChecked() ) storageVolDoc->show();
        if ( storagePoolDoc->isFloating() && toolBar->_stPoolUpAction->isChecked() ) storagePoolDoc->show();
    };
}
void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason r)
{
  if (r==QSystemTrayIcon::Trigger) changeVisibility();
}
void MainWindow::initConnListWidget()
{
  connListWidget = new ConnectList(this);
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
      connListWidget->addConnectItem(s);
  };
  setCentralWidget(connListWidget);
  connect(connListWidget, SIGNAL(removeConnect(QString&)), this, SLOT(removeConnectItem(QString&)));
  connect(connListWidget, SIGNAL(messageShowed()), this, SLOT(mainWindowUp()));
  connect(connListWidget, SIGNAL(warning(QString&)), this, SLOT(writeToErrorLog(QString&)));
  connect(connListWidget, SIGNAL(connPtr(virConnect*)), this, SLOT(receiveConnPtr(virConnect*)));
  connect(connListWidget, SIGNAL(connectClosed()), this, SLOT(stopProcessing()));
}
void MainWindow::initToolBar()
{
  toolBar = new ToolBar(this);
  toolBar->setObjectName("toolBar");
  connect(toolBar->_hideAction, SIGNAL(triggered()), this, SLOT(changeVisibility()));
  connect(toolBar->_createAction, SIGNAL(triggered()), this, SLOT(createNewConnect()));
  connect(toolBar->_editAction, SIGNAL(triggered()), this, SLOT(editCurrentConnect()));
  connect(toolBar->_deleteAction, SIGNAL(triggered()), this, SLOT(deleteCurrentConnect()));
  connect(toolBar->_openAction, SIGNAL(triggered()), this, SLOT(openCurrentConnect()));
  connect(toolBar->_showAction, SIGNAL(triggered()), this, SLOT(showCurrentConnect()));
  connect(toolBar->_closeAction, SIGNAL(triggered()), this, SLOT(closeCurrentConnect()));
  connect(toolBar->_closeAllAction, SIGNAL(triggered()), this, SLOT(closeAllConnect()));
  connect(toolBar->_logUpAction, SIGNAL(triggered()), this, SLOT(changeLogViewerVisibility()));
  connect(toolBar->_exitAction, SIGNAL(triggered()), this, SLOT(closeEvent()));
  connect(toolBar, SIGNAL(warningShowed()), this, SLOT(mainWindowUp()));
  int area_int = settings.value("ToolBarArea", 4).toInt();
  this->addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
}
void MainWindow::initDockWidgets()
{
    bool visible;
    Qt::DockWidgetArea area;
    logDoc = new QDockWidget(this);
    logDoc->setObjectName("logDoc");
    logDoc->setWindowTitle("Log:");
    logDoc->setFeatures(
        QDockWidget::DockWidgetMovable   |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar
    );
    logDocContent = new LogDocContent(this);
    logDoc->setWidget( logDocContent );
    settings.beginGroup("LogDoc");
    logDoc->setFloating(settings.value("Floating", false).toBool());
    logDoc->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = !settings.value("Visible", false).toBool();
    logDoc->setVisible(visible);
    changeLogViewerVisibility();
    area = getDocArea(settings.value("DocArea", Qt::BottomDockWidgetArea).toInt());
    settings.endGroup();
    addDockWidget(area, logDoc);

    domainDoc = new QDockWidget(this);
    domainDoc->setObjectName("domainDoc");
    domainDoc->setWindowTitle("Domain:");
    domainDoc->setFeatures(
        QDockWidget::DockWidgetMovable   |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar
    );
    domainDocContent = new DomainDocContent(this);
    domainDoc->setWidget( domainDocContent );
    settings.beginGroup("DomainDoc");
    domainDoc->setFloating(settings.value("Floating", false).toBool());
    domainDoc->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    domainDoc->setVisible(visible);
    toolBar->_domUpAction->setChecked(visible);
    connect(toolBar->_domUpAction, SIGNAL(triggered(bool)), domainDoc, SLOT(setVisible(bool)));
    area = getDocArea(settings.value("DocArea", Qt::BottomDockWidgetArea).toInt());
    settings.endGroup();
    addDockWidget(area, domainDoc);

    networkDoc = new QDockWidget(this);
    networkDoc->setObjectName("networkDoc");
    networkDoc->setWindowTitle("Network:");
    networkDoc->setFeatures(
        QDockWidget::DockWidgetMovable   |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar
    );
    networkDocContent = new VirtNetControl(this);
    networkDoc->setWidget( networkDocContent );
    settings.beginGroup("NetworkDoc");
    networkDoc->setFloating(settings.value("Floating", false).toBool());
    networkDoc->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    networkDoc->setVisible(visible);
    toolBar->_netUpAction->setChecked(visible);
    area = getDocArea(settings.value("DocArea", Qt::BottomDockWidgetArea).toInt());
    settings.endGroup();
    addDockWidget(area, networkDoc);
    connect(toolBar->_netUpAction, SIGNAL(triggered(bool)), networkDoc, SLOT(setVisible(bool)));
    connect(networkDocContent, SIGNAL(netMsg(QString&)), this, SLOT(writeToErrorLog(QString&)));

    storageVolDoc = new QDockWidget(this);
    storageVolDoc->setObjectName("storageVolDoc");
    storageVolDoc->setWindowTitle("StorageVol:");
    storageVolDoc->setFeatures(
        QDockWidget::DockWidgetMovable   |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar
    );
    storageVolDocContent = new StorageVolDocContent(this);
    storageVolDoc->setWidget( storageVolDocContent );
    settings.beginGroup("StorageVolDoc");
    storageVolDoc->setFloating(settings.value("Floating", false).toBool());
    storageVolDoc->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    storageVolDoc->setVisible(visible);
    toolBar->_stVolUpAction->setChecked(visible);
    area = getDocArea(settings.value("DocArea", Qt::BottomDockWidgetArea).toInt());
    settings.endGroup();
    addDockWidget(area, storageVolDoc);
    connect(toolBar->_stVolUpAction, SIGNAL(triggered(bool)), storageVolDoc, SLOT(setVisible(bool)));

    storagePoolDoc = new QDockWidget(this);
    storagePoolDoc->setObjectName("storagePoolDoc");
    storagePoolDoc->setWindowTitle("StoragePool:");
    storagePoolDoc->setFeatures(
        QDockWidget::DockWidgetMovable   |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar
    );
    storagePoolDocContent = new StoragePoolDocContent(this);
    storagePoolDoc->setWidget( storagePoolDocContent );
    settings.beginGroup("StoragePoolDoc");
    storagePoolDoc->setFloating(settings.value("Floating", false).toBool());
    storagePoolDoc->restoreGeometry(settings.value("Geometry").toByteArray());
    visible = settings.value("Visible", false).toBool();
    storagePoolDoc->setVisible(visible);
    toolBar->_stPoolUpAction->setChecked(visible);
    area = getDocArea(settings.value("DocArea", Qt::BottomDockWidgetArea).toInt());
    settings.endGroup();
    addDockWidget(area, storagePoolDoc);
    connect(toolBar->_stPoolUpAction, SIGNAL(triggered(bool)), storagePoolDoc, SLOT(setVisible(bool)));
}
void MainWindow::editCurrentConnect()
{
  connListWidget->connectItemEditAction();
}
void MainWindow::createNewConnect()
{
  QString s = QString("<noname>");
  connListWidget->addConnectItem(s);
}
void MainWindow::deleteCurrentConnect()
{
  connListWidget->deleteCurrentConnect();
}
void MainWindow::removeConnectItem(QString &connect)
{
  settings.beginGroup("Connects");
  settings.remove(connect);
  settings.endGroup();
  //qDebug()<<connect<<"connect deleted";
}
void MainWindow::openCurrentConnect()
{
    QModelIndex _item = connListWidget->currentIndex();
    if (_item.isValid()) {
        connListWidget->openConnect(_item);
    };
}
void MainWindow::showCurrentConnect()
{
    QModelIndex _item = connListWidget->currentIndex();
    if (_item.isValid()) {
        connListWidget->showConnect(_item);
    };
}
void MainWindow::closeCurrentConnect()
{
    QModelIndex _item = connListWidget->currentIndex();
    if (_item.isValid()) {
        connListWidget->closeConnect(_item);
    };
}
void MainWindow::closeAllConnect()
{
  int count = connListWidget->connItemModel->rowCount();
  for (int i = 0; i< count; i++) closeConnect(i);
}
void MainWindow::closeConnect(int i)
{
  //qDebug()<<i<<" item to stop";
  QModelIndex _item = connListWidget->connItemModel->index(i, 0);
  connListWidget->closeConnect(_item);
}
bool MainWindow::runningConnectsExist()
{
    bool result = false;
    int count = connListWidget->connItemModel->rowCount();
    for (int i=0; i<count; i++) {
        ConnItemIndex *item = connListWidget->connItemModel->connItemDataList.at(i);
        //qDebug()<<connListWidget->item(i)->text()<< connListWidget->item(i)->data(Qt::UserRole).toMap().value("isRunning").toBool();
        if ( item->getData().value("isRunning").toBool() ||
            !item->getData().value("availability").toBool() ) {
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
    logDocContent->appendErrorMsg(msg);
}
void MainWindow::changeLogViewerVisibility()
{
    QString text;
    if ( logDoc->isVisible() ) {
        logDoc->hide();
        text = "Show Log Viewer";
    } else {
        logDoc->show();
        text = "Hide Log Viewer";
    };
    trayIcon->setLogUpActionText(text);
    toolBar->_logUpAction->setChecked(logDoc->isVisible());
}

Qt::DockWidgetArea MainWindow::getDocArea(int i) const
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
void MainWindow::receiveConnPtr(virConnect *conn)
{
    // send connect ptr to all related virtual resources for operating
    networkDocContent->setCurrentWorkConnect(conn);
}
void MainWindow::stopProcessing()
{
    bool result = true;
    // stop processing of all virtual resources
    domainDocContent->stopProcessing();
    networkDocContent->stopProcessing();
    storageVolDocContent->stopProcessing();
    storagePoolDocContent->stopProcessing();
    //result = result && domainDocContent->getThreadState();
    result = result && networkDocContent->getThreadState() ;
    //result = result && storageVolDocContent->getThreadState();
    //result = result && storagePoolDocContent->getThreadState();
    if ( wait_thread!=NULL ) wait_thread->setProcessingState(result);
}
