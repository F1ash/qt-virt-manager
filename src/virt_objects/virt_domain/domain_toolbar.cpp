#include "domain_toolbar.h"

DomainToolBar::DomainToolBar(QWidget *parent) :
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
    create_Action = new QAction(this);
    create_Action->setIcon(QIcon::fromTheme("domain-create"));
    create_Action->setToolTip("Create");
    create_Menu = new OpenFileMenu(this, "create", "domain");
    create_Action->setMenu(create_Menu);
    connect(create_Action, SIGNAL(triggered()), this, SLOT(showMenu()));
    define_Action = new QAction(this);
    define_Action->setIcon(QIcon::fromTheme("domain-define"));
    define_Action->setToolTip("Define");
    define_Menu = new OpenFileMenu(this, "define", "domain");
    define_Action->setMenu(define_Menu);
    connect(define_Action, SIGNAL(triggered()), this, SLOT(showMenu()));
    undefine_Action = new QAction(this);
    undefine_Action->setIcon(QIcon::fromTheme("domain-undefine"));
    undefine_Action->setToolTip("Undefine");
    setAutostart_Action = new QAction(this);
    setAutostart_Action->setIcon(QIcon::fromTheme("domain-autostart"));
    setAutostart_Action->setToolTip("Change AutoStart State");
    getXMLDesc_Action = new QAction(this);
    getXMLDesc_Action->setIcon(QIcon::fromTheme("domain-xml"));
    getXMLDesc_Action->setToolTip("Get XML Description");
    migrate_Action = new QAction(this);
    migrate_Action->setIcon(QIcon::fromTheme("migrate"));
    migrate_Action->setToolTip("Migrate");
    _autoReload = new QPushButton(this);
    _autoReload->setToolTip("AutoReload Domain Overview");
    _autoReload->setIcon(QIcon::fromTheme("view-refresh"));
    _autoReload->setCheckable(true);
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
    addAction(create_Action);
    addAction(define_Action);
    addAction(undefine_Action);
    //addSeparator();
    addAction(setAutostart_Action);
    addSeparator();
    addAction(snapshot_Action);
    addSeparator();
    addAction(getXMLDesc_Action);
    addSeparator();
    addAction(migrate_Action);
    addSeparator();
    autoReload = addWidget(_autoReload);
    addAction(autoReload);

    settings.beginGroup("VirtDomainControl");
    interval = settings.value("UpdateTime", 3).toInt();
    _autoReload->setChecked(settings.value("AutoReload", false).toBool());
    settings.endGroup();

    //connect(start_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(destroy_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(create_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(define_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(undefine_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(setAutostart_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(getXMLDesc_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    connect(_autoReload, SIGNAL(toggled(bool)), this, SLOT(changeAutoReloadState(bool)));

    connect(create_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    connect(define_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    connect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(detectTriggerredAction(QAction*)));
}
DomainToolBar::~DomainToolBar()
{
    settings.beginGroup("VirtDomainControl");
    settings.setValue("UpdateTime", interval);
    settings.setValue("AutoReload", _autoReload->isChecked());
    settings.endGroup();
    //disconnect(start_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //disconnect(destroy_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //disconnect(create_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //disconnect(define_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //disconnect(undefine_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //disconnect(setAutostart_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //disconnect(getXMLDesc_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    disconnect(_autoReload, SIGNAL(toggled(bool)), this, SLOT(changeAutoReloadState(bool)));
    disconnect(create_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    disconnect(define_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    disconnect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(detectTriggerredAction(QAction*)));

    disconnect(define_Action, SIGNAL(triggered()), this, SLOT(showMenu()));
    disconnect(create_Action, SIGNAL(triggered()), this, SLOT(showMenu()));

    delete start_Action;
    start_Action = NULL;
    delete restore_Action;
    restore_Action = NULL;
    delete start_Menu;
    start_Menu = NULL;
    delete pause_Action;
    pause_Action = NULL;
    delete reset_Action;
    reset_Action = NULL;
    delete reboot_Action;
    reboot_Action = NULL;
    delete shutdown_Action;
    shutdown_Action = NULL;
    delete save_Action;
    save_Action = NULL;
    delete sep;
    sep = NULL;
    delete destroy_Menu;
    destroy_Menu = NULL;
    delete destroy_Action;
    destroy_Action = NULL;
    delete create_Menu;
    create_Menu = NULL;
    delete define_Menu;
    define_Menu = NULL;
    delete create_Action;
    create_Action = NULL;
    delete define_Action;
    define_Action = NULL;
    delete undefine_Action;
    undefine_Action = NULL;
    delete setAutostart_Action;
    setAutostart_Action = NULL;
    delete createSnapshot;
    createSnapshot = NULL;
    delete moreSnapshot_Actions;
    moreSnapshot_Actions = NULL;
    delete snapshot_Menu;
    snapshot_Menu = NULL;
    delete snapshot_Action;
    snapshot_Action = NULL;
    delete getXMLDesc_Action;
    getXMLDesc_Action = NULL;
    delete migrate_Action;
    migrate_Action = NULL;
    delete _autoReload;
    _autoReload = NULL;
    delete autoReload;
    autoReload = NULL;
}

/* public slots */
Qt::ToolBarArea DomainToolBar::get_ToolBarArea(int i) const
{
  Qt::ToolBarArea result;
  switch (i) {
  case 1:
    result = Qt::LeftToolBarArea;
    break;
  case 2:
    result = Qt::RightToolBarArea;
    break;
  case 4:
    result = Qt::TopToolBarArea;
    break;
  case 8:
    result = Qt::BottomToolBarArea;
    break;
  default:
    result = Qt::TopToolBarArea;
    break;
  };
  return result;
}
void DomainToolBar::enableAutoReload()
{
    if ( _autoReload->isChecked() ) timerId = startTimer(interval*1000);
}
void DomainToolBar::stopProcessing()
{
    if ( timerId ) {
        killTimer(timerId);
        timerId = 0;
    }
}
bool DomainToolBar::getAutoReloadState() const
{
    return _autoReload->isChecked();
}

/* private slots */
void DomainToolBar::timerEvent(QTimerEvent *event)
{
    int _timerId = event->timerId();
    //qDebug()<<_timerId<<timerId;
    if ( _timerId && timerId==_timerId ) {
        QStringList parameters;
        parameters << "reloadVirtDomain";
        emit execMethod(parameters);
    };
}
void DomainToolBar::repeatParameters(QStringList &p)
{
    emit fileForMethod(p);
}
void DomainToolBar::showHoveredMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act==create_Action ) {
        if ( define_Action->menu()->isVisible() )
            define_Action->menu()->hide();
    } else if ( act==define_Action ) {
        if ( create_Action->menu()->isVisible() )
            create_Action->menu()->hide();
    } else {
        if ( define_Action->menu()->isVisible() )
            define_Action->menu()->hide();
        if ( create_Action->menu()->isVisible() )
            create_Action->menu()->hide();
        return;
    };
    //act->menu()->popup(mapToGlobal(this->pos()), act);
    act->menu()->show();
    act->menu()->move(QCursor::pos());
}
void DomainToolBar::showMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act->menu()->isVisible() ) act->menu()->hide();
    else {
        act->menu()->show();
        act->menu()->move(QCursor::pos());
    };
}
void DomainToolBar::detectTriggerredAction(QAction *action)
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
    } else if ( action == undefine_Action ) {
        parameters << "undefineVirtDomain";
    } else if ( action == setAutostart_Action ) {
        parameters << "setAutostartVirtDomain";
    } else if ( action == getXMLDesc_Action ) {
        parameters << "getVirtDomainXMLDesc";
    } else if ( action == migrate_Action ) {
        parameters << "migrateVirtDomain";
    } else if ( action == snapshot_Action ) {
        parameters << "createVirtDomainSnapshot";
    } else if ( action == createSnapshot ) {
        parameters << "createVirtDomainSnapshot";
    } else if ( action == moreSnapshot_Actions ) {
        parameters << "moreSnapshotActions";
    } else return;
    emit execMethod(parameters);
}
void DomainToolBar::changeAutoReloadState(bool state)
{
    if ( state ) enableAutoReload();
    else stopProcessing();
}
