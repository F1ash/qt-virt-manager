#include "virtnet_control_menu.h"

VirtNetControlMenu::VirtNetControlMenu(QWidget *parent, QStringList params) :
    QMenu(parent), parameters(params)
{
    if (parameters.isEmpty()) return;
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
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
}
VirtNetControlMenu::~VirtNetControlMenu()
{
    disconnect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
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
}
void VirtNetControlMenu::emitExecMethod(QAction *action)
{
    QStringList paramList;
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
    paramList.insert(1, parameters.first());
    emit execMethod(paramList);
}
