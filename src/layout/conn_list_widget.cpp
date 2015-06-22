#include "layout/conn_list_widget.h"

ConnectionList::ConnectionList(QWidget *parent = NULL)
    : QTreeView(parent)
{
  this->setContextMenuPolicy ( Qt::CustomContextMenu );
  setContentsMargins (1, 1, 1, 1);
  setDefaultDropAction ( Qt::IgnoreAction );
  setCursor(Qt::ArrowCursor);
  setSortingEnabled(true);
  this->setItemsExpandable(false);
  this->setRootIsDecorated(false);
  connItemModel = new ConnItemModel(this);
  this->setModel(connItemModel);
  progressBarDlg = new ProgressBarDelegate();
  this->setItemDelegate(progressBarDlg);
  connections = new CONN_LIST();
  connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
          this, SLOT(connItemClicked(const QPoint &)));
  connect(this, SIGNAL(doubleClicked(const QModelIndex&)),
          this, SLOT(connItemDoubleClicked(const QModelIndex&)));
  searchThread = new SearchThread(this);
  connect(searchThread, SIGNAL(localConnFound(QString&)),
          this, SLOT(createLocalConnection(QString&)));
}
ConnectionList::~ConnectionList()
{
  disconnect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
             this, SLOT(connItemClicked(const QPoint &)));
  disconnect(this, SIGNAL(doubleClicked(const QModelIndex&)),
             this, SLOT(connItemDoubleClicked(const QModelIndex&)));
  disconnect(searchThread, SIGNAL(localConnFound(QString&)),
             this, SLOT(createLocalConnection(QString&)));
  delete searchThread;
  searchThread = NULL;
  delete progressBarDlg;
  progressBarDlg = NULL;
  delete connItemModel;
  connItemModel = NULL;
  QStringList keys = connections->keys();
  foreach (QString key, keys) {
      disconnect(connections->value(key), SIGNAL(warningShowed()),
                 this, SLOT(mainWindowUp()));
      disconnect(connections->value(key), SIGNAL(warning(QString&)),
                 this, SLOT(sendWarning(QString&)));
      disconnect(connections->value(key), SIGNAL(connPtr(virConnect*, QString&)),
                 this, SLOT(sendConnPtr(virConnect*, QString&)));
      disconnect(connections->value(key), SIGNAL(authRequested(QString&)),
                 this, SLOT(getAuthCredentials(QString&)));
      disconnect(connections->value(key), SIGNAL(domStateChanged(Result)),
                 this, SIGNAL(domResult(Result)));
      disconnect(connections->value(key), SIGNAL(connClosed(virConnect*)),
                 this, SIGNAL(connClosed(virConnect*)));
  };
  connections->clear();
  delete connections;
  connections = NULL;
}

/* public slots */
int  ConnectionList::connItemEditAction()
{
    int exitCode = 0;
    QModelIndex _item = currentIndex();
    if ( !_item.isValid() ) {
        showMessage("Info", "Item not exist.");
        return exitCode;
    };
    ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
    sDialog = new ConnSettings(this->parentWidget());
    sDialog->setConnectItem(idx);
    connect(sDialog, SIGNAL(creationConnCancelled()),
            this, SLOT(deleteCancelledCreation()));
    exitCode = sDialog->exec();
    disconnect(sDialog, SIGNAL(creationConnCancelled()),
               this, SLOT(deleteCancelledCreation()));

    sDialog->deleteLater();
    return exitCode;
}
void ConnectionList::searchLocalhostConnections()
{
    searchThread->start();
}
void ConnectionList::addConnItem(QString &s)
{
  int count = connItemModel->rowCount();
  bool exist = false;
  for (int i=0; i<count; i++) {
    ConnItemIndex *idx = connItemModel->connItemDataList.at(i);
    if ( idx->getName()==s ) {
        exist = true;
        break;
    }
  };
  if ( !exist ) {
      connItemModel->insertRow(0);
      QModelIndex item;
      for (int j=0; j<connItemModel->columnCount(); j++) {
          QString data;
          if (j) {
              data = "-";
          } else data = s;
          item = connItemModel->index(0,j);
          connItemModel->setData(item, data, Qt::EditRole);
      };
      int _createConnect = 1;
      if ( s==QString("<noname>") && item.isValid() ) {
          setCurrentIndex(item);
          _createConnect = connItemEditAction();
      };
      // check item if it exist and correct
      if ( _createConnect ) createConnection(item);
  };
}
void ConnectionList::deleteCurrentConnection()
{
  QModelIndex _item = currentIndex();
  if ( _item.isValid() ) {
      ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
      QString connection = idx->getName();
      ConnElement *conn;
      conn = connections->value(connection);
      int conn_state;
      bool conn_availability;
      conn_availability = idx->getData().value(QString("availability"), AVAILABLE).toBool();
      if ( !conn_availability ) {
          showMessage(QString("Connection '%1'").arg(connection), "Connection is Busy.");
          clearSelection();
          return;
      };
      conn_state = idx->getData().value(QString("isRunning"), STOPPED).toInt();
      if ( conn && conn_state==RUNNING ) {
          showMessage(QString("Connection '%1'").arg(connection), "Connection is Running.");
      } else {
          //if ( conn && !conn_state ) conn->closeConnection();
          disconnect(connections->value(connection), SIGNAL(warningShowed()),
                     this, SLOT(mainWindowUp()));
          disconnect(connections->value(connection), SIGNAL(warning(QString&)),
                     this, SLOT(sendWarning(QString&)));
          disconnect(connections->value(connection), SIGNAL(connPtr(virConnect*, QString&)),
                     this, SLOT(sendConnPtr(virConnect*, QString&)));
          disconnect(connections->value(connection), SIGNAL(authRequested(QString&)),
                     this, SLOT(getAuthCredentials(QString&)));
          disconnect(connections->value(connection), SIGNAL(domStateChanged(Result)),
                     this, SIGNAL(domResult(Result)));
          disconnect(connections->value(connection), SIGNAL(connClosed(virConnect*)),
                     this, SIGNAL(connClosed(virConnect*)));
          emit removeConnection(connection);
          connections->remove(connection);
          connItemModel->removeRow(_item.row());
      };
  } else showMessage("Info", "Item not exist.");
}
void ConnectionList::openConnection(QModelIndex &_item)
{
  checkConnection(_item, TO_RUN);
}
void ConnectionList::showConnection(QModelIndex &_item)
{
    for (int i=0; i<connections->count(); i++) {
        int conn_state;
        bool conn_availability;
        ConnItemIndex *idx = connItemModel->connItemDataList.at(i);
        if ( idx==NULL ) return;
        QString _name = idx->getName();
        ConnElement *conn;
        conn = connections->value(_name);
        conn->setOnViewConnAliveThread(i==_item.row());
        //qDebug()<<_name<<(i==_item.row());
        if ( i==_item.row() ) {
            conn_state = idx->getData().value(QString("isRunning"), STOPPED).toInt();
            conn_availability = idx->getData().value(QString("availability"), NOT_AVAILABLE).toBool();
            if ( conn_state==RUNNING && conn_availability ) {
                conn->showConnectionData();
            };
        } else
            connItemModel->setData(connItemModel->index(i, 0), false, Qt::DecorationRole);
    };
    clearSelection();
}
void ConnectionList::closeConnection(QModelIndex &_item)
{
  checkConnection(_item, TO_STOP);
}
virConnectPtr ConnectionList::getConnection(QString &name)
{
    return (connections->contains(name)) ?
            connections->value(name)->getConnection()
            : NULL;
}
void ConnectionList::stopProcessing()
{
    for (int i=0; i<connections->count(); i++) {
        ConnItemIndex *idx = connItemModel->connItemDataList.at(i);
        if ( idx==NULL ) return;
        QString _name = idx->getName();
        ConnElement *conn;
        conn = connections->value(_name);
        conn->setOnViewConnAliveThread(false);
        connItemModel->setData(connItemModel->index(i, 0), false, Qt::DecorationRole);
    };
    clearSelection();
}

/* private slots */
void ConnectionList::connItemClicked(const QPoint &pos)
{
  QModelIndex _item = indexAt(pos);
  if ( !_item.isValid() ) {
      clearSelection();
      //showMessage("Info", "Item not exist.");
      return;
  };
  //qDebug()<<_item->text()<<" Connection detected";
  DATA conn_Status;
  ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
  conn_Status = idx->getData();
  if ( !conn_Status.value("availability", NOT_AVAILABLE).toBool() ) return;
  bool to_run = TO_RUN;
  ConnectMenu *connectMenu = new ConnectMenu(this);
  if ( conn_Status.value("isRunning", STOPPED)==RUNNING ) {
      connectMenu->act->setText("Close Connection");
      connectMenu->act->setIcon(QIcon::fromTheme("stop"));
      connect(connectMenu->act, SIGNAL(triggered()),
              this, SLOT(connItemKillAction()));
      //connectMenu->clean->setEnabled(true);
      connectMenu->display->setEnabled(true);
      to_run = TO_STOP;
  } else {
      connectMenu->act->setText("Open Connection");
      connectMenu->act->setIcon(QIcon::fromTheme("run"));
      connect(connectMenu->act, SIGNAL(triggered()), this, SLOT(connItemRunAction()));
      //connectMenu->clean->setEnabled(false);
      connectMenu->display->setEnabled(false);
      to_run = TO_RUN;
  };
  idx->setData(conn_Status);
  connect(connectMenu->edit, SIGNAL(triggered()),
          this, SLOT(connItemEditAction()));
  connect(connectMenu->display, SIGNAL(triggered()),
          this, SLOT(connItemShowAction()));
  connect(connectMenu->clean, SIGNAL(triggered()),
          this, SLOT(deleteCurrentConnection()));
  connect(connectMenu->refresh, SIGNAL(triggered()),
          this, SLOT(refreshLocalhostConnection()));
  connectMenu->move(mapToGlobal(pos));
  connectMenu->exec();
  if (to_run) disconnect(connectMenu->act, SIGNAL(triggered()),
                         this, SLOT(connItemRunAction()));
  else disconnect(connectMenu->act, SIGNAL(triggered()),
                  this, SLOT(connItemKillAction()));
  disconnect(connectMenu->edit, SIGNAL(triggered()),
             this, SLOT(connItemEditAction()));
  disconnect(connectMenu->display, SIGNAL(triggered()),
             this, SLOT(connItemShowAction()));
  disconnect(connectMenu->clean, SIGNAL(triggered()),
             this, SLOT(deleteCurrentConnection()));
  disconnect(connectMenu->refresh, SIGNAL(triggered()),
             this, SLOT(refreshLocalhostConnection()));
  connectMenu->deleteLater();
}
void ConnectionList::connItemDoubleClicked(const QModelIndex &_item)
{
  ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
  QString _name = idx->getName();
  DATA conn_Status;
  conn_Status = idx->getData();
  QString key = conn_Status.value(QString("initName")).toString();
  ConnElement *conn = static_cast<ConnElement*>(connections->value(key));
  if ( NULL==conn ) {
      clearSelection();
      return;
  };
  if ( key != _name ) {
      conn_Status.insert(QString("initName"), QVariant(_name));
      connections->insert(_name, conn);
      connections->remove(key);
      conn->setItemReference(connItemModel, idx);
  };
  //qDebug()<<key<<" Connection doubleClicked"<<conn;
  int conn_state;
  conn_state = conn_Status.value(QString("isRunning"), STOPPED).toInt();
  if ( !conn_Status.value(QString("availability"), NOT_AVAILABLE).toBool() ) {
      showMessage("Info", "Connection is busy.");
  } else if ( conn_state!=RUNNING ) {
      conn->openConnection();
  } else if ( conn_state==RUNNING ) {
      emit connToClose(_item.row());
      conn->closeConnection();
  };
  clearSelection();
}
void ConnectionList::connItemKillAction()
{
  QModelIndex _item = currentIndex();
  checkConnection(_item, TO_STOP);
}
void ConnectionList::connItemRunAction()
{
    QModelIndex _item = currentIndex();
    checkConnection(_item, TO_RUN);
}
void ConnectionList::connItemShowAction()
{
    QModelIndex _item = currentIndex();
    showConnection(_item);
}
void ConnectionList::createConnection(QModelIndex &_item)
{
  ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
  QString key = idx->getName();
  ConnElement *conn = new ConnElement(this);
  conn->setItemReference(connItemModel, idx);
  connections->insert(key, conn);
  clearSelection();
  connect(connections->value(key), SIGNAL(warningShowed()),
          this, SLOT(mainWindowUp()));
  connect(connections->value(key), SIGNAL(warning(QString&)),
          this, SLOT(sendWarning(QString&)));
  connect(connections->value(key), SIGNAL(connPtr(virConnect*, QString&)),
          this, SLOT(sendConnPtr(virConnect*, QString&)));
  connect(connections->value(key), SIGNAL(authRequested(QString&)),
          this, SLOT(getAuthCredentials(QString&)));
  connect(connections->value(key), SIGNAL(domStateChanged(Result)),
          this, SIGNAL(domResult(Result)));
  connect(connections->value(key), SIGNAL(connClosed(virConnect*)),
          this, SIGNAL(connClosed(virConnect*)));
  //qDebug()<<key<<" create Connection item";
  if ( !searchThread->isFinished() && !searchThread->isRunning() ) {
      QString uri(conn->getURI());
      searchThread->compareURI(uri);
  };
}
void ConnectionList::createLocalConnection(QString &uri)
{
    //qDebug()<<uri<<"create connection";
    localConn ++;
    QString s = QString("{Local%2_%1}")
            .arg(uri.split(":///").first().toUpper())
            .arg(localConn);
    connItemModel->insertRow(0);
    QModelIndex item;
    for (int j=0; j<connItemModel->columnCount(); j++) {
        QString data;
        if (j==2) {
            data = "-";
        } else if (j==1) {
            data = uri;
        } else data = s;
        item = connItemModel->index(0,j);
        connItemModel->setData(item, data, Qt::EditRole);
    };
    ConnItemIndex *idx = connItemModel->connItemDataList.at(item.row());
    QString key = idx->getName();
    ConnElement *conn = new ConnElement(this);
    conn->setName(s);
    conn->setURI(uri);
    conn->setItemReferenceForLocal(connItemModel, idx);
    connections->insert(key, conn);
    clearSelection();
    connect(connections->value(key), SIGNAL(warningShowed()),
            this, SLOT(mainWindowUp()));
    connect(connections->value(key), SIGNAL(warning(QString&)),
            this, SLOT(sendWarning(QString&)));
    connect(connections->value(key), SIGNAL(connPtr(virConnect*, QString&)),
            this, SLOT(sendConnPtr(virConnect*, QString&)));
    connect(connections->value(key), SIGNAL(authRequested(QString&)),
            this, SLOT(getAuthCredentials(QString&)));
    connect(connections->value(key), SIGNAL(domStateChanged(Result)),
            this, SIGNAL(domResult(Result)));
    connect(connections->value(key), SIGNAL(connClosed(virConnect*)),
            this, SIGNAL(connClosed(virConnect*)));
    //qDebug()<<key<<" create Local Connection item";
}
void ConnectionList::refreshLocalhostConnection()
{
    searchThread->setURIList();
    localConn = 0;
    foreach (QString key, connections->keys()) {
        ConnElement *el = static_cast<ConnElement*>(
                    connections->value(key));
        if ( NULL!=el ) {
            QRegExp rx("^\\{Local([0-9]+)_([A-Z]+)\\}$");
            QString _name = el->getName();
            if ( _name.contains(rx) ) {
                //qDebug()<<_name;
                el->forceCloseConnection();
                int count = connItemModel->rowCount();
                ConnItemIndex *idx = NULL;
                for (int i=0; i<count; i++) {
                    idx = connItemModel->connItemDataList.at(i);
                    if ( idx->getName()==_name ) {
                        int row = connItemModel->connItemDataList.indexOf(idx);
                        connItemModel->removeRow(row);
                        delete el;
                        el = NULL;
                        connections->remove(_name);
                        break;
                    };
                };
            } else {
                QString uri(el->getURI());
                searchThread->compareURI(uri);
            };
        };
    }
    if ( !searchThread->isRunning() ) {
        searchLocalhostConnections();
    };
}
void ConnectionList::checkConnection(QModelIndex &_item, bool to_run = TO_RUN)
{
  int conn_state;
  ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
  conn_state = idx->getData().value(QString("isRunning"), STOPPED).toInt();
  if ( (to_run && conn_state!=RUNNING) || (!to_run && conn_state==RUNNING) )
    connItemDoubleClicked(_item);
}
void ConnectionList::deleteCancelledCreation()
{
  deleteCurrentConnection();
}
void ConnectionList::showMessage(QString title, QString msg)
{
  QMessageBox::information(this, title, msg);
  mainWindowUp();
}
void ConnectionList::sendWarning(QString &msg)
{
    emit warning(msg);
}
void ConnectionList::mainWindowUp()
{
    emit messageShowed();
}
void ConnectionList::sendConnPtr(virConnect *conn, QString &name)
{
    emit connPtr(conn, name);
}
void ConnectionList::getAuthCredentials(QString &crd)
{
    ConnElement *obj = static_cast<ConnElement*>(sender());
    QString text;
    QLineEdit::EchoMode mode = (crd.toLower()=="password") ?
                QLineEdit::PasswordEchoOnEdit :
                QLineEdit::Normal;
    text = QInputDialog::getText(
                    this,
                    QString("<%1> credentials").arg(obj->getName()),
                    crd,
                    mode);
    obj->setAuthCredentials(crd, text);
}
