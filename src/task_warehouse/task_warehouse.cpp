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
    if ( _taskDesc.count()>1 ) {
        ++counter;
        QString _number = QString("");
        QString _name = QString("#%1 %2 <%3>")
                .arg(_number.sprintf("%08d", counter))
                .arg(_taskDesc[0])
                .arg(_taskDesc[1]);
        QListWidgetItem *_item = new QListWidgetItem();
        _item->setText(_name);
        _item->setIcon(QIcon::fromTheme("run"));
        taskList->addItem(_item);
        QString _msg = _taskDesc.join(" ");
        emit taskMsg(_msg);
    };
    ControlThread *actThread = NULL;
    if ( _taskDesc[0].contains("Domain") ) {
        actThread = new DomControlThread(this);
    } else if ( _taskDesc[0].contains("Network") ) {

    } else if ( _taskDesc[0].contains("StoragePool") ) {

    } else if ( _taskDesc[0].contains("StorageVol") ) {

    } else return;
    virConnectPtr currWorkConnect = _conn;
    int ret = virConnectRef(currWorkConnect);
    if ( ret<0 ) {
        virErrorPtr virtErrors = virGetLastError();
        if ( virtErrors!=NULL && virtErrors->code>0 ) {
            /*
            QString time = QTime::currentTime().toString();
            QString msg = QString("%3 VirtError(%1) : %2")
                    .arg(virtErrors->code)
                    .arg(virtErrors->message)
                    .arg(time);
            emit domMsg( msg );
            */
            virResetError(virtErrors);
        };
        currWorkConnect = NULL;
    } else if ( NULL!=actThread ) {
        actThread->setCurrentWorkConnect(currWorkConnect, counter);
        actThread->execAction(GET_ALL_DOMAIN, QStringList());
        qDebug()<<_taskDesc<<"addNewTask";
    };
}

/* private slots */
void TaskWareHouse::closeEvent(QCloseEvent *ev)
{
    ev->ignore();
}
void TaskWareHouse::taskStateReceiver(uint, bool)
{

}
void TaskWareHouse::taskResultReceiver(uint, int, Result)
{

}
