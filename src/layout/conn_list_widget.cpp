#include "layout/conn_list_widget.h"

ConnectList::ConnectList(QWidget *parent = 0)
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
  connects = new CONN_LIST();
  connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(connectItemClicked(const QPoint &)));
  connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(connectItemDoubleClicked(const QModelIndex&)));
}
ConnectList::~ConnectList()
{
  disconnect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(connectItemClicked(const QPoint &)));
  disconnect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(connectItemDoubleClicked(const QModelIndex&)));
  delete progressBarDlg;
  progressBarDlg = 0;
  delete connItemModel;
  connItemModel = 0;
  QStringList keys = connects->keys();
  foreach (QString key, keys) {
      disconnect(connects->value(key), SIGNAL(warningShowed()), this, SLOT(mainWindowUp()));
      disconnect(connects->value(key), SIGNAL(warning(QString&)), this, SLOT(sendWarning(QString&)));
      disconnect(connects->value(key), SIGNAL(connPtr(virConnect*, QString&)), this, SLOT(sendConnPtr(virConnect*, QString&)));
      disconnect(connects->value(key), SIGNAL(authRequested(QString&)), this, SLOT(getAuthCredentials(QString&)));
  };
  connects->clear();
  delete connects;
  connects = 0;
}

/* public slots */
int  ConnectList::connectItemEditAction()
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
    connect(sDialog, SIGNAL(creatingConnectCancelled()), this, SLOT(deleteCancelledCreation()));
    exitCode = sDialog->exec();
    disconnect(sDialog, SIGNAL(creatingConnectCancelled()), this, SLOT(deleteCancelledCreation()));

    sDialog->deleteLater();
    return exitCode;
}
void ConnectList::addConnectItem(QString &s)
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
          _createConnect = connectItemEditAction();
      };
      // check item if it exist and correct
      if ( _createConnect ) createConnect(item);
  };
}
void ConnectList::deleteCurrentConnect()
{
  QModelIndex _item = currentIndex();
  if ( _item.isValid() ) {
      ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
      QString connect = idx->getName();
      ElemConnect *conn;
      conn = connects->value(connect);
      int conn_state;
      bool conn_availability;
      conn_availability = idx->getData().value(QString("availability"), AVAILABLE).toBool();
      if ( !conn_availability ) {
          showMessage(QString("Connect '%1'").arg(connect), "Connect is Busy.");
          clearSelection();
          return;
      };
      conn_state = idx->getData().value(QString("isRunning"), STOPPED).toInt();
      if ( conn && conn_state==RUNNING ) {
          showMessage(QString("Connect '%1'").arg(connect), "Connect is Running.");
      } else {
          //if ( conn && !conn_state ) conn->closeConnect();
          connects->remove(connect);
          connItemModel->removeRow(_item.row());
          //disconnect(connects->value(connect), SIGNAL(warningShowed()), this, SLOT(mainWindowUp()));
          //disconnect(connects->value(connect), SIGNAL(warning(QString&)), this, SLOT(sendWarning(QString&)));
          //disconnect(connects->value(key), SIGNAL(connPtr(virConnect*, QString&)), this, SLOT(sendConnPtr(virConnect*, QString&)));
          //disconnect(connects->value(key), SIGNAL(authRequested(QString&)), this, SLOT(getAuthCredentials(QString&)));
          emit removeConnect(connect);
      };
  } else showMessage("Info", "Item not exist.");
}
void ConnectList::openConnect(QModelIndex &_item)
{
  checkConnect(_item, TO_RUN);
}
void ConnectList::showConnect(QModelIndex &_item)
{
    int conn_state;
    bool conn_availability;
    ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
    conn_state = idx->getData().value(QString("isRunning"), STOPPED).toInt();
    conn_availability = idx->getData().value(QString("availability"), NOT_AVAILABLE).toBool();
    if ( conn_state==RUNNING && conn_availability ) {
        QString _name = idx->getName();
        ElemConnect *conn;
        conn = connects->value(_name);
        conn->showConnectData();
    };
    clearSelection();
}
void ConnectList::closeConnect(QModelIndex &_item)
{
  checkConnect(_item, TO_STOP);
}
virConnectPtr ConnectList::getConnect(QString &name)
{
    return (connects->contains(name)) ?
            connects->value(name)->getConnect()
            : NULL;
}

/* private slots */
void ConnectList::connectItemClicked(const QPoint &pos)
{
  QModelIndex _item = indexAt(pos);
  if ( !_item.isValid() ) {
      clearSelection();
      //showMessage("Info", "Item not exist.");
      return;
  };
  //qDebug()<<_item->text()<<" Connect detected";
  DATA conn_Status;
  ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
  conn_Status = idx->getData();
  if ( !conn_Status.value("availability", NOT_AVAILABLE).toBool() ) return;
  bool to_run = TO_RUN;
  ConnectMenu *connectMenu = new ConnectMenu(this);
  if ( conn_Status.value("isRunning", STOPPED)==RUNNING ) {
      connectMenu->act->setText("Close Connect");
      connectMenu->act->setIcon(QIcon::fromTheme("stop"));
      connect(connectMenu->act, SIGNAL(triggered()), this, SLOT(connectItemKillAction()));
      //connectMenu->clean->setEnabled(true);
      connectMenu->display->setEnabled(true);
      to_run = TO_STOP;
  } else {
      connectMenu->act->setText("Open Connect");
      connectMenu->act->setIcon(QIcon::fromTheme("run"));
      connect(connectMenu->act, SIGNAL(triggered()), this, SLOT(connectItemRunAction()));
      //connectMenu->clean->setEnabled(false);
      connectMenu->display->setEnabled(false);
      to_run = TO_RUN;
  };
  idx->setData(conn_Status);
  connect(connectMenu->edit, SIGNAL(triggered()), this, SLOT(connectItemEditAction()));
  connect(connectMenu->display, SIGNAL(triggered()), this, SLOT(connectItemShowAction()));
  connect(connectMenu->clean, SIGNAL(triggered()), this, SLOT(deleteCurrentConnect()));
  connectMenu->move(mapToGlobal(pos));
  connectMenu->exec();
  if (to_run) disconnect(connectMenu->act, SIGNAL(triggered()), this, SLOT(connectItemRunAction()));
  else disconnect(connectMenu->act, SIGNAL(triggered()), this, SLOT(connectItemKillAction()));
  disconnect(connectMenu->edit, SIGNAL(triggered()), this, SLOT(connectItemEditAction()));
  disconnect(connectMenu->display, SIGNAL(triggered()), this, SLOT(connectItemShowAction()));
  disconnect(connectMenu->clean, SIGNAL(triggered()), this, SLOT(deleteCurrentConnect()));
  connectMenu->deleteLater();
}
void ConnectList::connectItemDoubleClicked(const QModelIndex &_item)
{
  ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
  QString _name = idx->getName();
  DATA conn_Status;
  conn_Status = idx->getData();
  QString key = conn_Status.value(QString("initName")).toString();
  ElemConnect *conn;
  conn = connects->value(key);
  if ( key != _name ) {
      conn_Status.insert(QString("initName"), QVariant(_name));
      connects->insert(_name, conn);
      connects->remove(key);
      conn->setItemReference(connItemModel, idx);
  };
  //qDebug()<<key<<" Connect doubleClicked"<<conn;
  int conn_state;
  conn_state = conn_Status.value(QString("isRunning"), STOPPED).toInt();
  if ( !conn_Status.value(QString("availability"), NOT_AVAILABLE).toBool() ) {
      showMessage("Info", "Connect is busy.");
  } else if ( conn_state!=RUNNING ) conn->openConnect();
  else if ( conn_state==RUNNING ) {
      emit connectClosed(conn->getConnect());
      // TODO: check here for close related instances :
      // domains, networks, storages, etc.
      conn->closeConnect();
  };
  clearSelection();
}
void ConnectList::connectItemKillAction()
{
  QModelIndex _item = currentIndex();
  checkConnect(_item, TO_STOP);
}
void ConnectList::connectItemRunAction()
{
    QModelIndex _item = currentIndex();
    checkConnect(_item, TO_RUN);
}
void ConnectList::connectItemShowAction()
{
    QModelIndex _item = currentIndex();
    showConnect(_item);
}
void ConnectList::createConnect(QModelIndex &_item)
{
  ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
  QString key = idx->getName();
  ElemConnect *conn = new ElemConnect(this);
  conn->setItemReference(connItemModel, idx);
  connects->insert(key, conn);
  clearSelection();
  connect(connects->value(key), SIGNAL(warningShowed()), this, SLOT(mainWindowUp()));
  connect(connects->value(key), SIGNAL(warning(QString&)), this, SLOT(sendWarning(QString&)));
  connect(connects->value(key), SIGNAL(connPtr(virConnect*, QString&)), this, SLOT(sendConnPtr(virConnect*, QString&)));
  connect(connects->value(key), SIGNAL(authRequested(QString&)), this, SLOT(getAuthCredentials(QString&)));
  //qDebug()<<key<<" create Connect item";
}
void ConnectList::checkConnect(QModelIndex &_item, bool to_run = TO_RUN)
{
  int conn_state;
  ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
  conn_state = idx->getData().value(QString("isRunning"), STOPPED).toInt();
  if ( (to_run && conn_state!=RUNNING) || (!to_run && conn_state==RUNNING) )
    connectItemDoubleClicked(_item);
}
void ConnectList::deleteCancelledCreation()
{
  deleteCurrentConnect();
}
void ConnectList::showMessage(QString title, QString msg)
{
  QMessageBox::information(this, title, msg);
  mainWindowUp();
}
void ConnectList::sendWarning(QString &msg)
{
    emit warning(msg);
}
void ConnectList::mainWindowUp()
{
    emit messageShowed();
}
void ConnectList::sendConnPtr(virConnect *conn, QString &name)
{
    emit connPtr(conn, name);
}
void ConnectList::getAuthCredentials(QString &crd)
{
    ElemConnect *obj = static_cast<ElemConnect*>(sender());
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
