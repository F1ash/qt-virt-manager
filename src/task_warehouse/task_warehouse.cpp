#include "task_warehouse.h"

TaskWareHouse::TaskWareHouse(QWidget *parent) :
    QMainWindow(parent)
{
    setSizePolicy(QSizePolicy
                  (QSizePolicy::MinimumExpanding,
                   QSizePolicy::MinimumExpanding));
    setContentsMargins(0, 0, 0, 5);
    setWindowTitle("Task Bar");
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
void TaskWareHouse::addNewTask(QStringList &_taskDesc)
{
    /* TaskDescription:
     * state icon
     * task number
     * source
     * action type
     * connection name
     * parameters
     */
    ++counter;
    QListWidgetItem *_item = new QListWidgetItem();
    QString _name = QString("#%1 %2 %3")
            .arg(counter)
            .arg(_taskDesc[0])
            .arg(_taskDesc[1]);
    _item->setText(_name);
    _item->setIcon(QIcon::fromTheme("run"));
    taskList->addItem(_item);
    QString _msg = _taskDesc.join(" ");
    emit taskMsg(_msg);
}

/* private slots */
void TaskWareHouse::closeEvent(QCloseEvent *ev)
{
    ev->ignore();
}
