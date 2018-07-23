#ifndef DOCK_MENU_H
#define DOCK_MENU_H

#include <QMenu>

class DockMenu : public QMenu
{
    Q_OBJECT
public:
    explicit DockMenu(QWidget *parent = Q_NULLPTR);
    QAction         *logAct;
    QAction         *domainAct;
    QAction         *networkAct;
    QAction         *ifaceAct;
    QAction         *secretAct;
    QAction         *storageAct;
    QAction         *nwfilterAct;
};

#endif // DOCK_MENU_H
