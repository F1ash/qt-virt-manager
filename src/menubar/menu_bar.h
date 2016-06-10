#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <QMenuBar>
#include "file_menu.h"
#include "dock_menu.h"
#include "help_menu.h"

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = nullptr);
    FileMenu       *fileMenu;
    DockMenu       *dockMenu;
    HelpMenu       *helpMenu;
    QAction        *fileAct;
    QAction        *dockAct;
    QAction        *helpAct;
};

#endif // MENU_BAR_H
