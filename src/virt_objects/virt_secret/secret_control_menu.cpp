#include "secret_control_menu.h"

VirtSecretControlMenu::VirtSecretControlMenu(
        QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    if ( !parameters.isEmpty() ) {
        undefine = new QAction("Undefine", this);
        undefine->setIcon(QIcon::fromTheme("security-undefine"));
        getXMLDesc = new QAction("get XML Description", this);
        getXMLDesc->setIcon(QIcon::fromTheme("security-xml"));
        getXMLDesc->setEnabled(true);

        addAction(undefine);
        addSeparator();
        addAction(getXMLDesc);
        addSeparator();
    };
    reload = new QAction("Reload Secret OverView", this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
}

void VirtSecretControlMenu::emitExecMethod(QAction *action)
{
    QStringList paramList;
    if ( !parameters.isEmpty() ) {
        if ( action == undefine ) {
            paramList.append("undefineVirtSecret");
        } else if ( action == getXMLDesc ) {
            paramList.append("getVirtSecretXMLDesc");
        } else if ( action == reload ) {
            paramList.append("reloadVirtSecret");
        } else return;
        if ( action != reload ) paramList.append(parameters.first());
    } else if ( action == reload ) {
        paramList.append("reloadVirtSecret");
    } else return;
    //qDebug()<<paramList<<"paramList from menu";
    emit execMethod(paramList);
}
