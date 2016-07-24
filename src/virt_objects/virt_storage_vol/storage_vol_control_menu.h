#ifndef STORAGE_VOL_CONTROL_MENU_H
#define STORAGE_VOL_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>
#include "wipe_menu.h"
#include "resize_dialog.h"
#include "virt_objects/virt_entity_enums.h"
#include <QDebug>

class StorageVolControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit StorageVolControlMenu(
            QWidget     *parent = nullptr,
            QStringList  params = QStringList(),
            bool         state  = false);

signals:
    void            execMethod(const Act_Param&);

private:
    bool            autoReloadState;
    QAction        *delete_Action = nullptr;
    QAction        *download_Action = nullptr;
    QAction        *resize_Action = nullptr;
    QAction        *upload_Action = nullptr;
    QAction        *wipe_Action = nullptr;
    WipeMenu       *wipe_Menu = nullptr;
    QAction        *getXMLDesc_Action = nullptr;
    QAction        *reload = nullptr;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);
};

#endif // STORAGE_VOL_CONTROL_MENU_H
