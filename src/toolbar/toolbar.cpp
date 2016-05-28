#include "toolbar.h"

ToolBar::ToolBar (
        QWidget *parent = nullptr,
        bool     again  = true) :
    QToolBar(parent), showDonate(again)
{
    setAllowedAreas(Qt::AllToolBarAreas);
    setMovable(true);
    setFloatable(true);

    initActions();
}
void ToolBar::initActions()
{
    _hideAction = new QAction(QString("Hide to tray"), this);
    _hideAction->setIcon ( QIcon::fromTheme("down") );
    _closeOverview = new QAction(QString("Close Overview"), this);
    _closeOverview->setIcon ( QIcon::fromTheme("overview-stop") );
    _exitAction = new QAction(QString("Exit"), this);
    _exitAction->setIcon ( QIcon::fromTheme("exit") );
    _infoAction = new QAction(QString("About"), this);
    _infoAction->setIcon( QIcon::fromTheme("info") );
    _donateAction = new QAction(QString("Donate"), this);
    _donateAction->setIcon(QIcon::fromTheme("donate"));
    _donateAction->setVisible(showDonate);

    addAction(_hideAction);
    addSeparator();
    addItemControlMenu();
    addSeparator();
    addDocksControlMenu();
    addSeparator();
    addAction(_closeOverview);
    addSeparator();
    addAction(_infoAction);
    addAction(_donateAction);
    addSeparator();
    addAction(_exitAction);

    //connect(_hideAction, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
    //connect(_exitAction, SIGNAL(hovered()), this, SLOT(showHoveredMenu()));
}
void ToolBar::addItemControlMenu()
{
    itemControlMenu = new QMenu(this);

    _createAction = new QAction(QString("New Connection"), this);
    _createAction->setIcon ( QIcon::fromTheme("list-add") );
    _editAction = new QAction(QString("Edit selected Connection"), this);
    _editAction->setIcon ( QIcon::fromTheme("configure") );
    _deleteAction = new QAction(QString("Delete selected Connection"), this);
    _deleteAction->setIcon ( QIcon::fromTheme("list-remove") );
    _openAction = new QAction(QString("Open selected Connection"), this);
    _openAction->setIcon ( QIcon::fromTheme("connect") );
    _showAction = new QAction(QString("Overview selected Connection"), this);
    _showAction->setIcon ( QIcon::fromTheme("overview") );
    _closeAction = new QAction(QString("Close selected Connection"), this);
    _closeAction->setIcon ( QIcon::fromTheme("disconnect") );
    _closeAllAction = new QAction(QString("Close all Connection"), this);
    _closeAllAction->setIcon ( QIcon::fromTheme("disconnect") );

    itemControlMenu->addAction(_createAction);
    itemControlMenu->addSeparator();
    itemControlMenu->addAction(_openAction);
    itemControlMenu->addAction(_showAction);
    itemControlMenu->addAction(_closeAction);
    itemControlMenu->addAction(_closeAllAction);
    itemControlMenu->addSeparator();
    itemControlMenu->addAction(_editAction);
    itemControlMenu->addAction(_deleteAction);

    itemControlAction = new QAction(this);
    itemControlAction->setIcon(QIcon::fromTheme("job"));
    itemControlAction->setToolTip("Connection Control");
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
    _storageUpAction = new QAction("Storage Dock", this);
    _storageUpAction->setCheckable(true);
    _secretsUpAction = new QAction("Secret Dock", this);
    _secretsUpAction->setCheckable(true);
    _ifaceUpAction = new QAction("Interface Dock", this);
    _ifaceUpAction->setCheckable(true);

    showDocksControlMenu->addAction(_logUpAction);
    showDocksControlMenu->addSeparator();
    showDocksControlMenu->addAction(_domUpAction);
    showDocksControlMenu->addAction(_netUpAction);
    showDocksControlMenu->addAction(_storageUpAction);
    showDocksControlMenu->addAction(_secretsUpAction);
    showDocksControlMenu->addAction(_ifaceUpAction);

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
