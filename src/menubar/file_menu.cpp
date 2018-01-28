#include "file_menu.h"

FileMenu::FileMenu(QWidget *parent) :
    QMenu(parent)
{
    newConn = addAction(
                QIcon::fromTheme("connection-wizard"),
                tr("&New connection"));
    newConn->setShortcut(QKeySequence(tr("Ctrl+N")));
    hideToTray = addAction(
                QIcon::fromTheme("down"),
                tr("&Hide to tray"));
    restart = addAction(
                QIcon::fromTheme("view-refresh"),
                tr("&Restart"));
    restart->setShortcut(QKeySequence(tr("Ctrl+R")));
    addSeparator();
    exitApp = addAction(
                QIcon::fromTheme("exit"),
                tr("&Exit"));
    exitApp->setShortcut(QKeySequence(tr("Ctrl+Q")));
}
