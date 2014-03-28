#include "storage_vol_toolbar.h"

StorageVolToolBar::StorageVolToolBar(QWidget *parent) :
    QToolBar(parent)
{
    start_Action = new QAction(this);
    start_Action->setIcon(QIcon::fromTheme("storageVol-start"));
    start_Action->setToolTip("Start");
    destroy_Action = new QAction(this);
    destroy_Action->setIcon(QIcon::fromTheme("storageVol-stop"));
    destroy_Action->setToolTip("Stop");
    create_Action = new QAction(this);
    create_Action->setIcon(QIcon::fromTheme("storageVol-create"));
    create_Action->setToolTip("Create");
    create_Menu = new OpenFileMenu(this, "create", "StorageVol");
    create_Action->setMenu(create_Menu);
    connect(create_Action, SIGNAL(triggered()), this, SLOT(showMenu()));
    define_Action = new QAction(this);
    define_Action->setIcon(QIcon::fromTheme("storageVol-define"));
    define_Action->setToolTip("Define");
    define_Menu = new OpenFileMenu(this, "define", "StorageVol");
    define_Action->setMenu(define_Menu);
    connect(define_Action, SIGNAL(triggered()), this, SLOT(showMenu()));
    undefine_Action = new QAction(this);
    undefine_Action->setIcon(QIcon::fromTheme("storageVol-undefine"));
    undefine_Action->setToolTip("Undefine");
    setAutostart_Action = new QAction(this);
    setAutostart_Action->setIcon(QIcon::fromTheme("storageVol-autostart"));
    setAutostart_Action->setToolTip("Change AutoStart State");
    getXMLDesc_Action = new QAction(this);
    getXMLDesc_Action->setIcon(QIcon::fromTheme("storageVol-xml"));
    getXMLDesc_Action->setToolTip("Get XML Description");
    stopOverview_Action = new QAction(this);
    stopOverview_Action->setIcon(QIcon::fromTheme("overview-stop"));
    stopOverview_Action->setToolTip("Close Pool Overview");

    addAction(start_Action);
    addAction(destroy_Action);
    addSeparator();
    addAction(create_Action);
    addAction(define_Action);
    addAction(undefine_Action);
    //addSeparator();
    addAction(setAutostart_Action);
    addSeparator();
    addAction(getXMLDesc_Action);
    addSeparator();
    addAction(stopOverview_Action);

    //connect(start_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(destroy_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(create_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(define_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(undefine_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(setAutostart_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(getXMLDesc_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));

    connect(create_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    connect(define_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    connect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(detectTriggerredAction(QAction*)));
}
StorageVolToolBar::~StorageVolToolBar()
{
    //disconnect(start_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //disconnect(destroy_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //disconnect(create_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //disconnect(define_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //disconnect(undefine_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //disconnect(setAutostart_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //disconnect(getXMLDesc_Action, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    disconnect(create_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    disconnect(define_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    disconnect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(detectTriggerredAction(QAction*)));

    disconnect(define_Action, SIGNAL(triggered()), this, SLOT(showMenu()));
    disconnect(create_Action, SIGNAL(triggered()), this, SLOT(showMenu()));

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
    delete getXMLDesc_Action;
    getXMLDesc_Action = 0;
    delete stopOverview_Action;
    stopOverview_Action = 0;
}

Qt::ToolBarArea StorageVolToolBar::get_ToolBarArea(int i) const
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
void StorageVolToolBar::repeatParameters(QStringList &p)
{
    emit fileForMethod(p);
}
void StorageVolToolBar::showHoveredMenu()
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
void StorageVolToolBar::showMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act->menu()->isVisible() ) act->menu()->hide();
    else {
        act->menu()->show();
        act->menu()->move(QCursor::pos());
    };
}
void StorageVolToolBar::detectTriggerredAction(QAction *action)
{
    QStringList parameters;
    if ( action == start_Action) {
        parameters << "downloadVirtStorageVolList";
    } else if ( action == destroy_Action ) {
        parameters << "deleteVirtStorageVol";
    } else if ( action == undefine_Action ) {
        parameters << "resizeVirtStorageVol";
    } else if ( action == setAutostart_Action ) {
        parameters << "uploadVirtStorageVolList";
    } else if ( action == getXMLDesc_Action ) {
        parameters << "getVirtStorageVolXMLDesc";
    } else if ( action == stopOverview_Action ) {
        parameters << "stopOverViewVirtStoragePool";
    } else return;
    emit execMethod(parameters);
}
