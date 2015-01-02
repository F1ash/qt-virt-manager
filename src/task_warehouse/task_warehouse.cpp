#include "task_warehouse.h"

TaskWareHouse::TaskWareHouse(QWidget *parent) :
    QMainWindow(parent)
{
    setSizePolicy(QSizePolicy
                  (QSizePolicy::MinimumExpanding,
                   QSizePolicy::MinimumExpanding));
    setContentsMargins(0, 0, 0, 5);
    setWindowTitle("Task WareHouse");
    settings.beginGroup("TaskWareHouse");
    QByteArray _geometry = settings.value("Geometry").toByteArray();
    if ( !_geometry.isEmpty() ) {
        restoreGeometry(_geometry);
    } else {
        setMaximumSize(100, 400);
    };
    setVisible(settings.value("Visible", false).toBool());
    settings.endGroup();
    taskList = new QListWidget(this);
    setCentralWidget(taskList);
    threadPool = new THREAD_POOL;
}

/* public slots */
void TaskWareHouse::changeVisibility()
{
    setVisible(!isVisible());
    emit visibilityChanged(isVisible());
    //addNewTask();
}
void TaskWareHouse::saveCurrentState()
{
    settings.beginGroup("TaskWareHouse");
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("Visible", isVisible());
    settings.endGroup();
    settings.sync();
}
void TaskWareHouse::stopTaskComputing()
{
}
void TaskWareHouse::addNewTask(virConnectPtr _conn, QStringList &_taskDesc)
{
    /* TaskDescription:
     * state icon
     * task number
     * source
     * action type
     * connection name
     * parameters
     */
    //qDebug()<<_taskDesc<<"addNewTask";
    QString currConnName = _taskDesc.takeFirst();
    int ACT = _taskDesc.takeFirst().toInt();
    ++counter;
    QString _number = QString("").sprintf("%08d", counter);
    if ( _taskDesc.count()>0 ) {
        QString _name = QString("#%1 %2 <%3>")
                .arg(_number)
                .arg(_taskDesc[0])
                .arg( (_taskDesc.count()>1)? _taskDesc[1]:".");
        QListWidgetItem *_item = new QListWidgetItem();
        _item->setText(_name);
        _item->setIcon(QIcon::fromTheme("ledlightgreen"));
        taskList->addItem(_item);
    } else return;
    if ( _taskDesc[0].contains("Domain") ) {
        threadPool->insert(
                    _number,
                    new DomControlThread(this));
    } else if ( _taskDesc[0].contains("Network") ) {
        threadPool->insert(
                    _number,
                    new NetControlThread(this));
    } else if ( _taskDesc[0].contains("StoragePool") ) {
        threadPool->insert(
                    _number,
                    new StoragePoolControlThread(this));
    } else if ( _taskDesc[0].contains("StorageVol") ) {
        threadPool->insert(
                    _number,
                    new StorageVolControlThread(this));
        StorageVolControlThread *cThread =
                static_cast<StorageVolControlThread*>(
                    threadPool->value(_number));
        QString poolname = _taskDesc.last();
        cThread->setCurrentStoragePoolName(
                    _conn, poolname);
    } else return;
    ControlThread *cThread = static_cast<ControlThread*>(
                threadPool->value(_number));
    virConnectPtr currWorkConnect = _conn;
    int ret = virConnectRef(currWorkConnect);
    if ( ret<0 ) {
        virErrorPtr virtErrors = virGetLastError();
        if ( virtErrors!=NULL && virtErrors->code>0 ) {
            QString time = QTime::currentTime().toString();
            QString msg = QString("%3 VirtError(%1) : %2")
                    .arg(virtErrors->code)
                    .arg(virtErrors->message)
                    .arg(time);
            virResetError(virtErrors);
            msgRepeater( msg );
        };
        currWorkConnect = NULL;
    } else if ( NULL!=cThread ) {
        _taskDesc.removeFirst();
        //qDebug()<<ACT<<_taskDesc;
        connect(cThread, SIGNAL(errorMsg(QString)),
                this, SLOT(msgRepeater(QString)));
        connect(cThread, SIGNAL(resultData(Result)),
                this, SLOT(taskResultReceiver(Result)));
        cThread->setCurrentWorkConnect(
                    currWorkConnect, counter, currConnName);
        cThread->execAction(static_cast<Actions>(ACT), _taskDesc);
    };
}

/* private slots */
void TaskWareHouse::closeEvent(QCloseEvent *ev)
{
    ev->ignore();
}
void TaskWareHouse::msgRepeater(QString msg)
{
    QString time = QTime::currentTime().toString();
    //QString title = QString("Connect '%1'").arg(currConnName);
    //QString errorMsg = QString("<b>%1 %2:</b><br>%3").arg(time).arg(title).arg(msg);
    QString errorMsg = QString("<b>%1 :</b><br>%2").arg(time).arg(msg);
    emit taskMsg(errorMsg);
}
void TaskWareHouse::taskResultReceiver(Result data)
{
    if ( data.type=="domain" ) {
        emit domResult(data);
    } else if ( data.type=="network" ) {
        emit netResult(data);
    } else if ( data.type=="pool" ) {
        emit poolResult(data);
    } else if ( data.type=="volume" ) {
        emit volResult(data);
    } else return;
    QString _number = QString("").sprintf("%08d", data.number);
    ControlThread *cThread = static_cast<ControlThread*>(
                threadPool->value(_number));
    if ( NULL!=cThread ) {
        disconnect(cThread, SIGNAL(errorMsg(QString)),
                   this, SLOT(msgRepeater(QString)));
        disconnect(cThread, SIGNAL(resultData(Result)),
                   this, SLOT(taskResultReceiver(Result)));
        delete cThread;
        cThread = NULL;
        threadPool->remove(_number);
    };
    QString stateIcon;
    if ( data.result ) {
        stateIcon.append("leddarkblue");
    } else {
        stateIcon.append("ledlightred");
    };
    QList<QListWidgetItem*> _list = taskList->findItems(
                _number.prepend("#"), Qt::MatchStartsWith);
    if ( _list.count()>0 ) {
        _list.at(0)->setIcon(QIcon::fromTheme(stateIcon));
        // set result data to taskList item
        taskList->scrollToItem(_list.at(0));
    };
}
