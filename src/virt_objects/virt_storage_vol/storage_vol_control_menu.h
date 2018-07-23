#ifndef STORAGE_VOL_CONTROL_MENU_H
#define STORAGE_VOL_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>
#include "wipe_menu.h"
#include "resize_dialog.h"
#include "virt_objects/virt_entity_enums.h"
//#include <QDebug>

class StorageVolControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit StorageVolControlMenu(
            QWidget     *parent = Q_NULLPTR,
            QStringList  params = QStringList(),
            bool         state  = false);

signals:
    void            execMethod(const Act_Param&);

private:
    bool            autoReloadState;
    QAction        *delete_Action = Q_NULLPTR;
    QAction        *download_Action = Q_NULLPTR;
    QAction        *resize_Action = Q_NULLPTR;
    QAction        *upload_Action = Q_NULLPTR;
    QAction        *wipe_Action = Q_NULLPTR;
    WipeMenu       *wipe_Menu = Q_NULLPTR;
    QAction        *getXMLDesc_Action = Q_NULLPTR;
    QAction        *reload = Q_NULLPTR;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);
};

#endif // STORAGE_VOL_CONTROL_MENU_H
