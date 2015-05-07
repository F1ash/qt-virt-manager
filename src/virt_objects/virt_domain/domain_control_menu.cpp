#include "domain_control_menu.h"

DomainControlMenu::DomainControlMenu(QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    if ( !parameters.isEmpty() ) {
        if (parameters.isEmpty()) return;
        start = new QAction("Start", this);
        start->setIcon(QIcon::fromTheme("domain-start"));
        start->setVisible(parameters.last()=="yes" && parameters[1]!="active" );
        pause = new QAction("Pause", this);
        pause->setIcon(QIcon::fromTheme("domain-pause"));
        pause->setVisible(parameters[1]=="active");
        destroy = new QAction("Destroy", this);
        destroy->setIcon(QIcon::fromTheme("domain-stop"));
        destroy->setVisible(parameters[1]=="active");
        edit = new QAction("Edit", this);
        edit->setIcon(QIcon::fromTheme("preferences-system"));
        edit->setVisible(true);
        reset = new QAction("Reset", this);
        reset->setIcon(QIcon::fromTheme("reset"));
        reset->setVisible(parameters[1]=="active");
        reboot = new QAction("Reboot", this);
        reboot->setIcon(QIcon::fromTheme("reboot"));
        reboot->setVisible(parameters[1]=="active");
        shutdown = new QAction("Shutdown", this);
        shutdown->setIcon(QIcon::fromTheme("shutdown"));
        shutdown->setVisible(parameters[1]=="active");
        save = new QAction("Save", this);
        save->setIcon(QIcon::fromTheme("save"));
        save->setVisible(parameters[1]=="active");
        undefine = new QAction("Undefine", this);
        undefine->setIcon(QIcon::fromTheme("domain-undefine"));
        undefine->setVisible(parameters.last()=="yes");
        autoStart = new QAction("change AutoStart", this);
        autoStart->setIcon(QIcon::fromTheme("domain-autostart"));
        autoStart->setVisible(parameters.last()=="yes");
        createSnapshot = new QAction("Snapshot now!", this);
        createSnapshot->setIcon(QIcon::fromTheme("camera-photo"));
        getXMLDesc = new QAction("get XML Description", this);
        getXMLDesc->setIcon(QIcon::fromTheme("domain-xml"));
        getXMLDesc->setVisible(true);
        display = new QAction("display VM", this);
        display->setIcon(QIcon::fromTheme("display"));
        display->setVisible(parameters[1]=="active");
        addToMonitor = new QAction("add to State Monitor", this);
        addToMonitor->setIcon(QIcon::fromTheme("utilities-monitor"));
        addToMonitor->setVisible(true);
        migrate = new QAction("Migrate", this);
        migrate->setIcon(QIcon::fromTheme("migrate"));
        migrate->setVisible(parameters[1]=="active");

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
        addAction(createSnapshot);
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
    reload->setVisible(!autoReloadState);
    setSeparatorsCollapsible(true);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
}

/* private slots */
void DomainControlMenu::emitExecMethod(QAction *action)
{
    QStringList paramList;
    if ( !parameters.isEmpty() ) {
        if ( action == start) {
            paramList.append( "startVirtDomain");
        } else if ( action == pause ) {
            paramList.append("pauseVirtDomain");
        } else if ( action == destroy ) {
            paramList.append("destroyVirtDomain");
        } else if ( action == edit ) {
            paramList.append("editVirtDomain");
        } else if ( action == reset ) {
            paramList.append("resetVirtDomain");
        } else if ( action == reboot ) {
            paramList.append("rebootVirtDomain");
        } else if ( action == shutdown ) {
            paramList.append("shutdownVirtDomain");
        } else if ( action == save ) {
            paramList.append("saveVirtDomain");
        } else if ( action == undefine ) {
            paramList.append("undefineVirtDomain");
        } else if ( action == autoStart ) {
            paramList.append("setAutostartVirtDomain");
            paramList.append(QString((parameters[2]=="yes")? "0" : "1"));
        } else if ( action == getXMLDesc ) {
            paramList.append("getVirtDomainXMLDesc");
        } else if ( action == display ) {
            paramList.append("displayVirtDomain");
        } else if ( action == addToMonitor ) {
            paramList.append("monitorVirtDomain");
        } else if ( action == migrate ) {
            paramList.append("migrateVirtDomain");
        } else if ( action == reload ) {
            paramList.append("reloadVirtDomain");
        } else if ( action == createSnapshot ) {
            paramList.append("createVirtDomainSnapshot");
        } else return;
        if ( action != reload ) paramList.append(parameters.first());
    } else if ( action == reload ) {
        paramList.append("reloadVirtDomain");
    } else return;
    //qDebug()<<paramList<<"paramList from menu";
    emit execMethod(paramList);
}
