#ifndef LOG_DOCK_MENU_H
#define LOG_DOCK_MENU_H

#include <QMenu>
#include <QAction>

class LogDockMenu : public QMenu
{
    Q_OBJECT
public:
    explicit LogDockMenu(QWidget *parent = NULL);
    QAction         *saveAtExit, *useNameTemplate;
};

#endif // LOG_DOCK_MENU_H
