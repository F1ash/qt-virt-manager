#ifndef STORAGE_VOL_CONTROL_MENU_H
#define STORAGE_VOL_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>
#include "wipe_menu.h"
#include "resize_dialog.h"
#include <QDebug>

class StorageVolControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit StorageVolControlMenu(
            QWidget     *parent = NULL,
            QStringList  params = QStringList(),
            bool         state  = false);

signals:
    void            execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *delete_Action = NULL;
    QAction        *download_Action = NULL;
    QAction        *resize_Action = NULL;
    QAction        *upload_Action = NULL;
    QAction        *wipe_Action = NULL;
    WipeMenu       *wipe_Menu = NULL;
    QAction        *getXMLDesc_Action = NULL;
    QAction        *reload = NULL;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);
};

#endif // STORAGE_VOL_CONTROL_MENU_H
