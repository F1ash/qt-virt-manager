#include "secret_control_menu.h"

VirtSecretControlMenu::VirtSecretControlMenu(
        QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    if ( !parameters.isEmpty() ) {
        undefine = new QAction("Undefine", this);
        undefine->setIcon(QIcon::fromTheme("undefine"));
        getXMLDesc = new QAction("get XML Description", this);
        getXMLDesc->setIcon(QIcon::fromTheme("application-xml"));
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
    Act_Param paramList;
    if ( action == undefine ) {
        paramList.method = undefineEntity;
    } else if ( action == getXMLDesc ) {
        paramList.method = getEntityXMLDesc;
    } else if ( action == reload ) {
        paramList.method = reloadEntity;
    } else return;
    emit execMethod(paramList);
}
