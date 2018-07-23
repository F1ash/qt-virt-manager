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
    onViewExist = false;
    connections = new CONN_LIST();
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(connContextMenuRequested(const QPoint &)));
    connect(this, SIGNAL(clicked(const QModelIndex&)),
            this, SLOT(connItemClicked(const QModelIndex&)));
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
    if ( 0<=i && i<getConnItemDataListCount() ) {
        return connItemModel->connItemDataList.at(i);
    } else {
        return Q_NULLPTR;
    };
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
    return connections->value(name, Q_NULLPTR);
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
        ConnItemIndex *idx = getConnItemDataListIndex(i);
        if ( idx!=Q_NULLPTR && idx->getName().compare(s)==0 ) {
            exist = true;
            break;
        }
    };
    if ( !exist ) {
        connItemModel->insertRow(0);
        QModelIndex _item;
        for (int j=0; j<connItemModel->columnCount(); j++) {
            QString data;
            if (j) {
                data = "-";
            } else data = s;
            _item = connItemModel->index(0,j);
            connItemModel->setData(_item, data, Qt::EditRole);
        };
        int _createConnection = QDialog::Accepted;
        if ( s.compare("<noname>")==0 && _item.isValid() ) {
            setCurrentIndex(_item);
            _createConnection = connItemEditAction();
        };
        // check item if it exist and correct
        if ( _createConnection==QDialog::Accepted )
            createConnection(_item);
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
        if ( idx==Q_NULLPTR ) continue;
        const QString _name = idx->getName();
        ConnElement *conn = static_cast<ConnElement*>(
                    connections->value(_name));
        if ( conn==Q_NULLPTR ) continue;
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
                // set flag to TRUE
                onViewExist = true;
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
    virConnectPtr *_res = Q_NULLPTR;
    if ( connections->contains(name) ) {
            ConnElement *conn = static_cast<ConnElement*>(
                        connections->value(name));
            if ( Q_NULLPTR!=conn )
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
    int exitCode = QDialog::Rejected;
    QModelIndex _item = currentIndex();
    if ( !_item.isValid() ) {
        showMessage(tr("Info"), tr("Item not exist."));
        return exitCode;
    };
    ConnItemIndex *idx = getConnItemDataListIndex(_item.row());
    if ( idx==Q_NULLPTR ) return exitCode;
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
        if ( Q_NULLPTR!=conn ) {
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
        if ( idx==Q_NULLPTR ) continue;
        const QString _name = idx->getName();
        ConnElement *conn = static_cast<ConnElement*>(
                    connections->value(_name));
        if ( Q_NULLPTR==conn ) continue;
        conn->disableOverviewOfConnection();
    };
    // set flag to FALSE
    onViewExist = false;
}

/* private */
void ConnectionList::createConnection(const QModelIndex &_item)
{
    if ( !_item.isValid() ) return;
    ConnItemIndex *idx = getConnItemDataListIndex(_item.row());
    if ( idx==Q_NULLPTR ) return;
    QString key = idx->getName();
    connections->insert(key, new ConnElement(this));
    ConnElement *conn = static_cast<ConnElement*>(
                connections->value(key));
    if ( conn==Q_NULLPTR ) return;
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
    connect(conn, SIGNAL(domStateChanged()),
            this, SIGNAL(domStateChanged()));
    connect(conn, SIGNAL(netStateChanged()),
            this, SIGNAL(netStateChanged()));
    connect(conn, SIGNAL(poolStateChanged()),
            this, SIGNAL(poolStateChanged()));
    connect(conn, SIGNAL(secStateChanged()),
            this, SIGNAL(secStateChanged()));
    connect(conn, SIGNAL(connClosed(bool, const QString&)),
            this, SIGNAL(connClosed(bool, const QString&)));
    connect(conn, SIGNAL(connClosed(bool, const QString&)),
            this, SLOT(newItemClosed(bool)));
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
    ConnItemIndex *idx = getConnItemDataListIndex(_item.row());
    if ( idx==Q_NULLPTR ) return;
    conn_state = idx->getData().value(QString("isRunning"), CLOSED).toInt();
    if ( (to_run && conn_state!=RUNNING) || (!to_run && conn_state==RUNNING) )
        connItemDoubleClicked(_item);
}
void ConnectionList::deleteCurrentConnection(const QModelIndex &_item)
{
    if ( _item.isValid() ) {
        ConnItemIndex *idx = getConnItemDataListIndex(_item.row());
        if ( idx==Q_NULLPTR ) return;
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
                        QString(tr("Connection '%1'")).arg(connection),
                        tr("Connection is Busy."));
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
                        QString(tr("Connection '%1'")).arg(connection),
                        tr("Connection is Running."));
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
                disconnect(conn, SIGNAL(domStateChanged()),
                           this, SIGNAL(domStateChanged()));
                disconnect(conn, SIGNAL(netStateChanged()),
                           this, SIGNAL(netStateChanged()));
                disconnect(conn, SIGNAL(poolStateChanged()),
                           this, SIGNAL(poolStateChanged()));
                disconnect(conn, SIGNAL(secStateChanged()),
                           this, SIGNAL(secStateChanged()));
                disconnect(conn, SIGNAL(connClosed(bool, const QString&)),
                           this, SIGNAL(connClosed(bool, const QString&)));
                disconnect(conn, SIGNAL(connClosed(bool, const QString&)),
                           this, SLOT(newItemClosed(bool)));
                disconnect(conn, SIGNAL(domainEnd(const QString&)),
                           this, SIGNAL(domainEnd(const QString&)));
                disconnect(conn, SIGNAL(newOpenedConnection(const QString&)),
                           this, SLOT(setOnViewAvailableConnection(const QString&)));
                emit removeConnection(connection);
                connections->remove(connection);
                delete conn;
                conn = Q_NULLPTR;
            };
            connItemModel->removeRow(_item.row());
        };
    } else showMessage(tr("Info"), tr("Item not exist."));
}
void ConnectionList::showMessage(const QString &title, const QString &msg)
{
    QMessageBox::information(this, title, msg);
    mainWindowUp();
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
    ConnItemIndex *idx = getConnItemDataListIndex(_item.row());
    if ( idx==Q_NULLPTR ) return;
    conn_Status = idx->getData();
    if ( !conn_Status.value("availability", NOT_AVAILABLE).toBool() )
        return;
    bool to_run = TO_RUN;
    ConnectionMenu *connMenu = new ConnectionMenu(this);
    if ( conn_Status.value("isRunning", CLOSED).toInt()==RUNNING ) {
        connMenu->act->setText(tr("Close Connection"));
        connMenu->act->setIcon(QIcon::fromTheme("disconnect"));
        connect(connMenu->act, SIGNAL(triggered()),
                this, SLOT(connItemKillAction()));
        //connMenu->clean->setEnabled(true);
        connMenu->display->setEnabled(true);
        to_run = TO_STOP;
    } else {
        connMenu->act->setText(tr("Open Connection"));
        connMenu->act->setIcon(QIcon::fromTheme("connect"));
        connect(connMenu->act, SIGNAL(triggered()),
                this, SLOT(connItemRunAction()));
        //connMenu->clean->setEnabled(false);
        connMenu->display->setEnabled(false);
        to_run = TO_RUN;
    };
    idx->setData(conn_Status);
    connect(connMenu->edit, SIGNAL(triggered()),
            this, SLOT(connItemEditAction()));
    connect(connMenu->display, SIGNAL(triggered()),
            this, SLOT(connItemShowAction()));
    connect(connMenu->clean, SIGNAL(triggered()),
            this, SLOT(deleteCurrentConnection()));
    connect(connMenu->refresh, SIGNAL(triggered()),
            this, SLOT(refreshLocalhostConnection()));
    connMenu->move(mapToGlobal(pos));
    connMenu->exec();
    if (to_run)
        disconnect(connMenu->act, SIGNAL(triggered()),
                   this, SLOT(connItemRunAction()));
    else
        disconnect(connMenu->act, SIGNAL(triggered()),
                   this, SLOT(connItemKillAction()));
    disconnect(connMenu->edit, SIGNAL(triggered()),
               this, SLOT(connItemEditAction()));
    disconnect(connMenu->display, SIGNAL(triggered()),
               this, SLOT(connItemShowAction()));
    disconnect(connMenu->clean, SIGNAL(triggered()),
               this, SLOT(deleteCurrentConnection()));
    disconnect(connMenu->refresh, SIGNAL(triggered()),
               this, SLOT(refreshLocalhostConnection()));
    connMenu->deleteLater();
}
void ConnectionList::connItemClicked(const QModelIndex &_item)
{
    //if ( !_item.isValid() ) return;
    ConnItemIndex *idx = getConnItemDataListIndex(_item.row());
    if ( idx==Q_NULLPTR ) return;
    QString _name = idx->getName();
    DATA conn_Status = idx->getData();
    QString key = conn_Status.value(QString("initName")).toString();
    ConnElement *conn = static_cast<ConnElement*>(
                connections->value(key));
    if ( Q_NULLPTR==conn ) return;
    if ( key.compare(_name)!=0 ) {
        conn_Status.insert(QString("initName"), QVariant(_name));
        connections->insert(_name, conn);
        connections->remove(key);
        conn->setItemReference(connItemModel, idx);
    };
    int conn_state = conn_Status.value(QString("isRunning"), FAILED).toInt();
    if ( conn_Status.value(
             QString("availability"), NOT_AVAILABLE).toBool()
         && conn_state==RUNNING ) {
        overviewOfConnection(_item);
    };
}
void ConnectionList::connItemDoubleClicked(const QModelIndex &_item)
{
    if ( !_item.isValid() ) return;
    //clearSelection();
    ConnItemIndex *idx = getConnItemDataListIndex(_item.row());
    if ( idx==Q_NULLPTR ) return;
    QString _name = idx->getName();
    DATA conn_Status;
    conn_Status = idx->getData();
    QString key = conn_Status.value(QString("initName")).toString();
    ConnElement *conn = static_cast<ConnElement*>(
                connections->value(key));
    if ( Q_NULLPTR==conn ) return;
    if ( key.compare(_name)!=0 ) {
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
        showMessage(tr("Info"), tr("Connection is busy."));
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
    QModelIndex _item;
    for (int j=0; j<connItemModel->columnCount(); j++) {
        QString data;
        if (j==2) {
            data = "-";
        } else if (j==1) {
            data = uri;
        } else data = s;
        _item = connItemModel->index(0,j);
        connItemModel->setData(_item, data, Qt::EditRole);
    };
    if ( !_item.isValid() ) return;
    ConnItemIndex *idx = getConnItemDataListIndex(_item.row());
    if ( idx==Q_NULLPTR ) return;
    QString key = idx->getName();
    connections->insert(key, new ConnElement(this));
    ConnElement *conn = static_cast<ConnElement*>(
                connections->value(key));
    if ( Q_NULLPTR==conn ) {
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
    connect(conn, SIGNAL(domStateChanged()),
            this, SIGNAL(domStateChanged()));
    connect(conn, SIGNAL(netStateChanged()),
            this, SIGNAL(netStateChanged()));
    connect(conn, SIGNAL(poolStateChanged()),
            this, SIGNAL(poolStateChanged()));
    connect(conn, SIGNAL(secStateChanged()),
            this, SIGNAL(secStateChanged()));
    connect(conn, SIGNAL(connClosed(bool, const QString&)),
            this, SIGNAL(connClosed(bool, const QString&)));
    connect(conn, SIGNAL(connClosed(bool, const QString&)),
            this, SLOT(newItemClosed(bool)));
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
    QString title(tr("Search local connections"));
    QString time = QTime::currentTime().toString();
    QString errorMsg = QString(
    tr("<b>%1 %2:</b><br><font color='red'><b>ERROR</b></font>: %3"))
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
    if ( Q_NULLPTR==obj ) return;
    QString text;
    QLineEdit::EchoMode mode = (crd.toLower().compare("password")==0) ?
                QLineEdit::PasswordEchoOnEdit :
                QLineEdit::Normal;
    text = QInputDialog::getText(
                    this,
                    QString(tr("<%1> credentials")).arg(obj->getName()),
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
    if ( waitLocalConn->isRunning() ) return;
    if ( searchThread->isRunning() ) return;
    if ( onViewExist || connections->count()==0 ) return;
    QList<ConnItemIndex*>::const_iterator i;
    for (i=connItemModel->connItemDataList.constBegin();
         i!=connItemModel->connItemDataList.constEnd();
         ++i) {
        ConnItemIndex *idx = (*i);
        if ( idx==Q_NULLPTR ) continue;
        const QString _name = idx->getName();
        if ( _newName.compare(_name)!=0 ) continue;
        ConnElement *conn = static_cast<ConnElement*>(
                    connections->value(_name));
        if ( Q_NULLPTR==conn ) continue;
        conn->overviewOfConnection();
        // set flag to TRUE
        onViewExist = true;
        break;
    };
}
void ConnectionList::newItemClosed(bool onView)
{
    // if closed connection was onView, then set flag to FALSE
    if ( onView ) onViewExist = false;
}
