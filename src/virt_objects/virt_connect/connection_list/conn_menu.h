#ifndef JOBMENU_H
#define JOBMENU_H
#include <QMenu>

class ConnectionMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ConnectionMenu(QWidget *parent = nullptr);

    QAction  *edit;
    QAction  *display;
    QAction  *clean;
    QAction  *act;
    QAction  *refresh;
};
#endif
