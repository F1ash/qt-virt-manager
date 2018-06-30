#include "conn_menu.h"

ConnectionMenu::ConnectionMenu(QWidget *parent)
    : QMenu(parent)
{
    act      = addAction(
                QIcon::fromTheme("connect"),
                tr("Open Connection"));
    addSeparator();
    display  = addAction(
                QIcon::fromTheme("overview"),
                tr("Overview of Connection"));
    edit     = addAction(
                QIcon::fromTheme("configure"),
                tr("Edit Connection"));
    clean    = addAction(
                QIcon::fromTheme("list-remove"),
                tr("Delete Connection"));
    addSeparator();
    refresh  = addAction(
                QIcon::fromTheme("view-refresh"),
                tr("Refresh Localhost Connection"));
    setSeparatorsCollapsible(true);
}
