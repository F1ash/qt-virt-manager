#include "storage_pool_control_menu.h"

StoragePoolControlMenu::StoragePoolControlMenu(
        QWidget *parent, QVariantMap params, bool state) :
    QMenu(parent), autoReloadState(state)
{
    if ( !params.isEmpty() ) {
        active = params.value("active", false).toBool();
        autostart = params.value("auto", false).toBool();
        persistent = params.value("persistent", false).toBool();
        start = new QAction(tr("Start"), this);
        start->setIcon(QIcon::fromTheme("start"));
        start->setEnabled(persistent && !active);
        destroy = new QAction(tr("Destroy"), this);
        destroy->setIcon(QIcon::fromTheme("destroy"));
        destroy->setEnabled(active);
        undefine = new QAction(tr("Undefine"), this);
        undefine->setIcon(QIcon::fromTheme("undefine"));
        undefine->setEnabled(persistent);
        autoStart = new QAction(tr("change AutoStart"), this);
        autoStart->setIcon(QIcon::fromTheme("autostart"));
        autoStart->setEnabled(persistent);
        delete_Menu = new Delete_Pool_Menu(this);
        delete_Action = new QAction(tr("Delete"), this);
        delete_Action->setIcon(QIcon::fromTheme("delete"));
        delete_Action->setMenu(delete_Menu);
        connect(delete_Menu, SIGNAL(execMethod(const Act_Param&)),
                this, SIGNAL(execMethod(const Act_Param&)));
        getXMLDesc = new QAction(tr("get XML Description"), this);
        getXMLDesc->setIcon(QIcon::fromTheme("application-xml"));
        getXMLDesc->setEnabled(true);
        overview = new QAction(tr("overview Pool"), this);
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
    reload = new QAction(tr("Reload Pool OverView"), this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)),
            this, SLOT(emitExecMethod(QAction*)));
}

void StoragePoolControlMenu::emitExecMethod(QAction *action)
{
    Act_Param paramList;
    if ( action == start) {
        paramList.method = Methods::startEntity;
    } else if ( action == destroy ) {
        paramList.method = Methods::destroyEntity;
    } else if ( action == undefine ) {
        paramList.method = Methods::undefineEntity;
    } else if ( action == autoStart ) {
        paramList.method = Methods::setAutostartEntity;
        paramList.path = (autostart)? "0" : "1";
    } else if ( action == getXMLDesc ) {
        paramList.method = Methods::getEntityXMLDesc;
    } else if ( action == overview ) {
        paramList.method = Methods::overviewEntity;
    } else if ( action == reload ) {
        paramList.method = Methods::reloadEntity;
    } else return;
    emit execMethod(paramList);
}
