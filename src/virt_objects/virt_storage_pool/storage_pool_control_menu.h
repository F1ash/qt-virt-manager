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
    explicit StoragePoolControlMenu(
            QWidget     *parent = NULL,
            QStringList  params = QStringList(),
            bool         state  = false);

signals:
    void            execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *start = NULL;
    QAction        *destroy = NULL;
    QAction        *undefine = NULL;
    QAction        *autoStart = NULL;
    Delete_Pool_Menu
                    *delete_Menu = NULL;
    QAction        *delete_Action = NULL;
    QAction        *getXMLDesc = NULL;
    QAction        *overview = NULL;
    QAction        *reload = NULL;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);
};

#endif // STORAGE_POOL_CONTROL_MENU_H
