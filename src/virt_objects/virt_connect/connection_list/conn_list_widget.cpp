#include "conn_list_widget.h"

ConnectionList::ConnectionList(QWidget *parent)
    : TreeView(parent)
{
    this->setEnabled(false);
    this->setContextMenuPolicy ( Qt::CustomContextMenu );
    this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
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
            this, SLOT(connContextMenuRequested(const QPoint &)));
    connect(this, SIGNAL(doubleClicked(const QModelIndex&)),
            this, SLOT(connItemDoubleClicked(const QModelIndex&)));
    searchThread = new SearchThread(this);
    waitLocalConn = new WaitLocalConn(this, connections, connItemModel);
    connect(searchThread, SIGNAL(localConnFound(const QString&)),
            this, SLOT(createLocalConnection(const QString&)));
    connect(waitLocalConn, SIGNAL(uriToCompare(const QString&)),
            searchThread, SLOT(compareURI(const QString&)));
    connect(waitLocalConn, SIGNAL(finished()),
            this, SLOT(searchLocalhostConnections()));
    connect(searchThread, SIGNAL(errorMsg(const QString&, const uint)),
            this, SLOT(sendWarning(const QString&, const uint)));
    connect(searchThread, SIGNAL(finished()),
            this, SLOT(searchLocalhostConnComplete()));
}

void ConnectionList::setUsageInSoftTouched(bool state)
{
    prevL->setUsageStatus(state);
    homeL->setUsageStatus(state);
    nextL->setUsageStatus(state);
}
int ConnectionList::getListItemCount() const
{
    return connItemModel->rowCount();
}
QModelIndex ConnectionList::getCustomIndex(int i) const
{
    return connItemModel->index(i, 0);
}
ConnItemIndex* ConnectionList::getConnItemDataListIndex(int i) const
{
    return connItemModel->connItemDataList.at(i);
}
int ConnectionList::getConnItemDataListCount() const
{
    return connItemModel->connItemDataList.count();
}
int ConnectionList::getConnItemDataListIndexOf(ConnItemIndex* idx) const
{
    return connItemModel->connItemDataList.indexOf(idx);
}
bool ConnectionList::removeListItem(int row) const
{
    return connItemModel->removeRow(row);
}
bool ConnectionList::removeConnItemDataList(ConnItemIndex *idx) const
{
    return connItemModel->connItemDataList.removeOne(idx);
}
ConnElement* ConnectionList::getConnElementByName(QString name) const
{
    return connections->value(name);
}
int ConnectionList::removeConnectionItembyName(QString name) const
{
    return connections->remove(name);
}
void ConnectionList::addConnItem(const QString &s)
{
    const int count = connItemModel->rowCount();
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
void ConnectionList::openConnection(const QModelIndex &_item)
{
    checkConnection(_item, TO_RUN);
}
void ConnectionList::overviewOfConnection(const QModelIndex &_item)
{
    if ( !_item.isValid() ) return;
    QList<ConnItemIndex*>::const_iterator i;
    for (i=connItemModel->connItemDataList.constBegin();
         i!=connItemModel->connItemDataList.constEnd();
         ++i) {
        int conn_state;
        bool conn_availability;
        ConnItemIndex *idx = (*i);
        if ( idx==nullptr ) continue;
        const QString _name = idx->getName();
        ConnElement *conn = static_cast<ConnElement*>(
                    connections->value(_name));
        const int row = getConnItemDataListIndexOf(idx);
        //qDebug()<<_name<<(row==_item.row());
        if ( row==_item.row() ) {
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
                conn->overviewOfConnection();
            };
        } else {
            conn->disableOverviewOfConnection();
        };
    };
    clearSelection();
}
void ConnectionList::closeConnection(const QModelIndex &_item)
{
    checkConnection(_item, TO_STOP);
}
virConnectPtr* ConnectionList::getPtr_connectionPtr(const QString &name)
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

/* public slots */
void ConnectionList::refreshLocalhostConnection()
{
    emit searchStarted();
    this->setEnabled(false);
    clearSelection();
    searchThread->setURIList();
    localConn = 0;
    if ( !waitLocalConn->isRunning() ) waitLocalConn->start();
}
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
    if ( exitCode==QDialog::Accepted
         && !idx->getData().value("isRunning").toBool() ) {
        const QString key = idx->getName();
        ConnElement *conn = static_cast<ConnElement*>(
                    connections->value(key));
        if ( nullptr!=conn ) {
            conn->buildURI();
        };
    };
    return exitCode;
}
void ConnectionList::deleteCurrentConnection()
{
    QModelIndex _item = currentIndex();
    deleteCurrentConnection(_item);
}
void ConnectionList::stopProcessing()
{
    clearSelection();
    QList<ConnItemIndex*>::const_iterator i;
    for (i=connItemModel->connItemDataList.constBegin();
         i!=connItemModel->connItemDataList.constEnd();
         ++i) {
        ConnItemIndex *idx = (*i);
        if ( idx==nullptr ) continue;
        const QString _name = idx->getName();
        ConnElement *conn = static_cast<ConnElement*>(
                    connections->value(_name));
        if ( nullptr==conn ) continue;
        conn->disableOverviewOfConnection();
    };
}

/* private */
void ConnectionList::createConnection(const QModelIndex &_item)
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
    connect(conn, SIGNAL(warning(const QString&)),
            this, SLOT(sendWarning(const QString&)));
    connect(conn, SIGNAL(connToOverview(virConnectPtr*, const QString&)),
            this, SIGNAL(connToOverview(virConnectPtr*, const QString&)));
    connect(conn, SIGNAL(authRequested(const QString&)),
            this, SLOT(getAuthCredentials(const QString&)));
    connect(conn, SIGNAL(domStateChanged(Result)),
            this, SIGNAL(domResult(Result)));
    connect(conn, SIGNAL(netStateChanged(Result)),
            this, SIGNAL(netResult(Result)));
    connect(conn, SIGNAL(connClosed(bool, const QString&)),
            this, SIGNAL(connClosed(bool, const QString&)));
    connect(conn, SIGNAL(domainEnd(const QString&)),
            this, SIGNAL(domainEnd(const QString&)));
    connect(conn, SIGNAL(newOpenedConnection(const QString&)),
            this, SLOT(setOnViewAvailableConnection(const QString&)));
    //qDebug()<<key<<" create Connection item";
    if ( !searchThread->isFinished() && !searchThread->isRunning() ) {
        QString uri(conn->getURI());
        searchThread->compareURI(uri);
    };
}
void ConnectionList::checkConnection(const QModelIndex &_item, const bool to_run = TO_RUN)
{
    if ( !_item.isValid() ) return;
    int conn_state;
    ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
    conn_state = idx->getData().value(QString("isRunning"), CLOSED).toInt();
    if ( (to_run && conn_state!=RUNNING) || (!to_run && conn_state==RUNNING) )
        connItemDoubleClicked(_item);
}
void ConnectionList::deleteCurrentConnection(const QModelIndex &_item)
{
    if ( _item.isValid() ) {
        ConnItemIndex *idx =
                connItemModel->connItemDataList.at(_item.row());
        const QString connection = idx->getName();
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
                disconnect(conn, SIGNAL(warning(const QString&)),
                           this, SLOT(sendWarning(const QString&)));
                disconnect(conn, SIGNAL(connToOverview(virConnectPtr*, const QString&)),
                           this, SIGNAL(connToOverview(virConnectPtr*, const QString&)));
                disconnect(conn, SIGNAL(authRequested(const QString&)),
                           this, SLOT(getAuthCredentials(const QString&)));
                disconnect(conn, SIGNAL(domStateChanged(Result)),
                           this, SIGNAL(domResult(Result)));
                disconnect(conn, SIGNAL(netStateChanged(Result)),
                           this, SIGNAL(netResult(Result)));
                disconnect(conn, SIGNAL(connClosed(bool, const QString&)),
                           this, SIGNAL(connClosed(bool, const QString&)));
                disconnect(conn, SIGNAL(domainEnd(const QString&)),
                           this, SIGNAL(domainEnd(const QString&)));
                disconnect(conn, SIGNAL(newOpenedConnection(const QString&)),
                           this, SLOT(setOnViewAvailableConnection(const QString&)));
                emit removeConnection(connection);
                connections->remove(connection);
                delete conn;
                conn = nullptr;
            };
            connItemModel->removeRow(_item.row());
        };
    } else showMessage("Info", "Item not exist.");
}
void ConnectionList::showMessage(const QString &title, const QString &msg)
{
    QMessageBox::information(this, title, msg);
    mainWindowUp();
}
bool ConnectionList::onViewExist() const
{
    bool res = false;
    QList<ConnItemIndex*>::const_iterator i;
    for (i=connItemModel->connItemDataList.constBegin();
         i!=connItemModel->connItemDataList.constEnd();
         ++i) {
        ConnItemIndex *idx = (*i);
        if ( idx==nullptr ) continue;
        const QString _name = idx->getName();
        ConnElement *conn = static_cast<ConnElement*>(
                    connections->value(_name));
        if ( nullptr==conn ) continue;
        if ( conn->getOnViewState() ) {
            //qDebug()<<_name<<"onView";
            res = true;
            break;
        };
    };
    return res;
}

/* private slots */
void ConnectionList::searchLocalhostConnections()
{
    this->setEnabled(false);
    clearSelection();
    if ( !searchThread->isRunning() ) searchThread->start();
}
void ConnectionList::connContextMenuRequested(const QPoint &pos)
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
    if ( !conn_Status.value("availability", NOT_AVAILABLE).toBool() )
        return;
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
    //clearSelection();
    ConnItemIndex *idx = connItemModel->connItemDataList.at(_item.row());
    QString _name = idx->getName();
    DATA conn_Status;
    conn_Status = idx->getData();
    QString key = conn_Status.value(QString("initName")).toString();
    ConnElement *conn =
            static_cast<ConnElement*>(connections->value(key));
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
    if ( !conn_Status.value(
             QString("availability"), NOT_AVAILABLE).toBool() ) {
        showMessage("Info", "Connection is busy.");
    } else if ( conn_state!=RUNNING ) {
        conn->openConnection();
    } else if ( conn_state==RUNNING ) {
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
    overviewOfConnection(_item);
}
void ConnectionList::createLocalConnection(const QString &uri)
{
    //qDebug()<<uri<<"create connection";
    ++localConn;
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
    if ( nullptr==conn ) {
        --localConn;
        return;
    };
    clearSelection();
    connect(conn, SIGNAL(warningShowed()),
            this, SLOT(mainWindowUp()));
    connect(conn, SIGNAL(warning(const QString&)),
            this, SLOT(sendWarning(const QString&)));
    connect(conn, SIGNAL(connToOverview(virConnectPtr*, const QString&)),
            this, SIGNAL(connToOverview(virConnectPtr*, const QString&)));
    connect(conn, SIGNAL(authRequested(const QString&)),
            this, SLOT(getAuthCredentials(const QString&)));
    connect(conn, SIGNAL(domStateChanged(Result)),
            this, SIGNAL(domResult(Result)));
    connect(conn, SIGNAL(netStateChanged(Result)),
            this, SIGNAL(netResult(Result)));
    connect(conn, SIGNAL(connClosed(bool, const QString&)),
            this, SIGNAL(connClosed(bool, const QString&)));
    connect(conn, SIGNAL(domainEnd(const QString&)),
            this, SIGNAL(domainEnd(const QString&)));
    connect(conn, SIGNAL(newOpenedConnection(const QString&)),
            this, SLOT(setOnViewAvailableConnection(const QString&)));
    conn->setName(s);
    conn->setURI(uri);
    conn->setItemReferenceForLocal(connItemModel, idx);
    //qDebug()<<key<<" create Local Connection item";
}
void ConnectionList::deleteCancelledCreation()
{
    deleteCurrentConnection();
}
void ConnectionList::sendWarning(const QString &msg)
{
    emit warning(msg);
}
void ConnectionList::sendWarning(const QString &msg, const uint _num)
{
    Q_UNUSED(_num);
    QString title("Search local connections");
    QString time = QTime::currentTime().toString();
    QString errorMsg = QString(
    "<b>%1 %2:</b><br><font color='red'><b>ERROR</b></font>: %3")
            .arg(time).arg(title).arg(msg);
    emit warning(errorMsg);
}
void ConnectionList::mainWindowUp()
{
    emit messageShowed();
}
void ConnectionList::getAuthCredentials(const QString &crd)
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
    emit searchFinished();
}
void ConnectionList::setOnViewAvailableConnection(const QString &_newName)
{
    if ( searchThread->isRunning() ) return;
    if ( onViewExist() || connections->count()==0 ) return;
    QList<ConnItemIndex*>::const_iterator i;
    for (i=connItemModel->connItemDataList.constBegin();
         i!=connItemModel->connItemDataList.constEnd();
         ++i) {
        ConnItemIndex *idx = (*i);
        if ( idx==nullptr ) continue;
        const QString _name = idx->getName();
        if ( _newName!=_name ) continue;
        ConnElement *conn = static_cast<ConnElement*>(
                    connections->value(_name));
        if ( nullptr==conn ) continue;
        conn->overviewOfConnection();
        break;
    };
}
