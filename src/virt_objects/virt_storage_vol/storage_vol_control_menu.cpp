#include "storage_vol_control_menu.h"

StorageVolControlMenu::StorageVolControlMenu(QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    if ( !parameters.isEmpty() ) {
        start = new QAction("Start", this);
        start->setIcon(QIcon::fromTheme("storageVol-start"));
        start->setEnabled(parameters.last()=="yes" && parameters[1]!="active" );
        destroy = new QAction("Destroy", this);
        destroy->setIcon(QIcon::fromTheme("storageVol-stop"));
        destroy->setEnabled(parameters[1]=="active");
        undefine = new QAction("Undefine", this);
        undefine->setIcon(QIcon::fromTheme("storageVol-undefine"));
        undefine->setEnabled(parameters.last()=="yes");
        autoStart = new QAction("change AutoStart", this);
        autoStart->setIcon(QIcon::fromTheme("storageVol-autostart"));
        autoStart->setEnabled(parameters.last()=="yes");
        getXMLDesc = new QAction("get XML Description", this);
        getXMLDesc->setIcon(QIcon::fromTheme("storageVol-xml"));
        getXMLDesc->setEnabled(true);

        addAction(start);
        addAction(destroy);
        addAction(undefine);
        addAction(autoStart);
        addSeparator();
        addAction(getXMLDesc);
        addSeparator();
    };
    reload = new QAction("Reload Pool OverView", this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
}
StorageVolControlMenu::~StorageVolControlMenu()
{
    disconnect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
    if ( !parameters.isEmpty() ) {
        delete start;
        start = 0;
        delete destroy;
        destroy = 0;
        delete undefine;
        undefine = 0;
        delete autoStart;
        autoStart = 0;
        delete getXMLDesc;
        getXMLDesc = 0;
    };
    delete reload;
    reload = 0;
}
void StorageVolControlMenu::emitExecMethod(QAction *action)
{
    QStringList paramList;
    if ( !parameters.isEmpty() ) {
        if ( action == start) {
            paramList << "downloadVirtStorageVolList";
        } else if ( action == destroy ) {
            paramList << "deleteVirtStorageVol";
        } else if ( action == undefine ) {
            paramList << "resizeVirtStorageVol";
        } else if ( action == autoStart ) {
            paramList << "uploadVirtStorageVolList";
        } else if ( action == getXMLDesc ) {
            paramList << "getVirtStorageVolXMLDesc";
        } else if ( action == reload ) {
            paramList << "reloadVirtStoragePool";
        } else return;
        paramList.append(parameters.first());
    } else if ( action == reload ) {
        paramList << "reloadVirtStoragePool";
    } else return;
    //qDebug()<<paramList<<"paramList from menu";
    emit execMethod(paramList);
}
