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
    qDebug()<<_taskDesc<<"addNewTask";
    QString currConnName = _taskDesc.takeFirst();
    int ACT = _taskDesc.takeFirst().toInt();
    ++counter;
    QString _number = QString("").sprintf("%08d", counter);
    if ( _taskDesc.count()>1 ) {
        QString _name = QString("#%1 %2 <%3>")
                .arg(_number)
                .arg(_taskDesc[0])
                .arg(_taskDesc[1]);
        QListWidgetItem *_item = new QListWidgetItem();
        _item->setText(_name);
        _item->setIcon(QIcon::fromTheme("run"));
        taskList->addItem(_item);
    };
    ControlThread *cThread = NULL;
    if ( _taskDesc[0].contains("Domain") ) {
        threadPool->insert(
                    _number,
                    new DomControlThread(this));
        cThread = static_cast<ControlThread*>(
                    threadPool->value(_number));
    } else if ( _taskDesc[0].contains("Network") ) {
        threadPool->insert(
                    _number,
                    new NetControlThread(this));
        cThread = static_cast<ControlThread*>(
                    threadPool->value(_number));
    } else if ( _taskDesc[0].contains("StoragePool") ) {
        threadPool->insert(
                    _number,
                    new StoragePoolControlThread(this));
        cThread = static_cast<ControlThread*>(
                    threadPool->value(_number));
    } else if ( _taskDesc[0].contains("StorageVol") ) {
        threadPool->insert(
                    _number,
                    new StorageVolControlThread(this));
        cThread = static_cast<ControlThread*>(
                    threadPool->value(_number));
    } else return;
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
        qDebug()<<ACT<<_taskDesc;
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
void TaskWareHouse::taskStateReceiver(uint, bool)
{

}
void TaskWareHouse::taskResultReceiver(Result)
{

}
