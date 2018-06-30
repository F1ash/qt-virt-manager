#include "secret_control.h"

VirtSecretControl::VirtSecretControl(QWidget *parent) :
    VirtEntityControl(parent)
{
    setObjectName("VirtSecretControl");
    setWindowTitle(tr("VirtSecret Control"));
    setWindowIcon(QIcon::fromTheme("security"));
    virtSecretModel = new VirtSecretModel();
    entityList->setModel(virtSecretModel);
    secListDlg = new SecretListDelegate();
    entityList->setItemDelegate(secListDlg);
    //connect(entityList, SIGNAL(doubleClicked(const QModelIndex&)),
    //        this, SLOT(entityDoubleClicked(const QModelIndex&)));
    connect(entityList, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(entityClicked(const QPoint&)));
    settings.beginGroup("VirtSecretControl");
    entityList->setColumnWidth(0, settings.value("column0", 132).toInt());
    entityList->setColumnWidth(1, settings.value("column1", 32).toInt());
    int area_int = settings.value("ToolBarArea", 4).toInt();
    settings.endGroup();
    toolBar = new VirtSecretToolBar(this);
    addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
    connect(toolBar, SIGNAL(execMethod(const Act_Param&)),
            this, SLOT(execAction(const Act_Param&)));
}
VirtSecretControl::~VirtSecretControl()
{
    settings.beginGroup("VirtSecretControl");
    settings.setValue("column0", entityList->columnWidth(0));
    settings.setValue("column1", entityList->columnWidth(1));
    settings.setValue("ToolBarArea", toolBarArea(toolBar));
    settings.endGroup();
    settings.sync();

    stopProcessing();
}

/* public slots */
void VirtSecretControl::stopProcessing()
{
    setEnabled(false);
    // clear Secret list
    while ( virtSecretModel->DataList.count() ) {
        virtSecretModel->removeRow(0);
    };
    virtSecretModel->setHeaderData(
                0,
                Qt::Horizontal,
                tr("UUID"),
                Qt::EditRole);

}
bool VirtSecretControl::setCurrentWorkConnection(virConnectPtr *connPtrPtr)
{
    stopProcessing();
    ptr_ConnPtr = connPtrPtr;
    toolBar->enableAutoReload();
    return true;
}
void VirtSecretControl::setListHeader(const QString &connName)
{
    virtSecretModel->setHeaderData(
                0, Qt::Horizontal,
                QString(tr("Secret UUID in [ %1 ]")).arg(connName),
                Qt::EditRole);
    currConnName = connName;
    setEnabled(true);
    // for initiation content
    reloadState();
}
QString VirtSecretControl::getCurrentSecUsage() const
{
    QModelIndex index = entityList->currentIndex();
    if ( !index.isValid() ) return QString();
    return virtSecretModel->DataList.at(index.row())->getUsageID();
}
QString VirtSecretControl::getCurrentSecUUID() const
{
    QModelIndex index = entityList->currentIndex();
    if ( !index.isValid() ) return QString();
    return virtSecretModel->DataList.at(index.row())->getUUID();
}
QString VirtSecretControl::getCurrentSecType() const
{
    QModelIndex index = entityList->currentIndex();
    if ( !index.isValid() ) return QString();
    return virtSecretModel->DataList.at(index.row())->getType();
}
void VirtSecretControl::resultReceiver(Result *data)
{
    //qDebug()<<data->action<<data->name<<"result";
    if ( data->action == Actions::GET_ALL_ENTITY_STATE ) {
        if ( data->data.count() > virtSecretModel->DataList.count() ) {
            int _diff = data->data.count() - virtSecretModel->DataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtSecretModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( virtSecretModel->DataList.count() > data->data.count() ) {
            int _diff = virtSecretModel->DataList.count() - data->data.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtSecretModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QVariantMap _data, data->data) {
            if (_data.isEmpty()) continue;
            virtSecretModel->setData(
                            virtSecretModel->index(i, 0),
                            _data.value("UUID", ""),
                            Qt::EditRole);
            virtSecretModel->setData(
                            virtSecretModel->index(i, 1),
                            _data.value("UsageID", ""),
                            Qt::EditRole);
            virtSecretModel->setData(
                            virtSecretModel->index(i, 0),
                            _data.value("desc", ""),
                            Qt::ToolTipRole);
            virtSecretModel->setData(
                            virtSecretModel->index(i, 1),
                            _data.value("type", ""),
                            Qt::ToolTipRole);
            i++;
        };
        entityList->setEnabled(true);
        emit entityListUpdated();
    } else if ( data->action == Actions::GET_XML_DESCRIPTION ) {
        QString xml = data->fileName;
        data->msg.append(QString(tr("to <a href='%1'>%1</a>")).arg(xml));
        QString msg = QString("%1<br>%2")
                .arg(data->msg.join(" "))
                .arg(data->err);
        msgRepeater(msg);
        if ( data->result )
            QDesktopServices::openUrl(QUrl(xml));
    } else if ( data->action != Actions::_NONE_ACTION ) {
        if ( !data->msg.isEmpty() || !data->err.isEmpty() ) {
            QString msg = QString("%1<br>%2")
                    .arg(data->msg.join(" "))
                    .arg(data->err);
            msgRepeater(msg);
        };
        if ( data->result ) {
            reloadState();
            // for different action's specified manipulation
            switch (data->action) {
            case Actions::_NONE_ACTION:
                // some job;
                break;
            default:
                break;
            }
        };
    };
}
void VirtSecretControl::reloadState()
{
    entityList->setEnabled(false);
    entityList->clearSelection();
    TASK task;
    task.type       = VIRT_ENTITY::VIRT_SECRET;
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    task.action     = Actions::GET_ALL_ENTITY_STATE;
    task.method     = Methods::reloadEntity;
    emit addNewTask(&task);
}

/* private slots */
void VirtSecretControl::changeDockVisibility()
{
    toolBar->setEnabled( !toolBar->isEnabled() );
    entityList->setEnabled( !entityList->isEnabled() );
}
void VirtSecretControl::entityClicked(const QPoint &p)
{
    //qDebug()<<"custom Menu request";
    QModelIndex idx = entityList->indexAt(p);
    QStringList params;
    if ( idx.isValid() && virtSecretModel->DataList.count()>idx.row() ) {
        //qDebug()<<virtSecretModel->DataList.at(idx.row())->getName();
        params<<virtSecretModel->DataList.at(idx.row())->getUUID();
        params<<virtSecretModel->DataList.at(idx.row())->getUsageID();
    } else {
        entityList->clearSelection();
    };
    bool state = toolBar->getAutoReloadState();
    VirtSecretControlMenu *secControlMenu =
            new VirtSecretControlMenu(this, params, state);
    connect(secControlMenu, SIGNAL(execMethod(const Act_Param&)),
            this, SLOT(execAction(const Act_Param&)));
    secControlMenu->move(QCursor::pos());
    secControlMenu->exec();
    disconnect(secControlMenu, SIGNAL(execMethod(const Act_Param&)),
               this, SLOT(execAction(const Act_Param&)));
    secControlMenu->deleteLater();
}
void VirtSecretControl::entityDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        qDebug()<<virtSecretModel->DataList.at(index.row())->getUUID();
    }
}
void VirtSecretControl::execAction(const Act_Param &param)
{
    TASK task;
    task.type       = VIRT_ENTITY::VIRT_SECRET;
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    task.method     = param.method;
    QModelIndex idx = entityList->currentIndex();
    if ( idx.isValid() && virtSecretModel->DataList.count()>idx.row() ) {
        QString uuid = virtSecretModel->DataList.at(idx.row())->getUUID();
        task.object = uuid;
        if        ( param.method==Methods::defineEntity ) {
            QString xml;
            bool show = false;
            // show Secret Creator widget
            CreateVirtSecret *createVirtSec = new CreateVirtSecret(this, ptr_ConnPtr);
            int result = createVirtSec->exec();
            if ( createVirtSec!=nullptr && result==QDialog::Accepted ) {
                xml = createVirtSec->getXMLDescFileName();
                show = createVirtSec->getShowing();
                QStringList data;
                data.append(tr("New Secret XML'ed"));
                data.append(QString(tr("to <a href='%1'>%1</a>")).arg(xml));
                QString msg = data.join(" ");
                msgRepeater(msg);
                if ( show ) QDesktopServices::openUrl(QUrl(xml));
                task.action     = Actions::DEFINE_ENTITY;
                task.args.path  = xml;
                QByteArray secVal = createVirtSec->getSecretValue();
                task.secret->setSecretValue(&secVal);
                emit addNewTask(&task);
            };
            delete createVirtSec;
            createVirtSec = nullptr;
            //qDebug()<<xml<<"path"<<result;
        } else if ( param.method==Methods::undefineEntity ) {
            task.action     = Actions::UNDEFINE_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==Methods::getEntityXMLDesc ) {
            task.action     = Actions::GET_XML_DESCRIPTION;
            emit addNewTask(&task);
        } else if ( param.method==Methods::reloadEntity ) {
            reloadState();
        };
    } else if ( param.method==Methods::reloadEntity ) {
        reloadState();
    } else if ( param.method==Methods::defineEntity ) {
        QString xml;
        bool show = false;
        // show Secret Creator widget
        CreateVirtSecret *createVirtSec =
                new CreateVirtSecret(this, ptr_ConnPtr);
        int result = createVirtSec->exec();
        if ( createVirtSec!=nullptr && result==QDialog::Accepted ) {
            xml = createVirtSec->getXMLDescFileName();
            show = createVirtSec->getShowing();
            QStringList data;
            data.append(tr("New Secret XML'ed"));
            data.append(QString(tr("to <a href='%1'>%1</a>")).arg(xml));
            QString msg = data.join(" ");
            msgRepeater(msg);
            if ( show ) QDesktopServices::openUrl(QUrl(xml));
            task.action     = Actions::DEFINE_ENTITY;
            task.args.path  = xml;
            QByteArray secVal = createVirtSec->getSecretValue();
            task.secret->setSecretValue(&secVal);
            emit addNewTask(&task);
        };
        delete createVirtSec;
        createVirtSec = nullptr;
        //qDebug()<<xml<<"path"<<result;
    };
}
