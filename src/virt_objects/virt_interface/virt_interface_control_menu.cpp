#include "virt_interface_control_menu.h"

IfaceControlMenu::IfaceControlMenu(
        QWidget *parent, QVariantMap params, bool state) :
    QMenu(parent), autoReloadState(state)
{
    if ( !params.isEmpty() ) {
        state = params.value("state", false).toBool();
        changing = params.value("changing", false).toBool();
        start = new QAction(tr("Start"), this);
        start->setIcon(QIcon::fromTheme("start"));
        start->setVisible(!state );
        destroy = new QAction(tr("Destroy"), this);
        destroy->setIcon(QIcon::fromTheme("destroy"));
        destroy->setVisible(state);
        undefine = new QAction(tr("Undefine"), this);
        undefine->setIcon(QIcon::fromTheme("undefine"));
        changeBegin = new QAction(tr("Change Begin"), this);
        changeBegin->setIcon(QIcon::fromTheme("document-open"));
        changeBegin->setVisible(changing);
        changeCommit = new QAction(tr("Change Commit"), this);
        changeCommit->setIcon(QIcon::fromTheme("document-save"));
        changeCommit->setVisible(changing);
        changeRollback = new QAction(tr("Change Rollback"), this);
        changeRollback->setIcon(QIcon::fromTheme("document-revert"));
        changeRollback->setVisible(changing);
        getXMLDesc = new QAction(tr("get XML Description"), this);
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
    reload = new QAction(tr("Reload Interface OverView"), this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setEnabled(!autoReloadState);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)),
            this, SLOT(emitExecMethod(QAction*)));
}

void IfaceControlMenu::emitExecMethod(QAction *action)
{
    Act_Param paramList;
    if ( action == start) {
        paramList.method = Methods::startEntity;
    } else if ( action == destroy ) {
        paramList.method = Methods::destroyEntity;
    } else if ( action == undefine ) {
        paramList.method = Methods::undefineEntity;
    } else if ( action == changeBegin ) {
        paramList.method = Methods::changeBeginVirtInterface;
    } else if ( action == changeCommit ) {
        paramList.method = Methods::changeCommitVirtInterface;
    } else if ( action == changeRollback ) {
        paramList.method = Methods::changeRollbackVirtInterface;
    } else if ( action == getXMLDesc ) {
        paramList.method = Methods::getEntityXMLDesc;
    } else if ( action == reload ) {
        paramList.method = Methods::reloadEntity;
    } else return;
    emit execMethod(paramList);
}
