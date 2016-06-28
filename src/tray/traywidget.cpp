#include "traywidget.h"

TrayIcon::TrayIcon(QWidget *parent)
  : QSystemTrayIcon(parent)
{
    _mark = false;
    timerId = 0;
    appIcon = QIcon::fromTheme("virtual-engineering");
    warningIcon = QIcon::fromTheme("dialog-warning");
    setIcon(appIcon);
    hideAction = new QAction(QString("Down"), this);
    hideAction->setIcon (QIcon::fromTheme("down"));
    //logUpAction = new QAction(QString("Show Log Viewer"), this);
    //logUpAction->setIcon ( QIcon::fromTheme("utilities-log-viewer") );
    monitorAction = new QAction(QString("Domains State Monitor"), this);
    monitorAction->setIcon ( QIcon::fromTheme("utilities-monitor") );
    taskUpAction = new QAction(QString("Task WareHouse"), this);
    taskUpAction->setIcon ( QIcon::fromTheme("job") );
    closeAction = new QAction(QString("Exit"), this);
    closeAction->setIcon (QIcon::fromTheme("exit"));

    trayIconMenu = new QMenu(parent);
    trayIconMenu->addAction(hideAction);
    trayIconMenu->addSeparator();
    //trayIconMenu->addAction(logUpAction);
    trayIconMenu->addAction(monitorAction);
    trayIconMenu->addAction(taskUpAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(closeAction);

    setToolTip(QString("Qt VirtManager"));
    setContextMenu(trayIconMenu);
    show();
}

/* public slots */
void TrayIcon::setLogUpActionText(QString &s)
{
    //logUpAction->setText(s);
}
void TrayIcon::stateMonitorVisibilityChanged(bool state)
{
    monitorAction->setText(
                QString("%1 Domains State Monitor")
                .arg( (state)? "Hide" : "Show" ));
}
void TrayIcon::stateTaskWareHouseVisibilityChanged(bool state)
{
    taskUpAction->setText(
                QString("%1 Task WareHouse")
                .arg( (state)? "Hide" : "Show" ));
}
void TrayIcon::changeWarningState(bool state)
{
    if ( state ) {
        if ( timerId==0 ) timerId = startTimer(1000);
    } else {
        if ( timerId>0 ) {
            killTimer(timerId);
            timerId = 0;
        };
        setIcon(appIcon);
    };
}

/* private slots */
void TrayIcon::timerEvent(QTimerEvent *ev)
{
    if ( timerId==ev->timerId() ) {
        QIcon icon = (_mark)? appIcon : warningIcon;
        setIcon(icon);
        _mark=!_mark;
    };
}
