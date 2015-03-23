#include "virt_entity_control.h"

VirtEntityControl::VirtEntityControl(QWidget *parent) : QMainWindow(parent)
{
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
    if ( currWorkConnect!=NULL ) {
        virConnectClose(currWorkConnect);
        currWorkConnect = NULL;
    };
}

bool VirtEntityControl::getThreadState() const
{
    return true;
}
void VirtEntityControl::stopProcessing() {}
bool VirtEntityControl::setCurrentWorkConnect(virConnect *conn)
{
    stopProcessing();
    currWorkConnect = conn;
    int ret = virConnectRef(currWorkConnect);
    if ( ret<0 ) {
        virErrorPtr virtErrors = virGetLastError();
        if ( virtErrors!=NULL && virtErrors->code>0 ) {
            QString time = QTime::currentTime().toString();
            QString msg = QString("%3 VirtError(%1) : %2")
                    .arg(virtErrors->code)
                    .arg(virtErrors->message)
                    .arg(time);
            emit entityMsg( msg );
            virResetError(virtErrors);
        };
        currWorkConnect = NULL;
        return false;
    } else {
        //toolBar->enableAutoReload();
        return true;
    };
}
void VirtEntityControl::setListHeader(QString &_name)
{

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

/* public slots */
void VirtEntityControl::msgRepeater(QString msg)
{
    QString time = QTime::currentTime().toString();
    QString title = QString("Connect '%1'").arg(currConnName);
    QString errorMsg = QString("<b>%1 %2:</b><br>%3").arg(time).arg(title).arg(msg);
    emit entityMsg(errorMsg);
}
