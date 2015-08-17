#include "log_dock_menu.h"

LogDockMenu::LogDockMenu(QWidget *parent) :
    QMenu(parent)
{
    useNameTemplate = new QAction("Use name template for log-file", this);
    saveAtExit = new QAction("Save log at exit", this);
    useNameTemplate->setCheckable(true);
    saveAtExit->setCheckable(true);
    addAction(useNameTemplate);
    addAction(saveAtExit);
}
