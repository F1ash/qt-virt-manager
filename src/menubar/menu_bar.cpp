#include "menu_bar.h"

MenuBar::MenuBar(QWidget *parent) :
    QMenuBar(parent)
{
    fileMenu = new FileMenu(this);
    viewMenu = new ViewMenu(this);
    dockMenu = new DockMenu(this);
    helpMenu = new HelpMenu(this);
    fileAct = addMenu(fileMenu);
    fileAct->setText(tr("&File"));
    viewAct = addMenu(viewMenu);
    viewAct->setText(tr("View &Mode"));
    dockAct = addMenu(dockMenu);
    dockAct->setText(tr("&Virtual Entity Views"));
    helpAct = addMenu(helpMenu);
    helpAct->setText(tr("&Help"));
}
