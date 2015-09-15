#include "conn_menu.h"

ConnectMenu::ConnectMenu(QWidget *parent)
    : QMenu(parent)
{
    edit     = addAction(QIcon::fromTheme("configure"), "Edit Connection");
    display  = addAction(QIcon::fromTheme("overview"), "Overview Connection");
    clean    = addAction(QIcon::fromTheme("clean"), "Delete Connection");
    act      = addAction(QIcon::fromTheme("run"), "Open Connection");
    addSeparator();
    refresh  = addAction(QIcon::fromTheme("view-refresh"), "Refresh Localhost Connection");
    setTitle("Connection Control");
    setSeparatorsCollapsible(true);
}
