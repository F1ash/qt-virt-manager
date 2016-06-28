#ifndef TRAY_WIDGET_H
#define TRAY_WIDGET_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimerEvent>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public :
  explicit TrayIcon(QWidget *parent = nullptr);

    QAction   *hideAction;
    //QAction   *logUpAction;
    QAction   *monitorAction;
    QAction   *taskUpAction;
    QAction   *closeAction;

private :
    int        timerId;
    bool       _mark;
    QMenu     *trayIconMenu;
    QIcon      appIcon, warningIcon;

public slots:
    void       setLogUpActionText(QString&);
    void       stateMonitorVisibilityChanged(bool);
    void       stateTaskWareHouseVisibilityChanged(bool);
    void       changeWarningState(bool);

private slots:
    void       timerEvent(QTimerEvent*);
};

#endif // TRAY_WIDGET_H
