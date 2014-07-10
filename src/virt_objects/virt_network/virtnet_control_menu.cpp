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
        start = 0;
        delete destroy;
        destroy = 0;
        delete undefine;
        undefine = 0;
        delete autoStart;
        autoStart = 0;
        delete getXMLDesc;
        getXMLDesc = 0;
    };
    delete reload;
    reload = 0;
}
void VirtNetControlMenu::emitExecMethod(QAction *action)
{
    QStringList paramList;
    if ( !parameters.isEmpty() ) {
        if ( action == start) {
            paramList << "startVirtNetwork";
        } else if ( action == destroy ) {
            paramList << "destroyVirtNetwork";
        } else if ( action == undefine ) {
            paramList << "undefineVirtNetwork";
        } else if ( action == autoStart ) {
            paramList << "setAutostartVirtNetwork";
            paramList << QString((parameters[2]=="yes")? "0" : "1");
        } else if ( action == getXMLDesc ) {
            paramList << "getVirtNetXMLDesc";
        } else return;
        paramList.append(parameters.first());
    } else if ( action == reload ) {
        paramList << "reloadVirtNetwork";
    } else return;
    //qDebug()<<paramList<<"paramList from menu";
    emit execMethod(paramList);
}
