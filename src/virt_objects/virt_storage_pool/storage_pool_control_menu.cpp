#include "storage_pool_control_menu.h"

StoragePoolControlMenu::StoragePoolControlMenu(QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    if ( !parameters.isEmpty() ) {
        start = new QAction("Start", this);
        start->setIcon(QIcon::fromTheme("storagePool-start"));
        start->setEnabled(parameters.last()=="yes" && parameters[1]!="active" );
        destroy = new QAction("Destroy", this);
        destroy->setIcon(QIcon::fromTheme("storagePool-stop"));
        destroy->setEnabled(parameters[1]=="active");
        undefine = new QAction("Undefine", this);
        undefine->setIcon(QIcon::fromTheme("storagePool-undefine"));
        undefine->setEnabled(parameters.last()=="yes");
        autoStart = new QAction("change AutoStart", this);
        autoStart->setIcon(QIcon::fromTheme("storagePool-autostart"));
        autoStart->setEnabled(parameters.last()=="yes");
        delete_Menu = new Delete_Pool_Menu(this);
        delete_Action = new QAction("Delete", this);
        delete_Action->setIcon(QIcon::fromTheme("storageVol-delete"));
        delete_Action->setMenu(delete_Menu);
        connect(delete_Menu, SIGNAL(execMethod(const QStringList&)),
                this, SIGNAL(execMethod(const QStringList&)));
        getXMLDesc = new QAction("get XML Description", this);
        getXMLDesc->setIcon(QIcon::fromTheme("storagePool-xml"));
        getXMLDesc->setEnabled(true);
        overview = new QAction("overview Pool", this);
        overview->setIcon(QIcon::fromTheme("overview"));
        overview->setEnabled(true);

        addAction(start);
        addAction(destroy);
        addAction(undefine);
        addAction(autoStart);
        addAction(delete_Action);
        addSeparator();
        addAction(getXMLDesc);
        addSeparator();
        addAction(overview);
        addSeparator();
    };
    reload = new QAction("Reload Pool OverView", this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
}
StoragePoolControlMenu::~StoragePoolControlMenu()
{
    disconnect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
    if ( !parameters.isEmpty() ) {
        disconnect(delete_Menu, SIGNAL(execMethod(const QStringList&)),
                   this, SIGNAL(execMethod(const QStringList&)));
        delete start;
        start = NULL;
        delete destroy;
        destroy = NULL;
        delete undefine;
        undefine = NULL;
        delete autoStart;
        autoStart = NULL;
        delete delete_Menu;
        delete_Menu = NULL;
        delete delete_Action;
        delete_Action = NULL;
        delete getXMLDesc;
        getXMLDesc = NULL;
        delete overview;
        overview = NULL;
    };
    delete reload;
    reload = NULL;
}
void StoragePoolControlMenu::emitExecMethod(QAction *action)
{
    QStringList paramList;
    if ( !parameters.isEmpty() ) {
        if ( action == start) {
            paramList.append("startVirtStoragePool");
        } else if ( action == destroy ) {
            paramList.append("destroyVirtStoragePool");
        } else if ( action == undefine ) {
            paramList.append("undefineVirtStoragePool");
        } else if ( action == autoStart ) {
            paramList.append("setAutostartVirtStoragePool");
            paramList.append(QString((parameters[2]=="yes")? "0" : "1"));
        } else if ( action == getXMLDesc ) {
            paramList.append("getVirtStoragePoolXMLDesc");
        } else if ( action == overview ) {
            paramList.append("overviewVirtStoragePool");
        } else if ( action == reload ) {
            paramList.append("reloadVirtStoragePool");
        } else return;
        if ( action != reload ) paramList.append(parameters.first());
    } else if ( action == reload ) {
        paramList.append("reloadVirtStoragePool");
    } else return;
    //qDebug()<<paramList<<"paramList from menu";
    emit execMethod(paramList);
}
