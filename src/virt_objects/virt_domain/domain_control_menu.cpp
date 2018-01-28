#include "domain_control_menu.h"

DomainControlMenu::DomainControlMenu(
        QWidget *parent, QVariantMap params, bool state) :
    QMenu(parent), autoReloadState(state)
{
    if ( !params.isEmpty() ) {
        active = params.value("active", false).toBool();
        autostart = params.value("auto", false).toBool();
        persistent = params.value("persistent", false).toBool();
        start = new QAction(tr("Start"), this);
        start->setIcon(QIcon::fromTheme("start"));
        start->setVisible(persistent && !active );
        pause = new QAction(tr("Pause"), this);
        pause->setIcon(QIcon::fromTheme("pause"));
        pause->setVisible(active);
        destroy = new QAction(tr("Destroy"), this);
        destroy->setIcon(QIcon::fromTheme("destroy"));
        destroy->setVisible(active);
        edit = new QAction(tr("Edit"), this);
        edit->setIcon(QIcon::fromTheme("configure"));
        edit->setVisible(true);
        reset = new QAction(tr("Reset"), this);
        reset->setIcon(QIcon::fromTheme("reset"));
        reset->setVisible(active);
        reboot = new QAction(tr("Reboot"), this);
        reboot->setIcon(QIcon::fromTheme("reboot"));
        reboot->setVisible(active);
        shutdown = new QAction(tr("Shutdown"), this);
        shutdown->setIcon(QIcon::fromTheme("shutdown"));
        shutdown->setVisible(active);
        save = new QAction(tr("Save"), this);
        save->setIcon(QIcon::fromTheme("save"));
        save->setVisible(active);
        undefine = new QAction(tr("Undefine"), this);
        undefine->setIcon(QIcon::fromTheme("undefine"));
        undefine->setVisible(persistent);
        autoStart = new QAction(tr("change AutoStart"), this);
        autoStart->setIcon(QIcon::fromTheme("autostart"));
        autoStart->setVisible(persistent);
        createSnapshot = new QAction(tr("Snapshot now!"), this);
        createSnapshot->setIcon(QIcon::fromTheme("camera-photo"));
        moreSnapshot_Actions = new QAction(tr("more Snapshot actions"), this);
        moreSnapshot_Actions->setIcon(QIcon::fromTheme("camera-photo"));
        getXMLDesc = new QAction(tr("get XML Description"), this);
        getXMLDesc->setIcon(QIcon::fromTheme("application-xml"));
        getXMLDesc->setVisible(true);
        RunningData = new QAction(tr("current state data"), this);
        InactiveData = new QAction(tr("inactive state data"), this);
        xmlDescParams = new QMenu(this);
        xmlDescParams->addAction(RunningData);
        xmlDescParams->addAction(InactiveData);
        getXMLDesc->setMenu(xmlDescParams);
        display = new QAction(tr("display VM"), this);
        display->setIcon(QIcon::fromTheme("display"));
        display->setVisible(active);
        displayInExternal = new QAction(tr("display VM in external Viewer"), this);
        displayInExternal->setIcon(QIcon::fromTheme("display"));
        displayInExternal->setVisible(active);
        addToMonitor = new QAction(tr("add to State Monitor"), this);
        addToMonitor->setIcon(QIcon::fromTheme("utilities-monitor"));
        addToMonitor->setVisible(true);
        migrate = new QAction(tr("Migrate"), this);
        migrate->setIcon(QIcon::fromTheme("migrate"));
        migrate->setVisible(active);

        addAction(start);
        addAction(pause);
        addAction(destroy);
        addAction(undefine);
        addAction(autoStart);
        addSeparator();
        addAction(edit);
        addSeparator();
        addAction(createSnapshot);
        addAction(moreSnapshot_Actions);
        addSeparator();
        addAction(display);
        addAction(displayInExternal);
        addAction(addToMonitor);
        addSeparator();
        addAction(migrate);
        addSeparator();
        addAction(getXMLDesc);
        addSeparator();
        addAction(reset);
        addAction(reboot);
        addAction(shutdown);
        addAction(save);
        addSeparator();
    };
    reload = new QAction(tr("Reload Domain OverView"), this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setVisible(!autoReloadState);
    setSeparatorsCollapsible(true);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)),
            this, SLOT(emitExecMethod(QAction*)));
}

/* private slots */
void DomainControlMenu::emitExecMethod(QAction *action)
{
    Act_Param paramList;
    if ( action == start) {
        paramList.method = Methods::startEntity;
    } else if ( action == pause ) {
        paramList.method = Methods::pauseEntity;
    } else if ( action == destroy ) {
        paramList.method = Methods::destroyEntity;
    } else if ( action == edit ) {
        paramList.method = Methods::editEntity;
    } else if ( action == reset ) {
        paramList.method = Methods::resetVirtDomain;
    } else if ( action == reboot ) {
        paramList.method = Methods::rebootVirtDomain;
    } else if ( action == shutdown ) {
        paramList.method = Methods::shutdownVirtDomain;
    } else if ( action == save ) {
        paramList.method = Methods::saveVirtDomain;
    } else if ( action == undefine ) {
        paramList.method = Methods::undefineEntity;
    } else if ( action == autoStart ) {
        paramList.method = Methods::setAutostartEntity;
        paramList.path = (autostart)? "0" : "1";
    } else if ( action == RunningData ) {
        paramList.method = Methods::getEntityXMLDesc;
        paramList.context = HOW_TO_DO::DO_AsIs;
    } else if ( action == InactiveData ) {
        paramList.method = Methods::getEntityXMLDesc;
    } else if ( action == display ) {
        paramList.method = Methods::displayVirtDomain;
    } else if ( action == displayInExternal ) {
        paramList.method = Methods::displayVirtDomainInExternalViewer;
    } else if ( action == addToMonitor ) {
        paramList.method = Methods::monitorVirtDomain;
    } else if ( action == migrate ) {
        paramList.method = Methods::migrateVirtDomain;
    } else if ( action == reload ) {
        paramList.method = Methods::reloadEntity;
    } else if ( action == createSnapshot ) {
        paramList.method = Methods::createVirtDomainSnapshot;
    } else if ( action == moreSnapshot_Actions ) {
        paramList.method = Methods::moreSnapshotActions;
    } else return;
    //qDebug()<<paramList<<"paramList from menu";
    emit execMethod(paramList);
}
