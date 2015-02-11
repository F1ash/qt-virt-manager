#ifndef VIEWER_TOOLBAR_H
#define VIEWER_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QMenu>

class ViewerToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ViewerToolBar(QWidget *parent = NULL);

signals:
    void             execMethod(const QStringList&);

private:
    QAction         *start_Action;
    QAction         *restore_Action;
    QMenu           *start_Menu;
    QAction         *destroy_Action;
    QMenu           *destroy_Menu;
    QAction         *save_Action;
    QAction         *sep;
    QAction         *pause_Action;
    QAction         *reboot_Action;
    QAction         *reset_Action;
    QAction         *shutdown_Action;

private slots:
    void             showMenu();
    void             detectTriggerredAction(QAction*);
};

#endif // VIEWER_TOOLBAR_H
