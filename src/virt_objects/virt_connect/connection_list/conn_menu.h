#ifndef JOBMENU_H
#define JOBMENU_H
#include <QMenu>

class ConnectMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ConnectMenu(QWidget *parent = nullptr);

    QAction  *edit;
    QAction  *display;
    QAction  *clean;
    QAction  *act;
    QAction  *refresh;
};
#endif
