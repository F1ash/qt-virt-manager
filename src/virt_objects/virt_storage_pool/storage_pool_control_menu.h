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
            QWidget     *parent = nullptr,
            QStringList  params = QStringList(),
            bool         state  = false);

signals:
    void            execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *start = nullptr;
    QAction        *destroy = nullptr;
    QAction        *undefine = nullptr;
    QAction        *autoStart = nullptr;
    Delete_Pool_Menu
                    *delete_Menu = nullptr;
    QAction        *delete_Action = nullptr;
    QAction        *getXMLDesc = nullptr;
    QAction        *overview = nullptr;
    QAction        *reload = nullptr;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);
};

#endif // STORAGE_POOL_CONTROL_MENU_H
