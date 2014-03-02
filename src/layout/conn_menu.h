#ifndef JOBMENU_H
#define JOBMENU_H
#include <QMenu>

class ConnectMenu : public QMenu
{
    Q_OBJECT
public:
    ConnectMenu(QWidget *parent = 0);
    ~ConnectMenu();

    QAction  *edit;
    QAction  *display;
    QAction  *clean;
    QAction  *act;

signals:

public slots:

private:

private slots:

};
#endif
