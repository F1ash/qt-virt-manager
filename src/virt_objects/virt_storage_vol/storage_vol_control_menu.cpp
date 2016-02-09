#include "storage_vol_control_menu.h"

StorageVolControlMenu::StorageVolControlMenu(
        QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    if ( !parameters.isEmpty() ) {
        delete_Action = new QAction("Delete", this);
        delete_Action->setIcon(QIcon::fromTheme("delete"));
        download_Action = new QAction("Download", this);
        download_Action->setIcon(QIcon::fromTheme("download"));
        resize_Action = new QAction("Resize", this);
        resize_Action->setIcon(QIcon::fromTheme("resize"));
        upload_Action = new QAction("Upload", this);
        upload_Action->setIcon(QIcon::fromTheme("upload"));
        wipe_Menu = new WipeMenu(this);
        wipe_Action = new QAction("Wipe", this);
        wipe_Action->setIcon(QIcon::fromTheme("wipe"));
        wipe_Action->setMenu(wipe_Menu);
        connect(wipe_Menu, SIGNAL(execMethod(const QStringList&)),
                this, SIGNAL(execMethod(const QStringList&)));
        getXMLDesc_Action = new QAction("get XML Description", this);
        getXMLDesc_Action->setIcon(QIcon::fromTheme("application-xml"));

        addAction(delete_Action);
        addSeparator();
        addAction(download_Action);
        addAction(resize_Action);
        addAction(upload_Action);
        addAction(wipe_Action);
        addSeparator();
        addAction(getXMLDesc_Action);
        addSeparator();
    };
    reload = new QAction("Reload Volume OverView", this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
}

void StorageVolControlMenu::emitExecMethod(QAction *action)
{
    QStringList paramList;
    if ( !parameters.isEmpty() ) {
        if ( action == resize_Action) {
            paramList.append("resizeVirtStorageVol");
        } else if ( action == delete_Action ) {
            paramList.append("deleteVirtStorageVol");
        } else if ( action == download_Action ) {
            paramList.append("downloadVirtStorageVol");
        } else if ( action == upload_Action ) {
            paramList.append("uploadVirtStorageVol");
        } else if ( action == wipe_Action ) {
            paramList.append("wipeVirtStorageVol");
            paramList.append("0");
        } else if ( action == getXMLDesc_Action ) {
            paramList.append("getVirtStorageVolXMLDesc");
        } else if ( action == reload ) {
            paramList.append("reloadVirtStorageVol");
        } else return;
    } else if ( action == reload ) {
        paramList.append("reloadVirtStorageVol");
    } else return;
    //qDebug()<<paramList<<"paramList from menu";
    emit execMethod(paramList);
}
