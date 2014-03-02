#include "traywidget.h"

TrayIcon::TrayIcon(QWidget *parent = 0)
  : QSystemTrayIcon(parent)
{
  setIcon(QIcon::fromTheme("virtual-engineering"));
  hideAction = new QAction(QString("Down"), this);
  hideAction->setIcon (QIcon::fromTheme("down"));
  logUpAction = new QAction(QString("Show Log Viewer"), this);
  logUpAction->setIcon ( QIcon::fromTheme("utilities-log-viewer") );
  closeAction = new QAction(QString("Exit"), this);
  closeAction->setIcon (QIcon::fromTheme("exit"));

  trayIconMenu = new QMenu(parent);
  trayIconMenu->addAction(hideAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(logUpAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(closeAction);

  setToolTip(QString("Qt VirtManager"));
  setContextMenu(trayIconMenu);
  show();
}
TrayIcon::~TrayIcon()
{
  trayIconMenu->clear();
  delete trayIconMenu;
  trayIconMenu = 0;
  delete hideAction;
  hideAction = 0;
  delete logUpAction;
  logUpAction = 0;
  delete closeAction;
  closeAction = 0;
}
void TrayIcon::setLogUpActionText(QString &s)
{
    logUpAction->setText(s);
}
