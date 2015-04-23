#include "conn_menu.h"

ConnectMenu::ConnectMenu(QWidget *parent)
    : QMenu(parent)
{
  edit     = addAction(QIcon::fromTheme("configure"), "Edit Connection");
  display  = addAction(QIcon::fromTheme("overview"), "Overview Connection");
  clean    = addAction(QIcon::fromTheme("clean"), "Delete Connection");
  act      = addAction(QIcon::fromTheme("run"), "Open Connection");
  setTitle("Connection Control");
  setSeparatorsCollapsible(true);
}
ConnectMenu::~ConnectMenu()
{
  delete edit;
  edit = NULL;
  delete display;
  display = NULL;
  delete clean;
  clean = NULL;
  delete act;
  act = NULL;
}
