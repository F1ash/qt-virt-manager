#include "storage_pool_control_menu.h"

StoragePoolControlMenu::StoragePoolControlMenu(
        QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    if ( !parameters.isEmpty() ) {
        start = new QAction("Start", this);
        start->setIcon(QIcon::fromTheme("start"));
        start->setEnabled(parameters.last()=="yes" && parameters[1]!="active" );
        destroy = new QAction("Destroy", this);
        destroy->setIcon(QIcon::fromTheme("destroy"));
        destroy->setEnabled(parameters[1]=="active");
        undefine = new QAction("Undefine", this);
        undefine->setIcon(QIcon::fromTheme("undefine"));
        undefine->setEnabled(parameters.last()=="yes");
        autoStart = new QAction("change AutoStart", this);
        autoStart->setIcon(QIcon::fromTheme("autostart"));
        autoStart->setEnabled(parameters.last()=="yes");
        delete_Menu = new Delete_Pool_Menu(this);
        delete_Action = new QAction("Delete", this);
        delete_Action->setIcon(QIcon::fromTheme("delete"));
        delete_Action->setMenu(delete_Menu);
        connect(delete_Menu, SIGNAL(execMethod(const Act_Param&)),
                this, SIGNAL(execMethod(const Act_Param&)));
        getXMLDesc = new QAction("get XML Description", this);
        getXMLDesc->setIcon(QIcon::fromTheme("application-xml"));
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
    connect(this, SIGNAL(triggered(QAction*)),
            this, SLOT(emitExecMethod(QAction*)));
}

void StoragePoolControlMenu::emitExecMethod(QAction *action)
{
    Act_Param paramList;
    if ( action == start) {
        paramList.method = startEntity;
    } else if ( action == destroy ) {
        paramList.method = destroyEntity;
    } else if ( action == undefine ) {
        paramList.method = undefineEntity;
    } else if ( action == autoStart ) {
        paramList.method = setAutostartEntity;
        paramList.path =
               (QString((parameters[2]=="yes")? "0" : "1"));
    } else if ( action == getXMLDesc ) {
        paramList.method = getEntityXMLDesc;
    } else if ( action == overview ) {
        paramList.method = overviewEntity;
    } else if ( action == reload ) {
        paramList.method = reloadEntity;
    } else return;
    emit execMethod(paramList);
}
