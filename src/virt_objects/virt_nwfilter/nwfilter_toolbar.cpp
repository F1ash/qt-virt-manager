#include "nwfilter_toolbar.h"

VirtNWFilterToolBar::VirtNWFilterToolBar(QWidget *parent) :
    QToolBar(parent)
{
    define_Action = new QAction(this);
    define_Action->setIcon(QIcon::fromTheme("define"));
    define_Action->setToolTip("Define for persistent usage");
    define_Menu = new OpenFileMenu(this, "define", "nwfilter");
    define_Action->setMenu(define_Menu);
    connect(define_Action, SIGNAL(triggered()),
            this, SLOT(showMenu()));
    undefine_Action = new QAction(this);
    undefine_Action->setIcon(QIcon::fromTheme("undefine"));
    undefine_Action->setToolTip("Undefine");
    //getXMLDesc_Action = new QAction(this);
    //getXMLDesc_Action->setIcon(QIcon::fromTheme("application-xml"));
    //getXMLDesc_Action->setToolTip("Get XML Description");
    _autoReload = new QPushButton(this);
    _autoReload->setToolTip("AutoReload NwFilters Overview");
    _autoReload->setIcon(QIcon::fromTheme("view-refresh"));
    _autoReload->setCheckable(true);

    addAction(define_Action);
    addAction(undefine_Action);
    //addSeparator();
    //addAction(getXMLDesc_Action);
    addSeparator();
    autoReload = addWidget(_autoReload);
    addAction(autoReload);

    settings.beginGroup("VirtNWFilterControl");
    interval = settings.value("UpdateTime", 3).toInt();
    _autoReload->setChecked(settings.value("AutoReload", false).toBool());
    settings.endGroup();

    connect(_autoReload, SIGNAL(toggled(bool)),
            this, SLOT(changeAutoReloadState(bool)));

    connect(define_Menu, SIGNAL(fileForMethod(const OFILE_TASK&)),
            this, SIGNAL(fileForMethod(const OFILE_TASK&)));
    connect(this, SIGNAL(actionTriggered(QAction*)),
            this, SLOT(detectTriggerredAction(QAction*)));
}
VirtNWFilterToolBar::~VirtNWFilterToolBar()
{
    settings.beginGroup("VirtNWFilterControl");
    settings.setValue("UpdateTime", interval);
    settings.setValue("AutoReload", _autoReload->isChecked());
    settings.endGroup();
}

/* public slots */
Qt::ToolBarArea VirtNWFilterToolBar::get_ToolBarArea(int i) const
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
void VirtNWFilterToolBar::enableAutoReload()
{
    if ( _autoReload->isChecked() ) timerId = startTimer(interval*1000);
}
void VirtNWFilterToolBar::stopProcessing()
{
    if ( timerId ) {
        killTimer(timerId);
        timerId = 0;
    }
}
bool VirtNWFilterToolBar::getAutoReloadState() const
{
    return _autoReload->isChecked();
}

/* private slots */
void VirtNWFilterToolBar::timerEvent(QTimerEvent *event)
{
    int _timerId = event->timerId();
    //qDebug()<<_timerId<<timerId;
    if ( _timerId && timerId==_timerId ) {
        QStringList parameters;
        parameters << "reloadVirtNWFilter";
        emit execMethod(parameters);
    };
}
void VirtNWFilterToolBar::showMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act->menu()->isVisible() ) act->menu()->hide();
    else {
        act->menu()->show();
        act->menu()->move(QCursor::pos());
    };
}
void VirtNWFilterToolBar::detectTriggerredAction(QAction *action)
{
    QStringList parameters;
    if        ( action == define_Action ) {
        parameters << "defineVirtNWFilter";
    } else if ( action == undefine_Action ) {
        parameters << "undefineVirtNWFilter";
    //} else if ( action == getXMLDesc_Action ) {
    //    parameters << "getVirtNWFilterXMLDesc";
    } else return;
    emit execMethod(parameters);
}
void VirtNWFilterToolBar::changeAutoReloadState(bool state)
{
    if ( state ) enableAutoReload();
    else stopProcessing();
}
