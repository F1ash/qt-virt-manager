#include "virtnet_control_menu.h"

VirtNetControlMenu::VirtNetControlMenu(
        QWidget *parent, QVariantMap params, bool state) :
    QMenu(parent), autoReloadState(state)
{
    if ( !params.isEmpty() ) {
        active = params.value("active", false).toBool();
        autostart = params.value("auto", false).toBool();
        persistent = params.value("persistent", false).toBool();
        start = new QAction("Start", this);
        start->setIcon(QIcon::fromTheme("start"));
        start->setEnabled( persistent && !active );
        destroy = new QAction("Destroy", this);
        destroy->setIcon(QIcon::fromTheme("destroy"));
        destroy->setEnabled(active);
        undefine = new QAction("Undefine", this);
        undefine->setIcon(QIcon::fromTheme("undefine"));
        undefine->setEnabled(persistent);
        autoStart = new QAction("change AutoStart", this);
        autoStart->setIcon(QIcon::fromTheme("autostart"));
        autoStart->setEnabled(persistent);
        edit = new QAction("Edit", this);
        edit->setIcon(QIcon::fromTheme("configure"));
        edit->setVisible(true);
        getXMLDesc = new QAction("get XML Description", this);
        getXMLDesc->setIcon(QIcon::fromTheme("application-xml"));
        getXMLDesc->setEnabled(true);

        addAction(start);
        addAction(destroy);
        addAction(undefine);
        addAction(autoStart);
        addSeparator();
        addAction(edit);
        addSeparator();
        addAction(getXMLDesc);
        addSeparator();
    };
    reload = new QAction("Reload Network OverView", this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)),
            this, SLOT(emitExecMethod(QAction*)));
}

void VirtNetControlMenu::emitExecMethod(QAction *action)
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
    } else if ( action == edit ) {
        paramList.method = Methods::editEntity;
    } else if ( action == getXMLDesc ) {
        paramList.method = Methods::getEntityXMLDesc;
    } else if ( action == reload ) {
        paramList.method = Methods::reloadEntity;
    } else return;
    emit execMethod(paramList);
}
