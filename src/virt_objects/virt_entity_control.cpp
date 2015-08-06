#include "virt_entity_control.h"

VirtEntityControl::VirtEntityControl(QWidget *parent) : QMainWindow(parent)
{
    qRegisterMetaType<QString>("QString&");
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    setMinimumSize(100, 100);
    entityList = new QTreeView(this);
    entityList->setItemsExpandable(false);
    entityList->setRootIsDecorated(false);
    entityList->setFocus();
    entityList->setContextMenuPolicy(Qt::CustomContextMenu);
    setCentralWidget(entityList);
}
VirtEntityControl::~VirtEntityControl()
{
}

bool VirtEntityControl::getThreadState() const
{
    return true;
}
void VirtEntityControl::stopProcessing() {}
bool VirtEntityControl::setCurrentWorkConnect(virConnect *conn)
{
    stopProcessing();
    currWorkConnection = conn;
    //toolBar->enableAutoReload();
    return true;
}
void VirtEntityControl::setListHeader(QString &_name)
{

}
void VirtEntityControl::reloadState()
{
    return;
}
void VirtEntityControl::resultReceiver(Result data)
{

}
void VirtEntityControl::changeDockVisibility()
{
    //toolBar->setEnabled( !toolBar->isEnabled() );
    entityList->setEnabled( !entityList->isEnabled() );
}
void VirtEntityControl::entityClicked(const QPoint &pos)
{

}
void VirtEntityControl::entityDoubleClicked(const QModelIndex &index)
{

}
void VirtEntityControl::execAction(const QStringList &l)
{

}
void VirtEntityControl::newVirtEntityFromXML(const QStringList &_args)
{

}
void VirtEntityControl::doneEntityCreationDialog()
{

}

/* public slots */
void VirtEntityControl::msgRepeater(QString &msg)
{
    QString time = QTime::currentTime().toString();
    QString title = QString("Connection '%1'").arg(currConnName);
    QString currMsg = QString("<b>%1 %2:</b><br><font color='green'><b>ACTION</b></font>: %3")
            .arg(time).arg(title).arg(msg);
    emit entityMsg(currMsg);
}
