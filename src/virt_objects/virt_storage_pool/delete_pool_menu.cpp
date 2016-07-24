#include "delete_pool_menu.h"

Delete_Pool_Menu::Delete_Pool_Menu(QWidget *parent) :
    QMenu(parent)
{
    delete_Normal = new QAction("Delete Normal", this);
    delete_Normal->setIcon(QIcon::fromTheme("delete"));
    delete_Zeroed = new QAction("Delete Zeroed", this);
    delete_Zeroed->setIcon(QIcon::fromTheme("delete"));

    addAction(delete_Normal);
    addAction(delete_Zeroed);

    connect(this, SIGNAL(triggered(QAction*)),
            this, SLOT(emitExecMethod(QAction*)));
    connect(this, SIGNAL(hovered(QAction*)),
            this, SLOT(showActionToolTip(QAction*)));
}

/* private slots */
void Delete_Pool_Menu::emitExecMethod(QAction *action)
{
    Act_Param parameters;
    if ( action == delete_Normal ) {
        parameters.method = deleteEntity;
        parameters.path = QString::number(
                   VIR_STORAGE_POOL_DELETE_NORMAL);
    } else if ( action == delete_Zeroed ) {
        parameters.method = deleteEntity;
        parameters.path = QString::number(
                   VIR_STORAGE_POOL_DELETE_ZEROED);
    } else return;
    emit execMethod(parameters);
}
void Delete_Pool_Menu::showActionToolTip(QAction *action)
{
    QString toolTip;
    if ( action == delete_Normal ) {
        toolTip = QString("Delete metadata only (fast)");
    } else if ( action == delete_Zeroed ) {
        toolTip = QString("Clear all data to zeros (slow)");
    };
    setToolTip(toolTip);
}
