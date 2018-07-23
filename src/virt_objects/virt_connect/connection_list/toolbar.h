#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QWheelEvent>

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = Q_NULLPTR);

    QAction    *_createAction;
    QAction    *_editAction;
    QAction    *_deleteAction;
    QAction    *_openAction;
    QAction    *_overwiewAction;
    QAction    *_closeAction;
    QAction    *_closeAllAction;
    QAction    *_closeOverview;

    void        initActions();
    void        wheelEventEnabled(bool);

signals:
    void        viewNextDock();
    void        viewPrevDock();

private:
    bool        wheelEventState;

public slots:
    Qt::ToolBarArea get_ToolBarArea(int) const;

private slots:
    void            wheelEvent(QWheelEvent*);
};
#endif   // TOOLBAR_H
