#include "dock_menu.h"

DockMenu::DockMenu(QWidget *parent) :
    QMenu(parent)
{
    logAct = addAction(QIcon::fromTheme("utilities-log-viewer"), "&Log");
    logAct->setCheckable(true);
    addSeparator();
    domainAct = addAction(QIcon::fromTheme("domain"), "&Virtual Machines");
    domainAct->setCheckable(true);
    networkAct = addAction(QIcon::fromTheme("network"), "&Networks");
    networkAct->setCheckable(true);
    storageAct = addAction(QIcon::fromTheme("storage"), "&Storage Pools");
    storageAct->setCheckable(true);
    ifaceAct = addAction(QIcon::fromTheme("network-wired"), "&Interfaces");
    ifaceAct->setCheckable(true);
    secretAct = addAction(QIcon::fromTheme("security"), "Sec&rets");
    secretAct->setCheckable(true);
}
