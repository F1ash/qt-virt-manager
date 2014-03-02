#include "virtnet_toolbar.h"

VirtNetToolBar::VirtNetToolBar(QWidget *parent) :
    QToolBar(parent)
{
    start_Action = new QAction(this);
    start_Action->setIcon(QIcon::fromTheme("network-start"));
    start_Action->setToolTip("Start");
    destroy_Action = new QAction(this);
    destroy_Action->setIcon(QIcon::fromTheme("network-stop"));
    destroy_Action->setToolTip("Stop");
    create_Action = new QAction(this);
    create_Action->setIcon(QIcon::fromTheme("network-create"));
    create_Action->setToolTip("Create");
    create_Menu = new OpenFileMenu(this, "create");
    create_Action->setMenu(create_Menu);
    define_Action = new QAction(this);
    define_Action->setIcon(QIcon::fromTheme("network-define"));
    define_Action->setToolTip("Define");
    define_Menu = new OpenFileMenu(this, "define");
    define_Action->setMenu(define_Menu);
    undefine_Action = new QAction(this);
    undefine_Action->setIcon(QIcon::fromTheme("network-undefine"));
    undefine_Action->setToolTip("Undefine");
    setAutostart_Action = new QAction(this);
    setAutostart_Action->setIcon(QIcon::fromTheme("network-autostart"));
    setAutostart_Action->setToolTip("Change AutoStart State");

    addAction(start_Action);
    addAction(destroy_Action);
    addSeparator();
    addAction(create_Action);
    addAction(define_Action);
    addAction(undefine_Action);
    addSeparator();
    addAction(setAutostart_Action);

    connect(start_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    connect(destroy_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    connect(create_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    connect(define_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    connect(undefine_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    connect(setAutostart_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));

    connect(create_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    connect(define_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    connect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(detectTriggerredAction(QAction*)));
}
VirtNetToolBar::~VirtNetToolBar()
{
    disconnect(start_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    disconnect(destroy_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    disconnect(create_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    disconnect(define_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    disconnect(undefine_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    disconnect(setAutostart_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    disconnect(create_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    disconnect(define_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    disconnect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(detectTriggerredAction(QAction*)));
    delete start_Action;
    start_Action = 0;
    delete destroy_Action;
    destroy_Action = 0;
    delete create_Menu;
    create_Menu = 0;
    delete define_Menu;
    define_Menu = 0;
    delete create_Action;
    create_Action = 0;
    delete define_Action;
    define_Action = 0;
    delete undefine_Action;
    undefine_Action = 0;
    delete setAutostart_Action;
    setAutostart_Action = 0;
}

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
void VirtNetToolBar::repeatParameters(QStringList &p)
{
    emit fileForMethod(p);
}
void VirtNetToolBar::showHoveredMenu()
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
void VirtNetToolBar::detectTriggerredAction(QAction *action)
{
    QStringList parameters;
    if ( action == start_Action) {
        parameters << "startVirtNetwork";
    } else if ( action == destroy_Action ) {
        parameters << "destroyVirtNetwork";
    } else if ( action == undefine_Action ) {
        parameters << "undefineVirtNetwork";
    } else if ( action == setAutostart_Action ) {
        parameters << "setAutostartVirtNetwork";
    } else return;
    emit execMethod(parameters);
}
