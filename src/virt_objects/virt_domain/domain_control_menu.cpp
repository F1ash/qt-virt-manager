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
        edit = new QAction("Edit", this);
        edit->setIcon(QIcon::fromTheme("preferences-system"));
        edit->setEnabled(true);
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
        display->setEnabled(parameters[1]=="active");
        addToMonitor = new QAction("add to State Monitor", this);
        addToMonitor->setIcon(QIcon::fromTheme("utilities-monitor"));
        addToMonitor->setEnabled(true);
        migrate = new QAction("Migrate", this);
        migrate->setIcon(QIcon::fromTheme("migrate"));
        migrate->setEnabled(parameters[1]=="active");

        addAction(start);
        addAction(pause);
        addAction(destroy);
        addSeparator();
        addAction(edit);
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
        addAction(addToMonitor);
        addSeparator();
        addAction(migrate);
        addSeparator();
    };
    reload = new QAction("Reload Domain OverView", this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
}

/* private slots */
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
        } else if ( action == edit ) {
            paramList << "editVirtDomain";
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
            paramList << "getVirtDomainXMLDesc";
        } else if ( action == display ) {
            paramList << "displayVirtDomain";
        } else if ( action == addToMonitor ) {
            paramList << "monitorVirtDomain";
        } else if ( action == migrate ) {
            paramList << "migrateVirtDomain";
        } else return;
        paramList.append(parameters.first());
    } else if ( action == reload ) {
        paramList << "reloadVirtDomain";
    } else return;
    //qDebug()<<paramList<<"paramList from menu";
    emit execMethod(paramList);
}
