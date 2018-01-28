#include "storage_pool_toolbar.h"

StoragePoolToolBar::StoragePoolToolBar(QWidget *parent) :
    QToolBar(parent)
{
    start_Action = new QAction(this);
    start_Action->setIcon(QIcon::fromTheme("start"));
    start_Action->setToolTip(tr("Start"));
    destroy_Action = new QAction(this);
    destroy_Action->setIcon(QIcon::fromTheme("destroy"));
    destroy_Action->setToolTip(tr("Stop"));
    create_Action = new QAction(this);
    create_Action->setIcon(QIcon::fromTheme("create"));
    create_Action->setToolTip(tr("Create for once usage"));
    create_Menu = new OpenFileMenu(
                this, Actions::CREATE_ENTITY, VIRT_ENTITY::VIRT_STORAGE_POOL);
    create_Action->setMenu(create_Menu);
    connect(create_Action, SIGNAL(triggered()),
            this, SLOT(showMenu()));
    define_Action = new QAction(this);
    define_Action->setIcon(QIcon::fromTheme("define"));
    define_Action->setToolTip(tr("Define for persistent usage"));
    define_Menu = new OpenFileMenu(
                this, Actions::DEFINE_ENTITY, VIRT_ENTITY::VIRT_STORAGE_POOL);
    define_Action->setMenu(define_Menu);
    connect(define_Action, SIGNAL(triggered()),
            this, SLOT(showMenu()));
    undefine_Action = new QAction(this);
    undefine_Action->setIcon(QIcon::fromTheme("undefine"));
    undefine_Action->setToolTip(tr("Undefine"));
    //setAutostart_Action = new QAction(this);
    //setAutostart_Action->setIcon(QIcon::fromTheme("autostart"));
    //setAutostart_Action->setToolTip(tr("Change AutoStart State"));
    //delete_Menu = new Delete_Pool_Menu(this);
    //delete_Action = new QAction(this);
    //delete_Action->setIcon(QIcon::fromTheme("delete"));
    //delete_Action->setToolTip(tr("Delete"));
    //delete_Action->setMenu(delete_Menu);
    //getXMLDesc_Action = new QAction(this);
    //getXMLDesc_Action->setIcon(QIcon::fromTheme("application-xml"));
    //getXMLDesc_Action->setToolTip(tr("Get XML Description"));
    overview_Action = new QAction(this);
    overview_Action->setIcon(QIcon::fromTheme("overview"));
    overview_Action->setToolTip(tr("Overview Pool"));
    _autoReload = new QPushButton(this);
    _autoReload->setToolTip(tr("AutoReload Pool Overview"));
    _autoReload->setIcon(QIcon::fromTheme("view-refresh"));
    _autoReload->setCheckable(true);

    addAction(start_Action);
    addAction(destroy_Action);
    addSeparator();
    addAction(create_Action);
    addAction(define_Action);
    addAction(undefine_Action);
    //addSeparator();
    //addAction(setAutostart_Action);
    //addAction(delete_Action);
    //addSeparator();
    //addAction(getXMLDesc_Action);
    addSeparator();
    addAction(overview_Action);
    addSeparator();
    autoReload = addWidget(_autoReload);
    addAction(autoReload);

    settings.beginGroup("VirtStoragePoolControl");
    interval = settings.value("UpdateTime", 3).toInt();
    _autoReload->setChecked(
                settings.value("AutoReload", false)
                .toBool());
    settings.endGroup();

    connect(_autoReload, SIGNAL(toggled(bool)),
            this, SLOT(changeAutoReloadState(bool)));

    connect(create_Menu, SIGNAL(fileForMethod(const Act_Param&)),
            this, SIGNAL(fileForMethod(const Act_Param&)));
    connect(define_Menu, SIGNAL(fileForMethod(const Act_Param&)),
            this, SIGNAL(fileForMethod(const Act_Param&)));
    //connect(delete_Menu, SIGNAL(execMethod(const Act_Param&)),
    //        this, SIGNAL(execMethod(const Act_Param&)));
    connect(this, SIGNAL(actionTriggered(QAction*)),
            this, SLOT(detectTriggeredAction(QAction*)));
}
StoragePoolToolBar::~StoragePoolToolBar()
{
    settings.beginGroup("VirtStoragePoolControl");
    settings.setValue("UpdateTime", interval);
    settings.setValue("AutoReload", _autoReload->isChecked());
    settings.endGroup();
}

/* public slots */
Qt::ToolBarArea StoragePoolToolBar::get_ToolBarArea(int i) const
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
void StoragePoolToolBar::enableAutoReload()
{
    if ( _autoReload->isChecked() )
        timerId = startTimer(interval*1000);
}
void StoragePoolToolBar::stopProcessing()
{
    if ( timerId ) {
        killTimer(timerId);
        timerId = 0;
    }
}
bool StoragePoolToolBar::getAutoReloadState() const
{
    return _autoReload->isChecked();
}

/* private slots */
void StoragePoolToolBar::timerEvent(QTimerEvent *event)
{
    int _timerId = event->timerId();
    //qDebug()<<_timerId<<timerId;
    if ( _timerId && timerId==_timerId ) {
        Act_Param parameters;
        parameters.method = Methods::reloadEntity;
        emit execMethod(parameters);
    };
}
void StoragePoolToolBar::showHoveredMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act==nullptr ) return;
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
void StoragePoolToolBar::showMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act==nullptr ) return;
    if ( act->menu()->isVisible() ) act->menu()->hide();
    else {
        act->menu()->show();
        act->menu()->move(QCursor::pos());
    };
}
void StoragePoolToolBar::detectTriggeredAction(QAction *action)
{
    Act_Param parameters;
    if ( action == start_Action) {
        parameters.method = Methods::startEntity;
    } else if ( action == destroy_Action ) {
        parameters.method = Methods::destroyEntity;
    } else if ( action == undefine_Action ) {
        parameters.method = Methods::undefineEntity;
    //} else if ( action == setAutostart_Action ) {
    //    parameters << "setAutostartVirtStoragePool";
    //} else if ( action == getXMLDesc_Action ) {
    //    parameters << "getVirtStoragePoolXMLDesc";
    } else if ( action == overview_Action ) {
        parameters.method = Methods::overviewEntity;
    } else return;
    emit execMethod(parameters);
}
void StoragePoolToolBar::changeAutoReloadState(bool state)
{
    if ( state ) enableAutoReload();
    else stopProcessing();
}
