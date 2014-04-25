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
    explicit StorageVolControlMenu(QWidget *parent = 0, QStringList params = QStringList(), bool state = false);
    ~StorageVolControlMenu();

signals:
    void execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *delete_Action;
    QAction        *download_Action;
    QAction        *resize_Action;
    QAction        *upload_Action;
    QAction        *wipe_Action;
    WipeMenu       *wipe_Menu;
    QAction        *getXMLDesc_Action;
    QAction        *reload;
    QStringList     parameters;

private slots:
    void emitExecMethod(QAction*);
    void emitWipeAction(const QStringList&);

};

#endif // STORAGE_VOL_CONTROL_MENU_H
