#ifndef TRAYWIDGET_H
#define TRAYWIDGET_H

#include <QSystemTrayIcon>
#include <QMenu>

class TrayIcon : public QSystemTrayIcon
{
  Q_OBJECT
public :
  TrayIcon(QWidget *parent);
  ~TrayIcon();

  QAction *hideAction;
  QAction *logUpAction;
  QAction *closeAction;

private :
  QMenu *trayIconMenu;

public slots:
  void setLogUpActionText(QString&);
};

#endif
