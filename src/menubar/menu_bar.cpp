#include "menu_bar.h"

MenuBar::MenuBar(QWidget *parent) :
    QMenuBar(parent)
{
    fileMenu = new FileMenu(this);
    dockMenu = new DockMenu(this);
    helpMenu = new HelpMenu(this);
    fileAct = addMenu(fileMenu);
    fileAct->setText("&File");
    dockAct = addMenu(dockMenu);
    dockAct->setText("&Virtual Entity Views");
    helpAct = addMenu(helpMenu);
    helpAct->setText("&Help");
}
