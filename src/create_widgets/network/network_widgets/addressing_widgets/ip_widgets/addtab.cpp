#include "addtab.h"

AddTab::AddTab(QWidget *parent) : QMenu(parent)
{
    addTabAction = new QAction(this);
    addTabAction->setIcon(QIcon::fromTheme("list-add"));
    addTabAction->setText("add new IP Tab");
    addAction(addTabAction);
}
