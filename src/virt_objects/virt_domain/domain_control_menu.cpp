#include "domain_control_menu.h"

DomainControlMenu::DomainControlMenu(QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    if ( !parameters.isEmpty() ) {
        if (parameters.isEmpty()) return;
        start = new QAction("Start", this);
        start->setIcon(QIcon::fromTheme("domain-start"));
        start->setEnabled(parameters.last()=="yes" && parameters[1]!="active" );
        pause = new QAction("Pause", this);
        pause->setIcon(QIcon::fromTheme("domain-pause"));
        pause->setEnabled(parameters[1]=="active");
        destroy = new QAction("Destroy", this);
        destroy->setIcon(QIcon::fromTheme("domain-stop"));
        destroy->setEnabled(parameters[1]=="active");
        reset = new QAction("Reset", this);
        reset->setIcon(QIcon::fromTheme("reset"));
        reset->setEnabled(parameters[1]=="active");
        reboot = new QAction("Reboot", this);
        reboot->setIcon(QIcon::fromTheme("reboot"));
        reboot->setEnabled(parameters[1]=="active");
        shutdown = new QAction("Shutdown", this);
        shutdown->setIcon(QIcon::fromTheme("shutdown"));
        shutdown->setEnabled(parameters[1]=="active");
        save = new QAction("Save", this);
        save->setIcon(QIcon::fromTheme("save"));
        save->setEnabled(parameters[1]=="active");
        undefine = new QAction("Undefine", this);
        undefine->setIcon(QIcon::fromTheme("domain-undefine"));
        undefine->setEnabled(parameters.last()=="yes");
        autoStart = new QAction("change AutoStart", this);
        autoStart->setIcon(QIcon::fromTheme("domain-autostart"));
        autoStart->setEnabled(parameters.last()=="yes");
        getXMLDesc = new QAction("get XML Description", this);
        getXMLDesc->setIcon(QIcon::fromTheme("domain-xml"));
        getXMLDesc->setEnabled(true);
        display = new QAction("display VM", this);
        display->setIcon(QIcon::fromTheme("display"));
        display->setEnabled(true);

        addAction(start);
        addAction(pause);
        addAction(destroy);
        addSeparator();
        addAction(reset);
        addAction(reboot);
        addAction(shutdown);
        addAction(save);
        addSeparator();
        addAction(undefine);
        addAction(autoStart);
        addSeparator();
        addAction(getXMLDesc);
        addSeparator();
        addAction(display);
        addSeparator();
    };
    reload = new QAction("Reload Pool OverView", this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
}
DomainControlMenu::~DomainControlMenu()
{
    disconnect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
    if ( !parameters.isEmpty() ) {
        delete start;
        start = 0;
        delete pause;
        pause = 0;
        delete destroy;
        destroy = 0;
        delete reset;
        reset = 0;
        delete reboot;
        reboot = 0;
        delete shutdown;
        shutdown = 0;
        delete save;
        save = 0;
        delete undefine;
        undefine = 0;
        delete autoStart;
        autoStart = 0;
        delete getXMLDesc;
        getXMLDesc = 0;
        delete display;
        display = 0;
    };
    delete reload;
    reload = 0;
}
void DomainControlMenu::emitExecMethod(QAction *action)
{
    QStringList paramList;
    if ( !parameters.isEmpty() ) {
        if ( action == start) {
            paramList << "startVirtDomain";
        } else if ( action == pause ) {
            paramList << "pauseVirtDomain";
        } else if ( action == destroy ) {
            paramList << "destroyVirtDomain";
        } else if ( action == reset ) {
            paramList << "resetVirtDomain";
        } else if ( action == reboot ) {
            paramList << "rebootVirtDomain";
        } else if ( action == shutdown ) {
            paramList << "shutdownVirtDomain";
        } else if ( action == save ) {
            paramList << "saveVirtDomain";
        } else if ( action == undefine ) {
            paramList << "undefineVirtDomain";
        } else if ( action == autoStart ) {
            paramList << "setAutostartVirtDomain";
            paramList << QString((parameters[2]=="yes")? "0" : "1");
        } else if ( action == getXMLDesc ) {
            paramList << "getVirtDomXMLDesc";
        } else if ( action == display ) {
            paramList << "displayVirtDomain";
        } else return;
        paramList.append(parameters.first());
    } else if ( action == reload ) {
        paramList << "reloadVirtDomain";
    } else return;
    //qDebug()<<paramList<<"paramList from menu";
    emit execMethod(paramList);
}
