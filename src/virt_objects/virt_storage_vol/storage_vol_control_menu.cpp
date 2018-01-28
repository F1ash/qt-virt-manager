#include "storage_vol_control_menu.h"

StorageVolControlMenu::StorageVolControlMenu(
        QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    if ( !parameters.isEmpty() ) {
        delete_Action = new QAction(tr("Delete"), this);
        delete_Action->setIcon(QIcon::fromTheme("delete"));
        download_Action = new QAction(tr("Download"), this);
        download_Action->setIcon(QIcon::fromTheme("download"));
        resize_Action = new QAction(tr("Resize"), this);
        resize_Action->setIcon(QIcon::fromTheme("resize"));
        upload_Action = new QAction(tr("Upload"), this);
        upload_Action->setIcon(QIcon::fromTheme("upload"));
        wipe_Menu = new WipeMenu(this);
        wipe_Action = new QAction(tr("Wipe"), this);
        wipe_Action->setIcon(QIcon::fromTheme("wipe"));
        wipe_Action->setMenu(wipe_Menu);
        connect(wipe_Menu, SIGNAL(execMethod(const Act_Param&)),
                this, SIGNAL(execMethod(const Act_Param&)));
        getXMLDesc_Action = new QAction(tr("get XML Description"), this);
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
    reload = new QAction(tr("Reload Volume OverView"), this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)),
            this, SLOT(emitExecMethod(QAction*)));
}

void StorageVolControlMenu::emitExecMethod(QAction *action)
{
    Act_Param paramList;
    if ( action == resize_Action) {
        paramList.method = Methods::resizeVirtStorageVol;
    } else if ( action == delete_Action ) {
        paramList.method = Methods::deleteEntity;
    } else if ( action == download_Action ) {
        paramList.method = Methods::downloadVirtStorageVol;
    } else if ( action == upload_Action ) {
        paramList.method = Methods::uploadVirtStorageVol;
    } else if ( action == wipe_Action ) {
        paramList.method = Methods::wipeVirtStorageVol;
        paramList.path = "0";
    } else if ( action == getXMLDesc_Action ) {
        paramList.method = Methods::getEntityXMLDesc;
    } else if ( action == reload ) {
        paramList.method = Methods::reloadEntity;
    } else return;
    emit execMethod(paramList);
}
