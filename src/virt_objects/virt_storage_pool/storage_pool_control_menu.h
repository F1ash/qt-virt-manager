#ifndef STORAGE_POOL_CONTROL_MENU_H
#define STORAGE_POOL_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>
#include "delete_pool_menu.h"

class StoragePoolControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit StoragePoolControlMenu(QWidget *parent = NULL, QStringList params = QStringList(), bool state = false);
    ~StoragePoolControlMenu();

signals:
    void            execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *start;
    QAction        *destroy;
    QAction        *undefine;
    QAction        *autoStart;
    Delete_Pool_Menu
                    *delete_Menu;
    QAction        *delete_Action;
    QAction        *getXMLDesc;
    QAction        *overview;
    QAction        *reload;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);
};

#endif // STORAGE_POOL_CONTROL_MENU_H
