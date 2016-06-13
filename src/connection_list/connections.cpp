#include "connections.h"

Connections::Connections(QWidget *parent) :
    QMainWindow(parent)
{
    toolBar = new ToolBar(this);
    toolBar->setObjectName("toolBar");
    list = new ConnectionList(this);
    setCentralWidget(list);
    show();
}
void Connections::setToolBarArea(int area_int)
{
    addToolBar(
                toolBar->get_ToolBarArea(area_int),
                toolBar);
}
