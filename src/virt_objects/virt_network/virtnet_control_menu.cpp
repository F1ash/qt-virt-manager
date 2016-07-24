#include "virtnet_control_menu.h"

VirtNetControlMenu::VirtNetControlMenu(
        QWidget *parent,
        QStringList params,
        bool state) :
    QMenu(parent),
    parameters(params),
    autoReloadState(state)
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
        paramList.method = startEntity;
    } else if ( action == destroy ) {
        paramList.method = destroyEntity;
    } else if ( action == undefine ) {
        paramList.method = undefineEntity;
    } else if ( action == autoStart ) {
        paramList.method = setAutostartEntity;
        paramList.path =
              (QString((parameters[2]=="yes")? "0" : "1"));
    } else if ( action == edit ) {
        paramList.method = editEntity;
    } else if ( action == getXMLDesc ) {
        paramList.method = getEntityXMLDesc;
    } else if ( action == reload ) {
        paramList.method = reloadEntity;
    } else return;
    emit execMethod(paramList);
}
