#include "traywidget.h"

TrayIcon::TrayIcon(QWidget *parent = 0)
  : QSystemTrayIcon(parent)
{
  setIcon(QIcon::fromTheme("virtual-engineering"));
  hideAction = new QAction(QString("Down"), this);
  hideAction->setIcon (QIcon::fromTheme("down"));
  logUpAction = new QAction(QString("Show Log Viewer"), this);
  logUpAction->setIcon ( QIcon::fromTheme("utilities-log-viewer") );
  monitorAction = new QAction(QString("Domains State Monitor "), this);
  monitorAction->setIcon ( QIcon::fromTheme("utilities-log-viewer") );
  closeAction = new QAction(QString("Exit"), this);
  closeAction->setIcon (QIcon::fromTheme("exit"));

  trayIconMenu = new QMenu(parent);
  trayIconMenu->addAction(hideAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(logUpAction);
  trayIconMenu->addAction(monitorAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(closeAction);

  setToolTip(QString("Qt VirtManager"));
  setContextMenu(trayIconMenu);
  show();
}

void TrayIcon::setLogUpActionText(QString &s)
{
    logUpAction->setText(s);
}
