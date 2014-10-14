#include "conn_menu.h"

ConnectMenu::ConnectMenu(QWidget *parent)
    : QMenu(parent)
{
  edit     = addAction(QIcon::fromTheme("configure"), "Edit Connect");
  display  = addAction(QIcon::fromTheme("overview"), "Overview Connect");
  clean    = addAction(QIcon::fromTheme("clean"), "Delete Connect");
  act      = addAction(QIcon::fromTheme("run"), "Open Connect");
  setTitle("Connect Control");
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
