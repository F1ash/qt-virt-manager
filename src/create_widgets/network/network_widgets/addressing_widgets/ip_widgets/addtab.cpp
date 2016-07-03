#include "addtab.h"

AddTab::AddTab(QWidget *parent) : QMenu(parent)
{
    addTabAction = new QAction(this);
    addTabAction->setIcon(QIcon::fromTheme("list-add"));
    addAction(addTabAction);
    setToolTip("add new IP Element");
}
