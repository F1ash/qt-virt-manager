#include "virt_interface_toolbar.h"

InterfaceToolBar::InterfaceToolBar(QWidget *parent) :
    QToolBar(parent)
{
    start_Action = new QAction(this);
    start_Action->setIcon(QIcon::fromTheme("start"));
    start_Action->setToolTip("Start");
    destroy_Action = new QAction(this);
    destroy_Action->setIcon(QIcon::fromTheme("destroy"));
    destroy_Action->setToolTip("Destroy");
    define_Action = new QAction(this);
    define_Action->setIcon(QIcon::fromTheme("define"));
    define_Action->setToolTip("Define for persistent usage");
    define_Menu = new OpenFileMenu(
                this, Actions::DEFINE_ENTITY, VIRT_ENTITY::VIRT_INTERFACE);
    define_Action->setMenu(define_Menu);
    connect(define_Action, SIGNAL(triggered()),
            this, SLOT(showMenu()));
    undefine_Action = new QAction(this);
    undefine_Action->setIcon(QIcon::fromTheme("undefine"));
    undefine_Action->setToolTip("Undefine");
    changeBegin_Action = new QAction(this);
    changeBegin_Action->setIcon(QIcon::fromTheme("document-open"));
    changeBegin_Action->setToolTip("Change Begin");
    changeCommit_Action = new QAction(this);
    changeCommit_Action->setIcon(QIcon::fromTheme("document-save"));
    changeCommit_Action->setToolTip("Change Commit");
    changeRollback_Action = new QAction(this);
    changeRollback_Action->setIcon(QIcon::fromTheme("document-revert"));
    changeRollback_Action->setToolTip("Change Rollback");
    //getXMLDesc_Action = new QAction(this);
    //getXMLDesc_Action->setIcon(QIcon::fromTheme("application-xml"));
    //getXMLDesc_Action->setToolTip("Get XML Description");
    _autoReload = new QPushButton(this);
    _autoReload->setToolTip("AutoReload Interface Overview");
    _autoReload->setIcon(QIcon::fromTheme("view-refresh"));
    _autoReload->setCheckable(true);

    addAction(start_Action);
    addAction(destroy_Action);
    addAction(define_Action);
    addAction(undefine_Action);
    addSeparator();
    addAction(changeBegin_Action);
    addAction(changeCommit_Action);
    addAction(changeRollback_Action);
    //addSeparator();
    //addAction(getXMLDesc_Action);
    addSeparator();
    autoReload = addWidget(_autoReload);
    addAction(autoReload);

    settings.beginGroup("VirtIfaceControl");
    interval = settings.value("UpdateTime", 3).toInt();
    _autoReload->setChecked(
                settings.value("AutoReload", false)
                .toBool());
    settings.endGroup();

    connect(_autoReload, SIGNAL(toggled(bool)),
            this, SLOT(changeAutoReloadState(bool)));

    connect(this, SIGNAL(actionTriggered(QAction*)),
            this, SLOT(detectTriggeredAction(QAction*)));

    connect(define_Menu, SIGNAL(fileForMethod(const Act_Param&)),
            this, SIGNAL(fileForMethod(const Act_Param&)));
}
InterfaceToolBar::~InterfaceToolBar()
{
    settings.beginGroup("VirtIfaceControl");
    settings.setValue("UpdateTime", interval);
    settings.setValue("AutoReload", _autoReload->isChecked());
    settings.endGroup();
}

/* public slots */
Qt::ToolBarArea InterfaceToolBar::get_ToolBarArea(int i) const
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
void InterfaceToolBar::enableAutoReload()
{
    if ( _autoReload->isChecked() )
        timerId = startTimer(interval*1000);
}
void InterfaceToolBar::stopProcessing()
{
    if ( timerId ) {
        killTimer(timerId);
        timerId = 0;
    }
}
bool InterfaceToolBar::getAutoReloadState() const
{
    return _autoReload->isChecked();
}

/* private slots */
void InterfaceToolBar::timerEvent(QTimerEvent *event)
{
    int _timerId = event->timerId();
    //qDebug()<<_timerId<<timerId;
    if ( _timerId && timerId==_timerId ) {
        Act_Param parameters;
        parameters.method = Methods::reloadEntity;
        emit execMethod(parameters);
    };
}
void InterfaceToolBar::showMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act==nullptr ) return;
    if ( act->menu()->isVisible() ) act->menu()->hide();
    else {
        act->menu()->show();
        act->menu()->move(QCursor::pos());
    };
}
void InterfaceToolBar::detectTriggeredAction(QAction *action)
{
    Act_Param parameters;
    if        ( action == undefine_Action ) {
        parameters.method = Methods::undefineEntity;
    } else if ( action == start_Action ) {
        parameters.method = Methods::startEntity;
    } else if ( action == destroy_Action ) {
        parameters.method = Methods::destroyEntity;
    } else if ( action == changeBegin_Action ) {
        parameters.method = Methods::changeBeginVirtInterface;
    } else if ( action == changeCommit_Action ) {
        parameters.method = Methods::changeCommitVirtInterface;
    } else if ( action == changeRollback_Action ) {
        parameters.method = Methods::changeRollbackVirtInterface;
    //} else if ( action == getXMLDesc_Action ) {
    //    parameters << "getVirtInterfaceXMLDesc";
    } else return;
    emit execMethod(parameters);
}
void InterfaceToolBar::changeAutoReloadState(bool state)
{
    if ( state ) enableAutoReload();
    else stopProcessing();
}
