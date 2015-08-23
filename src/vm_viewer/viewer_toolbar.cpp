#include "viewer_toolbar.h"

ViewerToolBar::ViewerToolBar(QWidget *parent) :
    QToolBar(parent)
{
    start_Action = new QAction(this);
    start_Action->setIcon(QIcon::fromTheme("domain-start"));
    start_Action->setToolTip("Start");
    start_Menu = new QMenu(this);
    restore_Action = start_Menu->addAction("Restore");
    restore_Action->setIcon(QIcon::fromTheme("domain-start"));
    start_Action->setMenu(start_Menu);
    pause_Action = new QAction(this);
    pause_Action->setIcon(QIcon::fromTheme("domain-pause"));
    pause_Action->setToolTip("Pause");
    destroy_Menu = new QMenu(this);
    reboot_Action = destroy_Menu->addAction("Reboot");
    reboot_Action->setIcon(QIcon::fromTheme("reboot"));
    reset_Action = destroy_Menu->addAction("Reset");
    reset_Action->setIcon(QIcon::fromTheme("reset"));
    sep = destroy_Menu->addSeparator();
    shutdown_Action = destroy_Menu->addAction("Shutdown");
    shutdown_Action->setIcon(QIcon::fromTheme("shutdown"));
    save_Action = destroy_Menu->addAction("Save");
    save_Action->setIcon(QIcon::fromTheme("save"));
    destroy_Action = new QAction(this);
    destroy_Action->setIcon(QIcon::fromTheme("domain-stop"));
    destroy_Action->setToolTip("Stop");
    destroy_Action->setMenu(destroy_Menu);
    snapshot_Menu = new QMenu(this);
    createSnapshot = snapshot_Menu->addAction("create Snapshot of Current Domain");
    createSnapshot->setIcon(QIcon::fromTheme("camera-photo"));
    moreSnapshot_Actions = snapshot_Menu->addAction("more Snapshot actions for Domain");
    moreSnapshot_Actions->setIcon(QIcon::fromTheme("camera-photo"));
    snapshot_Action = new QAction(this);
    snapshot_Action->setIcon(QIcon::fromTheme("camera-photo"));
    snapshot_Action->setToolTip("Snapshot now!");
    snapshot_Action->setMenu(snapshot_Menu);

    addAction(start_Action);
    addAction(pause_Action);
    addAction(destroy_Action);
    addSeparator();
    addAction(snapshot_Action);

    connect(this, SIGNAL(actionTriggered(QAction*)),
            this, SLOT(detectTriggerredAction(QAction*)));
}

/* public slots */

/* private slots */
void ViewerToolBar::showMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act->menu()->isVisible() ) act->menu()->hide();
    else {
        act->menu()->show();
        act->menu()->move(QCursor::pos());
    };
}
void ViewerToolBar::detectTriggerredAction(QAction *action)
{
    QStringList parameters;
    if ( action == start_Action) {
        parameters << "startVirtDomain";
    } else if ( action == pause_Action ) {
        parameters << "pauseVirtDomain";
    } else if ( action == destroy_Action ) {
        parameters << "destroyVirtDomain";
    } else if ( action == reset_Action ) {
        parameters << "resetVirtDomain";
    } else if ( action == reboot_Action ) {
        parameters << "rebootVirtDomain";
    } else if ( action == shutdown_Action ) {
        parameters << "shutdownVirtDomain";
    } else if ( action == save_Action ) {
        parameters << "saveVirtDomain";
    } else if ( action == restore_Action ) {
        parameters << "restoreVirtDomain";
    } else if ( action == snapshot_Action ) {
        parameters << "createVirtDomainSnapshot";
    } else if ( action == createSnapshot ) {
        parameters << "createVirtDomainSnapshot";
    } else if ( action == moreSnapshot_Actions ) {
        parameters << "moreSnapshotActions";
    } else return;
    emit execMethod(parameters);
}
