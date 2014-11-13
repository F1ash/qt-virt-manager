#include "task_bar.h"

TaskBar::TaskBar(QWidget *parent) :
    QMainWindow(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    setContentsMargins(0, 0, 0, 5);
    setWindowTitle("Task Bar");
    settings.beginGroup("TaskBar");
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
void TaskBar::changeVisibility()
{
    setVisible(!isVisible());
    emit visibilityChanged(isVisible());
}
void TaskBar::saveCurrentState()
{
    settings.beginGroup("TaskBar");
    //settings.setValue("State", saveState());
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("Visible", isVisible());
    settings.endGroup();
    settings.sync();
}
void TaskBar::stopTaskComputing()
{
}

/* private slots */
void TaskBar::closeEvent(QCloseEvent *ev)
{
    ev->ignore();
}
