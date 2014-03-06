#include "conn_menu.h"

ConnectMenu::ConnectMenu(QWidget *parent)
    : QMenu(parent)
{
  edit     = addAction(QIcon::fromTheme("configure"), "Edit Connect");
  display  = addAction(QIcon::fromTheme("utilities-log-viewer"), "Overview Connect");
  clean    = addAction(QIcon::fromTheme("clean"), "Delete Connect");
  act      = addAction(QIcon::fromTheme("run"), "Open Connect");
  setTitle("Connect Control");
  setSeparatorsCollapsible(true);
}
ConnectMenu::~ConnectMenu()
{
  delete edit;
  edit = 0;
  delete display;
  display = 0;
  delete clean;
  clean = 0;
  delete act;
  act = 0;
}
