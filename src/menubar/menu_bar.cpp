#include "menu_bar.h"

MenuBar::MenuBar(QWidget *parent) :
    QMenuBar(parent)
{
    fileMenu = new FileMenu(this);
    viewMenu = new ViewMenu(this);
    dockMenu = new DockMenu(this);
    helpMenu = new HelpMenu(this);
    fileAct = addMenu(fileMenu);
    fileAct->setText("&File");
    viewAct = addMenu(viewMenu);
    viewAct->setText("&View Mode");
    dockAct = addMenu(dockMenu);
    dockAct->setText("&Virtual Entity Views");
    helpAct = addMenu(helpMenu);
    helpAct->setText("&Help");
}
