#include "connections.h"

Connections::Connections(QWidget *parent) :
    QMainWindow(parent)
{
    toolBar = new ToolBar(this);
    toolBar->setObjectName("toolBar");
    list = new ConnectionList(this);
    list->setEnabled(false);
    setCentralWidget(list);
}
void Connections::setToolBarArea(int area_int)
{
    addToolBar(
                toolBar->get_ToolBarArea(area_int),
                toolBar);
}
