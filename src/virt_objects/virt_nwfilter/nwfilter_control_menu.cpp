#include "nwfilter_control_menu.h"

VirtNWFilterControlMenu::VirtNWFilterControlMenu(
        QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    edit = new QAction("Edit", this);
    edit->setIcon(QIcon::fromTheme("configure"));
    getXMLDesc = new QAction("get XML Description", this);
    getXMLDesc->setIcon(QIcon::fromTheme("application-xml"));
    getXMLDesc->setEnabled(true);
    reload = new QAction("Reload NWFilter OverView", this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(edit);
    addSeparator();
    addAction(getXMLDesc);
    addSeparator();
    addAction(reload);

    connect(this, SIGNAL(triggered(QAction*)),
            this, SLOT(emitExecMethod(QAction*)));
}

void VirtNWFilterControlMenu::emitExecMethod(QAction *action)
{
    QStringList paramList;
    if ( !parameters.isEmpty() ) {
        if        ( action == edit ) {
            paramList.append("editVirtNWFilter");
        } else if ( action == getXMLDesc ) {
            paramList.append("getVirtNWFilterXMLDesc");
        } else if ( action == reload ) {
            paramList.append("reloadVirtNWFilter");
        } else return;
        if ( action != reload ) paramList.append(parameters.first());
    } else if ( action == reload ) {
        paramList.append("reloadVirtNWFilter");
    } else return;
    //qDebug()<<paramList<<"paramList from menu";
    emit execMethod(paramList);
}
