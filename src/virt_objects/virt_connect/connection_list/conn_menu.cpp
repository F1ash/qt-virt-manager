#include "conn_menu.h"

ConnectMenu::ConnectMenu(QWidget *parent)
    : QMenu(parent)
{
    act      = addAction(
                QIcon::fromTheme("connect"),
                "Open Connection");
    addSeparator();
    display  = addAction(
                QIcon::fromTheme("overview"),
                "Overview of Connection");
    edit     = addAction(
                QIcon::fromTheme("configure"),
                "Edit Connection");
    clean    = addAction(
                QIcon::fromTheme("list-remove"),
                "Delete Connection");
    addSeparator();
    refresh  = addAction(
                QIcon::fromTheme("view-refresh"),
                "Refresh Localhost Connection");
    setSeparatorsCollapsible(true);
}
