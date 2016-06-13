#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QAction>

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);

    QAction    *_createAction;
    QAction    *_editAction;
    QAction    *_deleteAction;
    QAction    *_openAction;
    QAction    *_showAction;
    QAction    *_closeAction;
    QAction    *_closeAllAction;
    QAction    *_closeOverview;

  void          initActions();

public slots:
    Qt::ToolBarArea get_ToolBarArea(int) const;
};
#endif   // TOOLBAR_H
