#include "toolbar.h"

ToolBar::ToolBar (QWidget *parent = 0) : QToolBar(parent)
{
  setAllowedAreas(Qt::AllToolBarAreas);
  setMovable(true);
  setFloatable(true);

  initActions();
}
ToolBar::~ToolBar()
{
  //disconnect(itemControlAction, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
  //disconnect(_hideAction, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
  //disconnect(_docsUpAction, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
  //disconnect(_exitAction, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));

    disconnect(itemControlAction, SIGNAL(triggered()), this, SLOT(showMenu()));
    disconnect(_docsUpAction, SIGNAL(triggered()), this, SLOT(showMenu()));

  delete _hideAction;
  _hideAction = 0;
  delete _exitAction;
  _exitAction = 0;

  delete _createAction;
  _createAction = 0;
  delete _deleteAction;
  _deleteAction = 0;
  delete _editAction;
  _editAction = 0;
  delete _openAction;
  _openAction = 0;
  delete _showAction;
  _showAction = 0;
  delete _closeAction;
  _closeAction = 0;
  delete _closeAllAction;
  _closeAllAction = 0;
  delete itemControlMenu;
  itemControlMenu = 0;
  delete itemControlAction;
  itemControlAction = 0;

  delete _logUpAction;
  _logUpAction = 0;
  delete _domUpAction;
  _domUpAction = 0;
  delete _netUpAction;
  _netUpAction = 0;
  delete _stVolUpAction;
  _stVolUpAction = 0;
  delete _stPoolUpAction;
  _stPoolUpAction = 0;
  delete showDocksControlMenu;
  showDocksControlMenu = 0;
  delete _docsUpAction;
  _docsUpAction = 0;
}
void ToolBar::initActions()
{
    _hideAction = new QAction(QString("Hide to tray"), this);
    _hideAction->setIcon ( QIcon::fromTheme("down") );
    _exitAction = new QAction(QString("Exit"), this);
    _exitAction->setIcon ( QIcon::fromTheme("exit") );

    addAction(_hideAction);
    addSeparator();
    addItemControlMenu();
    addSeparator();
    addDocksControlMenu();
    addSeparator();
    addAction(_exitAction);

    //connect(_hideAction, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(_exitAction, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
}
void ToolBar::addItemControlMenu()
{
    itemControlMenu = new QMenu(this);

    _createAction = new QAction(QString("Create new Connect"), this);
    _createAction->setIcon ( QIcon::fromTheme("install") );
    _editAction = new QAction(QString("Edit selected Connect"), this);
    _editAction->setIcon ( QIcon::fromTheme("configure") );
    _deleteAction = new QAction(QString("Delete selected Connect"), this);
    _deleteAction->setIcon ( QIcon::fromTheme("clean") );
    _openAction = new QAction(QString("Open selected Connect"), this);
    _openAction->setIcon ( QIcon::fromTheme("run") );
    _showAction = new QAction(QString("Overview selected Connect"), this);
    _showAction->setIcon ( QIcon::fromTheme("overview") );
    _closeAction = new QAction(QString("Close selected Connect"), this);
    _closeAction->setIcon ( QIcon::fromTheme("stop") );
    _closeAllAction = new QAction(QString("Close all Connect"), this);
    _closeAllAction->setIcon ( QIcon::fromTheme("stop-all") );

    itemControlMenu->addAction(_createAction);
    itemControlMenu->addAction(_editAction);
    itemControlMenu->addAction(_deleteAction);
    itemControlMenu->addSeparator();
    itemControlMenu->addAction(_openAction);
    itemControlMenu->addAction(_showAction);
    itemControlMenu->addAction(_closeAction);
    itemControlMenu->addAction(_closeAllAction);

    itemControlAction = new QAction(this);
    itemControlAction->setIcon(QIcon::fromTheme("job"));
    itemControlAction->setToolTip("Connect Control");
    itemControlAction->setMenu(itemControlMenu);
    addAction( itemControlAction );
    //connect(itemControlAction, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    connect(itemControlAction, SIGNAL(triggered()), this, SLOT(showMenu()));
}
void ToolBar::addDocksControlMenu()
{
    showDocksControlMenu = new QMenu(this);

    _logUpAction = new QAction("Log Dock", this);
    _logUpAction->setCheckable(true);
    _domUpAction = new QAction("Domain Dock", this);
    _domUpAction->setCheckable(true);
    _netUpAction = new QAction("Network Dock", this);
    _netUpAction->setCheckable(true);
    _stVolUpAction = new QAction("StorageVol Dock", this);
    _stVolUpAction->setCheckable(true);
    _stPoolUpAction = new QAction("StoragePool Dock", this);
    _stPoolUpAction->setCheckable(true);

    showDocksControlMenu->addAction(_logUpAction);
    showDocksControlMenu->addSeparator();
    showDocksControlMenu->addAction(_domUpAction);
    showDocksControlMenu->addAction(_netUpAction);
    showDocksControlMenu->addAction(_stVolUpAction);
    showDocksControlMenu->addAction(_stPoolUpAction);

    _docsUpAction = new QAction(QString("Dock Control"), this);
    _docsUpAction->setIcon ( QIcon::fromTheme("utilities-log-viewer") );
    _docsUpAction->setMenu(showDocksControlMenu);
    addAction( _docsUpAction );
    //connect(_docsUpAction, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    connect(_docsUpAction, SIGNAL(triggered()), this, SLOT(showMenu()));
}
void ToolBar::mainWindowUp()
{
    emit warningShowed();
}
void ToolBar::showHoveredMenu()
{
    if ( !isEnabled() ) return;
    QAction *act = static_cast<QAction*>(sender());
    if ( act==itemControlAction ) {
        if ( _docsUpAction->menu()->isVisible() )
            _docsUpAction->menu()->hide();
    } else if ( act==_docsUpAction ) {
        if ( itemControlAction->menu()->isVisible() )
            itemControlAction->menu()->hide();
    }  else {
        if ( itemControlAction->menu()->isVisible() )
            itemControlAction->menu()->hide();
        if ( _docsUpAction->menu()->isVisible() )
            _docsUpAction->menu()->hide();
        return;
    };
    //act->menu()->popup(mapToGlobal(this->pos()), act);
    act->menu()->show();
    act->menu()->move(QCursor::pos());
}
void ToolBar::showMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act->menu()->isVisible() ) act->menu()->hide();
    else {
        act->menu()->show();
        act->menu()->move(QCursor::pos());
    };
}

Qt::ToolBarArea ToolBar::get_ToolBarArea(int i) const
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
