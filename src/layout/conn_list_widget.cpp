#include "layout/conn_list_widget.h"

ConnectionList::ConnectionList(QWidget *parent)
    : QTreeView(parent)
{
    this->setEnabled(false);
    this->setContextMenuPolicy ( Qt::CustomContextMenu );
    setContentsMargins (1, 1, 1, 1);
    setDefaultDropAction ( Qt::IgnoreAction );
    setCursor(Qt::ArrowCursor);
    setSortingEnabled(true);
    this->setItemsExpandable(false);
    this->setRootIsDecorated(false);
    connItemModel = new ConnItemModel(this);
    this->setModel(connItemModel);
    connListDlg = new ConnListDelegate();
    this->setItemDelegate(connListDlg);
    connections = new CONN_LIST();
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(connItemClicked(const QPoint &)));
    connect(this, SIGNAL(doubleClicked(const QModelIndex&)),
            this, SLOT(connItemDoubleClicked(const QModelIndex&)));
    searchThread = new SearchThread(this);
    waitLocalConn = new WaitLocalConn(this, connections, connItemModel);
    connect(searchThread, SIGNAL(localConnFound(QString&)),
            this, SLOT(createLocalConnection(QString&)));
    connect(waitLocalConn, SIGNAL(uriToCompare(QString&)),
            searchThread, SLOT(compareURI(QString&)));
    connect(waitLocalConn, SIGNAL(finished()),
            this, SLOT(searchLocalhostConnections()));
    connect(searchThread, SIGNAL(errorMsg(QString&,uint)),
            this, SLOT(sendWarning(QString&,uint)));
    connect(searchThread, SIGNAL(finished()),
            this, SLOT(searchLocalhostConnComplete()));
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
    ConnSettings *sDialog = new ConnSettings(this->parentWidget());
    sDialog->setConnectItem(idx);
    connect(sDialog, SIGNAL(creationConnCancelled()),
            this, SLOT(deleteCancelledCreation()));
    exitCode = sDialog->exec();
    disconnect(sDialog, SIGNAL(creationConnCancelled()),
               this, SLOT(deleteCancelledCreation()));
    sDialog->deleteLater();
    if ( exitCode==QDialog::Accepted && !idx->getData().value("isRunning").toBool() ) {
        QString key = idx->getName();
        ConnElement *conn = static_cast<ConnElement*>(
                    connections->value(key));
        if ( nullptr!=conn ) {
            conn->buildURI();
        };
    };
    return exitCode;
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
        if ( _createConnect==QDialog::Accepted )
            createConnection(item);
    };
}
void ConnectionList::deleteCurrentConnection()
{
    QModelIndex _item = currentIndex();
    deleteCurrentConnection(_item);
}
void ConnectionList::openConnection(QModelIndex &_item)
{
    checkConnection(_item, TO_RUN);
}
void ConnectionList::showConnection(QModelIndex &_item)
{
    if ( !_item.isValid() ) return;
    for (int i=0; i<connections->count(); i++) {
        int conn_state;
        bool conn_availability;
        ConnItemIndex *idx = connItemModel->connItemDataList.at(i);
        if ( idx==nullptr ) continue;
        QString _name = idx->getName();
        ConnElement *conn = static_cast<ConnElement*>(
                    connections->value(_name));
        conn->setOnViewConnAliveThread(i==_item.row());
        //qDebug()<<_name<<(i==_item.row());
        if ( i==_item.row() ) {
            conn_state = idx->getData()
                    .value(
                        QString("isRunning"),
                        CLOSED)
                    .toInt();
            conn_availability = idx->getData()
                    .value(
                        QString("availability"),
                        NOT_AVAILABLE)
                    .toBool();
            if ( conn_state==RUNNING && conn_availability ) {
                conn->overviewConnection();
            };
        } else
            connItemModel->setData(
                        connItemModel->index(i, 0),
                        false,
                        Qt::DecorationRole);
    };
    clearSelection();
}
void ConnectionList::closeConnection(QModelIndex &_item)
{
    checkConnection(_item, TO_STOP);
}
virConnectPtr* ConnectionList::getPtr_connectionPtr(QString &name)
{
    //qDebug()<<name<<connections->contains(name);
    virConnectPtr *_res = nullptr;
    if ( connections->contains(name) ) {
            ConnElement *conn = static_cast<ConnElement*>(
                        connections->value(name));
            if ( nullptr!=conn )
                _res = conn->getPtr_connectionPtr();
    };
    return _res;
}
void ConnectionList::stopProcessing()
{
    clearSelection();
    for (int i=0; i<connections->count(); i++) {
        ConnItemIndex *idx = connItemModel->connItemDataList.at(i);
        if ( idx==nullptr ) continue;
        QString _name = idx->getName();
        ConnElement *conn = static_cast<ConnElement*>(
                    connections->value(_name));
        if ( nullptr==conn ) continue;
        conn->setOnViewConnAliveThread(false);
        connItemModel->setData(
                    connItemModel->index(i, 0),
                    false,
                    Qt::DecorationRole);
    };
}

/* private slots */
void ConnectionList::searchLocalhostConnections()
{
    this->setEnabled(false);
    clearSelection();
    if ( !searchThread->isRunning() ) searchThread->start();
}
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
    if ( conn_Status.value("isRunning", CLOSED).toInt()==RUNNING ) {
        connectMenu->act->setText("Close Connection");
        connectMenu->act->setIcon(QIcon::fromTheme("disconnect"));
        connect(connectMenu->act, SIGNAL(triggered()),
                this, SLOT(connItemKillAction()));
        //connectMenu->clean->setEnabled(true);
        connectMenu->display->setEnabled(true);
        to_run = TO_STOP;
    } else {
        connectMenu->act->setText("Open Connection");
        connectMenu->act->setIcon(QIcon::fromTheme("connect"));
        connect(connectMenu->act, SIGNAL(triggered()),
                this, SLOT(connItemRunAction()));
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
    if (to_run)
        disconnect(connectMenu->act, SIGNAL(triggered()),
                   this, SLOT(connItemRunAction()));
    else
        disconnect(connectMenu->act, SIGNAL(triggered()),
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
    clearSelection();
    ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
    QString _name = idx->getName();
    DATA conn_Status;
    conn_Status = idx->getData();
    QString key = conn_Status.value(QString("initName")).toString();
    ConnElement *conn = static_cast<ConnElement*>(connections->value(key));
    if ( nullptr==conn ) return;
    if ( key != _name ) {
        conn_Status.insert(QString("initName"), QVariant(_name));
        connections->insert(_name, conn);
        connections->remove(key);
        conn->setItemReference(connItemModel, idx);
    };
    //qDebug()<<key<<" Connection doubleClicked"<<conn;
    int conn_state;
    conn_state = conn_Status.value(QString("isRunning"), FAILED).toInt();
    if ( !conn_Status.value(QString("availability"), NOT_AVAILABLE).toBool() ) {
        showMessage("Info", "Connection is busy.");
    } else if ( conn_state!=RUNNING ) {
        conn->openConnection();
    } else if ( conn_state==RUNNING ) {
        emit connToClose(_item.row());
        conn->closeConnection();
    };
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
    if ( !_item.isValid() ) return;
    ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
    QString key = idx->getName();
    connections->insert(key, new ConnElement(this));
    ConnElement *conn = static_cast<ConnElement*>(
                connections->value(key));
    conn->setItemReference(connItemModel, idx);
    clearSelection();
    connect(conn, SIGNAL(warningShowed()),
            this, SLOT(mainWindowUp()));
    connect(conn, SIGNAL(warning(QString&)),
            this, SLOT(sendWarning(QString&)));
    connect(conn, SIGNAL(connPtrPtr(virConnectPtr*, QString&)),
            this, SLOT(sendConnPtrPtr(virConnectPtr*, QString&)));
    connect(conn, SIGNAL(authRequested(QString&)),
            this, SLOT(getAuthCredentials(QString&)));
    connect(conn, SIGNAL(domStateChanged(Result)),
            this, SIGNAL(domResult(Result)));
    connect(conn, SIGNAL(netStateChanged(Result)),
            this, SIGNAL(netResult(Result)));
    connect(conn, SIGNAL(connClosed(bool, QString&)),
            this, SIGNAL(connClosed(bool, QString&)));
    connect(conn, SIGNAL(domainEnd(QString&)),
            this, SIGNAL(domainEnd(QString&)));
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
    connections->insert(key, new ConnElement(this));
    ConnElement *conn = static_cast<ConnElement*>(
                connections->value(key));
    if ( nullptr==conn ) return;
    conn->setName(s);
    conn->setURI(uri);
    conn->setItemReferenceForLocal(connItemModel, idx);
    clearSelection();
    connect(conn, SIGNAL(warningShowed()),
            this, SLOT(mainWindowUp()));
    connect(conn, SIGNAL(warning(QString&)),
            this, SLOT(sendWarning(QString&)));
    connect(conn, SIGNAL(connPtrPtr(virConnectPtr*, QString&)),
            this, SLOT(sendConnPtrPtr(virConnectPtr*, QString&)));
    connect(conn, SIGNAL(authRequested(QString&)),
            this, SLOT(getAuthCredentials(QString&)));
    connect(conn, SIGNAL(domStateChanged(Result)),
            this, SIGNAL(domResult(Result)));
    connect(conn, SIGNAL(netStateChanged(Result)),
            this, SIGNAL(netResult(Result)));
    connect(conn, SIGNAL(connClosed(bool, QString&)),
            this, SIGNAL(connClosed(bool, QString&)));
    connect(conn, SIGNAL(domainEnd(QString&)),
            this, SIGNAL(domainEnd(QString&)));
    //qDebug()<<key<<" create Local Connection item";
}
void ConnectionList::refreshLocalhostConnection()
{
    this->setEnabled(false);
    clearSelection();
    searchThread->setURIList();
    localConn = 0;
    if ( !waitLocalConn->isRunning() ) waitLocalConn->start();
}
void ConnectionList::checkConnection(QModelIndex &_item, bool to_run = TO_RUN)
{
    if ( !_item.isValid() ) return;
    int conn_state;
    ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
    conn_state = idx->getData().value(QString("isRunning"), CLOSED).toInt();
    if ( (to_run && conn_state!=RUNNING) || (!to_run && conn_state==RUNNING) )
        connItemDoubleClicked(_item);
}
void ConnectionList::deleteCancelledCreation()
{
    deleteCurrentConnection();
}
void ConnectionList::deleteCurrentConnection(QModelIndex &_item)
{
    if ( _item.isValid() ) {
        ConnItemIndex *idx =
                connItemModel->connItemDataList.at(_item.row());
        QString connection = idx->getName();
        ConnElement *conn = static_cast<ConnElement*>(
                    connections->value(connection));
        bool conn_availability;
        conn_availability = idx->getData()
                .value(
                    QString("availability"),
                    AVAILABLE)
                .toBool();
        if ( !conn_availability ) {
            showMessage(
                        QString("Connection '%1'").arg(connection),
                        "Connection is Busy.");
            clearSelection();
            return;
        };
        int conn_state = idx->getData()
                .value(
                    QString("isRunning"),
                    CLOSED)
                .toInt();
        if ( conn && conn_state==RUNNING ) {
            showMessage(
                        QString("Connection '%1'").arg(connection),
                        "Connection is Running.");
        } else {
            if ( conn ) {
                disconnect(conn, SIGNAL(warningShowed()),
                           this, SLOT(mainWindowUp()));
                disconnect(conn, SIGNAL(warning(QString&)),
                           this, SLOT(sendWarning(QString&)));
                disconnect(conn, SIGNAL(connPtrPtr(virConnectPtr*, QString&)),
                           this, SLOT(sendConnPtrPtr(virConnectPtr*, QString&)));
                disconnect(conn, SIGNAL(authRequested(QString&)),
                           this, SLOT(getAuthCredentials(QString&)));
                disconnect(conn, SIGNAL(domStateChanged(Result)),
                           this, SIGNAL(domResult(Result)));
                disconnect(conn, SIGNAL(netStateChanged(Result)),
                           this, SIGNAL(netResult(Result)));
                disconnect(conn, SIGNAL(connClosed(bool, QString&)),
                           this, SIGNAL(connClosed(bool, QString&)));
                disconnect(conn, SIGNAL(domainEnd(QString&)),
                           this, SIGNAL(domainEnd(QString&)));
                emit removeConnection(connection);
                connections->remove(connection);
            };
            connItemModel->removeRow(_item.row());
        };
    } else showMessage("Info", "Item not exist.");
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
void ConnectionList::sendWarning(QString &msg, uint _num)
{
    Q_UNUSED(_num);
    QString title("Search local connections");
    QString time = QTime::currentTime().toString();
    QString errorMsg = QString("<b>%1 %2:</b><br><font color='red'><b>ERROR</b></font>: %3")
            .arg(time).arg(title).arg(msg);
    emit warning(errorMsg);
}
void ConnectionList::mainWindowUp()
{
    emit messageShowed();
}
void ConnectionList::sendConnPtrPtr(virConnectPtr *_connPtrPtr, QString &name)
{
    //qDebug()<<"sendConnPtrPtr"<<(*_connPtrPtr);
    emit connPtrPtr(_connPtrPtr, name);
}
void ConnectionList::getAuthCredentials(QString &crd)
{
    ConnElement *obj = static_cast<ConnElement*>(sender());
    if ( nullptr==obj ) return;
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
void ConnectionList::searchLocalhostConnComplete()
{
    this->setEnabled(true);
}
