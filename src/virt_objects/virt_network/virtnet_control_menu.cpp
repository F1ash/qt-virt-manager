#include "virtnet_control_menu.h"

VirtNetControlMenu::VirtNetControlMenu(QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    if ( !parameters.isEmpty() ) {
        start = new QAction("Start", this);
        start->setIcon(QIcon::fromTheme("network-start"));
        start->setEnabled(parameters.last()=="yes" && parameters[1]!="active" );
        destroy = new QAction("Destroy", this);
        destroy->setIcon(QIcon::fromTheme("network-stop"));
        destroy->setEnabled(parameters[1]=="active");
        undefine = new QAction("Undefine", this);
        undefine->setIcon(QIcon::fromTheme("network-undefine"));
        undefine->setEnabled(parameters.last()=="yes");
        autoStart = new QAction("change AutoStart", this);
        autoStart->setIcon(QIcon::fromTheme("network-autostart"));
        autoStart->setEnabled(parameters.last()=="yes");
        getXMLDesc = new QAction("get XML Description", this);
        getXMLDesc->setIcon(QIcon::fromTheme("network-xml"));
        getXMLDesc->setEnabled(true);

        addAction(start);
        addAction(destroy);
        addAction(undefine);
        addAction(autoStart);
        addSeparator();
        addAction(getXMLDesc);
        addSeparator();
    };
    reload = new QAction("Reload Network OverView", this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
}
VirtNetControlMenu::~VirtNetControlMenu()
{
    disconnect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
    if ( !parameters.isEmpty() ) {
        delete start;
        start = NULL;
        delete destroy;
        destroy = NULL;
        delete undefine;
        undefine = NULL;
        delete autoStart;
        autoStart = NULL;
        delete getXMLDesc;
        getXMLDesc = NULL;
    };
    delete reload;
    reload = NULL;
}
void VirtNetControlMenu::emitExecMethod(QAction *action)
{
    QStringList paramList;
    if ( !parameters.isEmpty() ) {
        if ( action == start) {
            paramList.append("startVirtNetwork");
        } else if ( action == destroy ) {
            paramList.append("destroyVirtNetwork");
        } else if ( action == undefine ) {
            paramList.append("undefineVirtNetwork");
        } else if ( action == autoStart ) {
            paramList.append("setAutostartVirtNetwork");
            paramList.append(QString((parameters[2]=="yes")? "0" : "1"));
        } else if ( action == getXMLDesc ) {
            paramList.append("getVirtNetworkXMLDesc");
        } else if ( action == reload ) {
            paramList.append("reloadVirtNetwork");
        } else return;
        if ( action != reload ) paramList.append(parameters.first());
    } else if ( action == reload ) {
        paramList.append("reloadVirtNetwork");
    } else return;
    //qDebug()<<paramList<<"paramList from menu";
    emit execMethod(paramList);
}
