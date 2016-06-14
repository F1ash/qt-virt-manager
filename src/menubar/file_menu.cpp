#include "file_menu.h"

FileMenu::FileMenu(QWidget *parent) :
    QMenu(parent)
{
    newConn = addAction(
                QIcon::fromTheme("connection-wizard"),
                "&New connection");
    newConn->setShortcut(QKeySequence(tr("Ctrl+N")));
    hideToTray = addAction(
                QIcon::fromTheme("down"),
                "&Hide to tray");
    restart = addAction(
                QIcon::fromTheme("view-refresh"),
                "&Restart");
    restart->setShortcut(QKeySequence(tr("Ctrl+R")));
    addSeparator();
    exitApp = addAction(
                QIcon::fromTheme("exit"),
                "&Exit");
    exitApp->setShortcut(QKeySequence(tr("Ctrl+Q")));
}
