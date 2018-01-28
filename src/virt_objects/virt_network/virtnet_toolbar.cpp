#include "virtnet_toolbar.h"

VirtNetToolBar::VirtNetToolBar(QWidget *parent) :
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
                this, Actions::CREATE_ENTITY, VIRT_ENTITY::VIRT_NETWORK);
    create_Action->setMenu(create_Menu);
    connect(create_Action, SIGNAL(triggered()),
            this, SLOT(showMenu()));
    define_Action = new QAction(this);
    define_Action->setIcon(QIcon::fromTheme("define"));
    define_Action->setToolTip(tr("Define for persistent usage"));
    define_Menu = new OpenFileMenu(
                this, Actions::DEFINE_ENTITY, VIRT_ENTITY::VIRT_NETWORK);
    define_Action->setMenu(define_Menu);
    connect(define_Action, SIGNAL(triggered()),
            this, SLOT(showMenu()));
    undefine_Action = new QAction(this);
    undefine_Action->setIcon(QIcon::fromTheme("undefine"));
    undefine_Action->setToolTip(tr("Undefine"));
    //setAutostart_Action = new QAction(this);
    //setAutostart_Action->setIcon(QIcon::fromTheme("autostart"));
    //setAutostart_Action->setToolTip(tr("Change AutoStart State"));
    //getXMLDesc_Action = new QAction(this);
    //getXMLDesc_Action->setIcon(QIcon::fromTheme("application-xml"));
    //getXMLDesc_Action->setToolTip(tr("Get XML Description"));
    _autoReload = new QPushButton(this);
    _autoReload->setToolTip(tr("AutoReload Network Overview"));
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
    //addSeparator();
    //addAction(getXMLDesc_Action);
    addSeparator();
    autoReload = addWidget(_autoReload);
    addAction(autoReload);

    settings.beginGroup("VirtNetControl");
    interval = settings.value("UpdateTime", 3).toInt();
    _autoReload->setChecked(
                settings.value("AutoReload", false)
                .toBool());
    settings.endGroup();

    //connect(start_Action, SIGNAL(hovered()),
    // this, SLOT(showHoveredMenu()));
    //connect(destroy_Action, SIGNAL(hovered()),
    // this, SLOT(showHoveredMenu()));
    //connect(create_Action, SIGNAL(hovered()),
    // this, SLOT(showHoveredMenu()));
    //connect(define_Action, SIGNAL(hovered()),
    // this, SLOT(showHoveredMenu()));
    //connect(undefine_Action, SIGNAL(hovered()),
    // this, SLOT(showHoveredMenu()));
    //connect(setAutostart_Action, SIGNAL(hovered()),
    // this, SLOT(showHoveredMenu()));
    //connect(getXMLDesc_Action, SIGNAL(hovered()),
    // this, SLOT(showHoveredMenu()));
    connect(_autoReload, SIGNAL(toggled(bool)),
            this, SLOT(changeAutoReloadState(bool)));

    connect(create_Menu, SIGNAL(fileForMethod(const Act_Param&)),
            this, SIGNAL(fileForMethod(const Act_Param&)));
    connect(define_Menu, SIGNAL(fileForMethod(const Act_Param&)),
            this, SIGNAL(fileForMethod(const Act_Param&)));
    connect(this, SIGNAL(actionTriggered(QAction*)),
            this, SLOT(detectTriggeredAction(QAction*)));
}
VirtNetToolBar::~VirtNetToolBar()
{
    settings.beginGroup("VirtNetControl");
    settings.setValue("UpdateTime", interval);
    settings.setValue("AutoReload", _autoReload->isChecked());
    settings.endGroup();
}

/* public slots */
Qt::ToolBarArea VirtNetToolBar::get_ToolBarArea(int i) const
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
void VirtNetToolBar::enableAutoReload()
{
    if ( _autoReload->isChecked() )
        timerId = startTimer(interval*1000);
}
void VirtNetToolBar::stopProcessing()
{
    if ( timerId ) {
        killTimer(timerId);
        timerId = 0;
    }
}
bool VirtNetToolBar::getAutoReloadState() const
{
    return _autoReload->isChecked();
}

/* private slots */
void VirtNetToolBar::timerEvent(QTimerEvent *event)
{
    int _timerId = event->timerId();
    //qDebug()<<_timerId<<timerId;
    if ( _timerId && timerId==_timerId ) {
        Act_Param parameters;
        parameters.method = Methods::reloadEntity;
        emit execMethod(parameters);
    };
}
void VirtNetToolBar::showHoveredMenu()
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
void VirtNetToolBar::showMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act==nullptr ) return;
    if ( act->menu()->isVisible() ) act->menu()->hide();
    else {
        act->menu()->show();
        act->menu()->move(QCursor::pos());
    };
}
void VirtNetToolBar::detectTriggeredAction(QAction *action)
{
    Act_Param parameters;
    if ( action == start_Action) {
        parameters.method = Methods::startEntity;
    } else if ( action == destroy_Action ) {
        parameters.method = Methods::destroyEntity;
    } else if ( action == undefine_Action ) {
        parameters.method = Methods::undefineEntity;
    //} else if ( action == setAutostart_Action ) {
    //    parameters << "setAutostartVirtNetwork";
    //} else if ( action == getXMLDesc_Action ) {
    //    parameters << "getVirtNetworkXMLDesc";
    } else return;
    emit execMethod(parameters);
}
void VirtNetToolBar::changeAutoReloadState(bool state)
{
    if ( state ) enableAutoReload();
    else stopProcessing();
}
