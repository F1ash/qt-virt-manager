#include "virt_interface_control_menu.h"

IfaceControlMenu::IfaceControlMenu(
        QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    if ( !parameters.isEmpty() ) {
        start = new QAction("Start", this);
        start->setIcon(QIcon::fromTheme("start"));
        start->setVisible(parameters[2]=="inactive" );
        destroy = new QAction("Destroy", this);
        destroy->setIcon(QIcon::fromTheme("destroy"));
        destroy->setVisible(parameters[2]=="active");
        undefine = new QAction("Undefine", this);
        undefine->setIcon(QIcon::fromTheme("undefine"));
        changeBegin = new QAction("Change Begin", this);
        changeBegin->setIcon(QIcon::fromTheme("document-open"));
        changeBegin->setVisible(parameters[3]!="yes");
        changeCommit = new QAction("Change Commit", this);
        changeCommit->setIcon(QIcon::fromTheme("document-save"));
        changeCommit->setVisible(parameters[3]=="yes");
        changeRollback = new QAction("Change Rollback", this);
        changeRollback->setIcon(QIcon::fromTheme("document-revert"));
        changeRollback->setVisible(parameters[3]=="yes");
        getXMLDesc = new QAction("get XML Description", this);
        getXMLDesc->setIcon(QIcon::fromTheme("application-xml"));
        getXMLDesc->setEnabled(true);

        addAction(start);
        addAction(destroy);
        addAction(undefine);
        addSeparator();
        addAction(changeBegin);
        addAction(changeCommit);
        addAction(changeRollback);
        addSeparator();
        addAction(getXMLDesc);
        addSeparator();
    };
    reload = new QAction("Reload Interface OverView", this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
}

void IfaceControlMenu::emitExecMethod(QAction *action)
{
    Act_Param paramList;
    if ( action == start) {
        paramList.method = startEntity;
    } else if ( action == destroy ) {
        paramList.method = destroyEntity;
    } else if ( action == undefine ) {
        paramList.method = undefineEntity;
    } else if ( action == changeBegin ) {
        paramList.method = changeBeginVirtInterface;
    } else if ( action == changeCommit ) {
        paramList.method = changeCommitVirtInterface;
    } else if ( action == changeRollback ) {
        paramList.method = changeRollbackVirtInterface;
    } else if ( action == getXMLDesc ) {
        paramList.method = getEntityXMLDesc;
    } else if ( action == reload ) {
        paramList.method = reloadEntity;
    } else return;
    emit execMethod(paramList);
}
