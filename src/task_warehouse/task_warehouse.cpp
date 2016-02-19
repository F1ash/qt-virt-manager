#include "task_warehouse.h"

#define LIST_STYLE QString("\
QListWidget::item {\
    border-style: outset;\
    border-width: 2px;\
    border-radius: 10px;\
    border-color: beige;\
    font: bold 14px;\
    min-width: 10em;\
    padding: 6px;\
}\
")

TaskWareHouse::TaskWareHouse(QWidget *parent) :
    QMainWindow(parent)
{
    setSizePolicy(QSizePolicy
                  (QSizePolicy::MinimumExpanding,
                   QSizePolicy::MinimumExpanding));
    setContentsMargins(0, 0, 0, 5);
    setWindowTitle("Task WareHouse");
    setWindowIcon(QIcon::fromTheme("virtual-engineering"));
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
    taskList->setStyleSheet(LIST_STYLE);
    connect(taskList, SIGNAL(clicked(QModelIndex)),
            taskList, SLOT(clearSelection()));
    setCentralWidget(taskList);
    threadPool = new THREAD_POOL;
}

/* public slots */
void TaskWareHouse::changeVisibility()
{
    setVisible(!isVisible());
    emit visibilityChanged(isVisible());
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
    blockSignals(true);
}
void TaskWareHouse::addNewTask(TASK task)
{
    //qDebug()<<task.srcConnPtr<<task.srcConName<<task.action
    //      <<task.method<<task.object<<task.args.list()
    //      <<task.args.destConnPtr<<task.type<<"addNewTask_TASK";
    //
    ++counter;
    QString _number = QString("").sprintf("%08d", counter);
    if ( !task.method.startsWith("reload") &&
         !task.method.startsWith("edit") ) {
        QString _name = QString("#%1 %2 <%3> in <%4> connection")
                .arg(_number)
                .arg(task.method)
                .arg(task.object)
                .arg(task.srcConName);
        QListWidgetItem *_item = new QListWidgetItem();
        _item->setText(_name);
        _item->setIcon(QIcon::fromTheme("ledlightgreen"));
        QTime _time = QTime::currentTime();
        QMap<QString, QVariant> itemData;
        itemData.insert("Connection", task.srcConName);
        itemData.insert("Object", task.object);
        itemData.insert("Action", task.method);
        itemData.insert("Start", QString("%1:%2:%3:%4")
                        .arg(QString("").sprintf("%02d", _time.hour()))
                        .arg(QString("").sprintf("%02d", _time.minute()))
                        .arg(QString("").sprintf("%02d", _time.second()))
                        .arg(QString("").sprintf("%03d", _time.msec())));
        itemData.insert("End", "-");
        itemData.insert("Arguments", task.args.list());
        itemData.insert("Result", "Processing");
        itemData.insert("Message", "-");
        _item->setData(Qt::UserRole, itemData);
        setNewTooltip(_item);
        taskList->addItem(_item);
    };
    if        ( task.type == "domain" ) {
        threadPool->insert(
                    _number,
                    new DomControlThread(this));
    } else if ( task.type == "network" ) {
        threadPool->insert(
                    _number,
                    new NetControlThread(this));
    } else if ( task.type == "pool" ) {
        threadPool->insert(
                    _number,
                    new StoragePoolControlThread(this));
    } else if ( task.type == "volume" ) {
        threadPool->insert(
                    _number,
                    new StorageVolControlThread(this));
    } else if ( task.type == "secret" ) {
        threadPool->insert(
                    _number,
                    new SecretControlThread(this));
    } else if ( task.type == "iface" ) {
        threadPool->insert(
                    _number,
                    new InterfaceControlThread(this));
    } else return;
    ControlThread *cThread = static_cast<ControlThread*>(
                threadPool->value(_number));
    if ( nullptr!=cThread ) {
        connect(cThread, SIGNAL(errorMsg(QString&,uint)),
                this, SLOT(msgRepeater(QString&, uint)));
        connect(cThread, SIGNAL(resultData(Result)),
                this, SLOT(taskResultReceiver(Result)));
        cThread->execAction(counter, task);
    };
}

/* private slots */
void TaskWareHouse::closeEvent(QCloseEvent *ev)
{
    ev->ignore();
}
void TaskWareHouse::msgRepeater(QString &msg, uint _number)
{
    QString time = QTime::currentTime().toString();
    QString number = QString("").sprintf("%08d", _number);
    QString title = QString("in TASK #%1").arg(number);
    QString currMsg = QString("<b>%1 %2:</b><br><font color='red'><b>ERROR</b></font>: %3")
            .arg(time).arg(title).arg(msg);
    emit taskMsg(currMsg);
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
    } else if ( data.type=="secret" ) {
        emit secResult(data);
    } else if ( data.type=="iface" ) {
        emit ifaceResult(data);
    } else return;
    QString _number = QString("").sprintf("%08d", data.number);
    ControlThread *cThread = static_cast<ControlThread*>(
                threadPool->value(_number));
    if ( nullptr!=cThread ) {
        //qDebug()<<_number<<"delete";
        disconnect(cThread, SIGNAL(errorMsg(QString&, uint)),
                   this, SLOT(msgRepeater(QString&, uint)));
        disconnect(cThread, SIGNAL(resultData(Result)),
                   this, SLOT(taskResultReceiver(Result)));
        threadPool->value(_number)->quit();
        delete cThread;
        cThread = nullptr;
        //delete threadPool->value(_number);
        int deleted = threadPool->remove(_number);
        //qDebug()<<_number<<"deleted:"<<deleted;
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
        QMap<QString, QVariant> _data = _list.at(0)->data(Qt::UserRole).toMap();
        QTime _time = QTime::currentTime();
        _data.insert("End", QString("%1:%2:%3:%4")
                     .arg(QString("").sprintf("%02d", _time.hour()))
                     .arg(QString("").sprintf("%02d", _time.minute()))
                     .arg(QString("").sprintf("%02d", _time.second()))
                     .arg(QString("").sprintf("%03d", _time.msec())));
        _data.insert("Result", (data.result)? "Success":"Fail");
        _data.insert("Message", data.msg.join("\n"));
        _data.insert("Error", data.err);
        _list.at(0)->setData(Qt::UserRole, _data);
        taskList->scrollToItem(_list.at(0));
        setNewTooltip(_list.at(0));
    };
}
void TaskWareHouse::setNewTooltip(QListWidgetItem *_item)
{
    QString _toolTip, _table, _conn, _dom, _arg, _task, _time, _res, _msg, _err;
    QVariant data = _item->data(Qt::UserRole);
    _conn.append(QString("<TR><TD>%1</TD><TD>%2</TD></TR>")
                 .arg("<b>Connection</b>")
                 .arg(data.toMap().value("Connection").toString()));
    _dom.append (QString("<TR><TD>%1</TD><TD>%2</TD></TR>")
                 .arg("<b>Object</b>")
                 .arg(data.toMap().value("Object").toString()));
    _task.append(QString("<TR><TD>%1</TD><TD>%2</TD></TR>")
                 .arg("<b>Action</b>")
                 .arg(data.toMap().value("Action").toString()));
    _arg.append (QString("<TR><TD>%1</TD><TD>%2</TD></TR>")
                 .arg("<b>Arguments</b>")
                 .arg(data.toMap().value("Arguments").toString()));
    _time.append(QString("<TR><TD>%1</TD><TD>%2 - %3</TD></TR>")
                 .arg("<b>Time</b>")
                 .arg(data.toMap().value("Start").toString())
                 .arg(data.toMap().value("End").toString()));
    _res.append (QString("<TR><TD>%1</TD><TD>%2</TD></TR>")
                 .arg("<b>Result</b>")
                 .arg(data.toMap().value("Result").toString()));
    _msg.append (QString("<TR><TD>%1</TD><TD>%2</TD></TR>")
                 .arg("<b>Message</b>")
                 .arg(data.toMap().value("Message").toString()));
    _err.append (QString("<TR><TD>%1</TD><TD>%2</TD></TR>")
                 .arg("<b>Error</b>")
                 .arg(data.toMap().value("Error").toString()));
    _table.append(QString("%1%2%3%4%5%6%7%8")
                  .arg(_conn)
                  .arg(_dom)
                  .arg(_task)
                  .arg(_arg)
                  .arg(_time)
                  .arg(_res)
                  .arg(_msg)
                  .arg(_err));
    _toolTip = QString("<TABLE BORDER=3>%1</TABLE>").arg(_table);
    _item->setToolTip(_toolTip);
}
