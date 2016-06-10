#include "toolbar.h"

ToolBar::ToolBar (QWidget *parent) :
    QToolBar(parent)
{
    setAllowedAreas(Qt::AllToolBarAreas);
    setMovable(true);
    setFloatable(true);

    initActions();
}
void ToolBar::initActions()
{
    _createAction = new QAction(
                QString("New Connection"), this);
    _createAction->setIcon ( QIcon::fromTheme("list-add") );
    _editAction = new QAction(
                QString("Edit Connection"), this);
    _editAction->setIcon ( QIcon::fromTheme("configure") );
    _deleteAction = new QAction(
                QString("Delete Connection"), this);
    _deleteAction->setIcon ( QIcon::fromTheme("list-remove") );
    _openAction = new QAction(
                QString("Open Connection"), this);
    _openAction->setIcon ( QIcon::fromTheme("connect") );
    _closeAction = new QAction(
                QString("Close Connection"), this);
    _closeAction->setIcon ( QIcon::fromTheme("disconnect") );
    _closeAllAction = new QAction(
                QString("Close all Connections"), this);
    _closeAllAction->setIcon ( QIcon::fromTheme("disconnect") );
    _showAction = new QAction(
                QString("Overview Connection"), this);
    _showAction->setIcon ( QIcon::fromTheme("overview") );
    _closeOverview = new QAction(
                QString("Close Overview"), this);
    _closeOverview->setIcon ( QIcon::fromTheme("overview-stop") );

    addAction(_createAction);
    addAction(_editAction);
    addAction(_deleteAction);
    addSeparator();
    addAction(_openAction);
    addAction(_closeAction);
    addAction(_closeAllAction);
    addSeparator();
    addAction(_showAction);
    addAction(_closeOverview);
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
