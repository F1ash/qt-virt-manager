#include "dock_menu.h"

DockMenu::DockMenu(QWidget *parent) :
    QMenu(parent)
{
    logAct = addAction(
                QIcon::fromTheme("utilities-log-viewer"),
                tr("&Log"));
    logAct->setCheckable(true);
    addSeparator();
    domainAct = addAction(
                QIcon::fromTheme("domain"),
                tr("&Virtual Machines"));
    domainAct->setCheckable(true);
    networkAct = addAction(
                QIcon::fromTheme("network"),
                tr("&Networks"));
    networkAct->setCheckable(true);
    storageAct = addAction(
                QIcon::fromTheme("storage"),
                tr("&Storage Pools"));
    storageAct->setCheckable(true);
    ifaceAct = addAction(
                QIcon::fromTheme("network-wired"),
                tr("&Interfaces"));
    ifaceAct->setCheckable(true);
    secretAct = addAction(
                QIcon::fromTheme("security"),
                tr("Sec&rets"));
    secretAct->setCheckable(true);
    nwfilterAct = addAction(
                QIcon::fromTheme("nwfilter"),
                tr("Network &Filters"));
    nwfilterAct->setCheckable(true);
}
