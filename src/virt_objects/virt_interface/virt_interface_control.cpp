#include "virt_interface_control.h"

VirtInterfaceControl::VirtInterfaceControl(QWidget *parent) :
    VirtEntityControl(parent)
{
    setObjectName("VirtIfaceControl");
    setWindowTitle(tr("VirtIface Control"));
    setWindowIcon(QIcon::fromTheme("network-wired"));
    virtIfaceModel = new VirtIfaceModel();
    entityList->setModel(virtIfaceModel);
    ifaceListDelegate = new IfaceListDelegate();
    entityList->setItemDelegate(ifaceListDelegate);
    //connect(entityList, SIGNAL(doubleClicked(const QModelIndex&)),
    //        this, SLOT(entityDoubleClicked(const QModelIndex&)));
    connect(entityList, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(entityClicked(const QPoint&)));
    settings.beginGroup("VirtIfaceControl");
    entityList->setColumnWidth(0, settings.value("column0", 132).toInt());
    entityList->setColumnWidth(1, settings.value("column1", 32).toInt());
    entityList->setColumnWidth(2, settings.value("column2", 32).toInt());
    entityList->setColumnWidth(3, settings.value("column3", 32).toInt());
    int area_int = settings.value("ToolBarArea", 4).toInt();
    settings.endGroup();
    toolBar = new InterfaceToolBar(this);
    addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
    connect(toolBar, SIGNAL(fileForMethod(const Act_Param&)),
            this, SLOT(newVirtEntityFromXML(const Act_Param&)));
    connect(toolBar, SIGNAL(execMethod(const Act_Param&)),
            this, SLOT(execAction(const Act_Param&)));
}
VirtInterfaceControl::~VirtInterfaceControl()
{
    settings.beginGroup("VirtIfaceControl");
    settings.setValue("column0", entityList->columnWidth(0));
    settings.setValue("column1", entityList->columnWidth(1));
    settings.setValue("column2", entityList->columnWidth(2));
    settings.setValue("column3", entityList->columnWidth(3));
    settings.setValue("ToolBarArea", toolBarArea(toolBar));
    settings.endGroup();
    settings.sync();

    stopProcessing();
}

/* public slots */
void VirtInterfaceControl::stopProcessing()
{
    setEnabled(false);
    // clear Network list
    while ( virtIfaceModel->DataList.count() ) {
        virtIfaceModel->removeRow(0);
    };
    virtIfaceModel->setHeaderData(
                0,
                Qt::Horizontal,
                tr("Name"),
                Qt::EditRole);

}
bool VirtInterfaceControl::setCurrentWorkConnection(virConnectPtr *connPtrPtr)
{
    stopProcessing();
    ptr_ConnPtr = connPtrPtr;
    toolBar->enableAutoReload();
    return true;
}
void VirtInterfaceControl::setListHeader(const QString &connName)
{
    virtIfaceModel->setHeaderData(
                0, Qt::Horizontal,
                QString(tr("Interface in [ %1 ]")).arg(connName),
                Qt::EditRole);
    currConnName = connName;
    setEnabled(true);
    // for initiation content
    reloadState();
}
void VirtInterfaceControl::resultReceiver(Result *data)
{
    //qDebug()<<data->action<<data->msg<<"result";
    if ( data->action == Actions::GET_ALL_ENTITY_STATE ) {
        if ( data->data.count() > virtIfaceModel->DataList.count() ) {
            int _diff = data->data.count() - virtIfaceModel->DataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtIfaceModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( virtIfaceModel->DataList.count() > data->data.count() ) {
            int _diff = virtIfaceModel->DataList.count() - data->data.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtIfaceModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QVariantMap _data, data->data) {
            if (_data.isEmpty()) continue;
            virtIfaceModel->setData(
                            virtIfaceModel->index(i, 0),
                            _data.value("name", ""),
                            Qt::EditRole);
            virtIfaceModel->setData(
                            virtIfaceModel->index(i, 1),
                            _data.value("MAC", ""),
                            Qt::EditRole);
            virtIfaceModel->setData(
                            virtIfaceModel->index(i, 2),
                            _data.value("state", false),
                            Qt::EditRole);
            virtIfaceModel->setData(
                            virtIfaceModel->index(i, 3),
                            _data.value("changing", false),
                            Qt::EditRole);
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
            int row = -1;
            for ( int i=0; i<virtIfaceModel->DataList.count(); i++ ) {
                if ( virtIfaceModel->DataList.at(i)->getName().compare(data->name)==0 ) {
                    row = i;
                    break;
                };
            };
            // for different action's specified manipulation;
            // set manually, because next reload not change 'changing'
            switch (data->action) {
            case Actions::IFACE_CHANGE_BEGIN:
                if (row+1>0)
                    virtIfaceModel->DataList.at(row)->setChanging(true);
                break;
            case Actions::IFACE_CHANGE_COMMIT:
                if (row+1>0)
                    virtIfaceModel->DataList.at(row)->setChanging(false);
                break;
            case Actions::IFACE_CHANGE_ROLLBACK:
                if (row+1>0)
                    virtIfaceModel->DataList.at(row)->setChanging(false);
                break;
            default:
                break;
            };
        };
        reloadState();
    };
}

/* private slots */
void VirtInterfaceControl::reloadState()
{
    entityList->setEnabled(false);
    entityList->clearSelection();
    TASK task;
    task.type       = VIRT_ENTITY::VIRT_INTERFACE;
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    task.action     = Actions::GET_ALL_ENTITY_STATE;
    task.method     = Methods::reloadEntity;
    emit addNewTask(&task);
}
void VirtInterfaceControl::changeDockVisibility()
{
    toolBar->setEnabled( !toolBar->isEnabled() );
    entityList->setEnabled( !entityList->isEnabled() );
}
void VirtInterfaceControl::entityClicked(const QPoint &p)
{
    //qDebug()<<"custom Menu request";
    QModelIndex idx = entityList->indexAt(p);
    QVariantMap params;
    if ( idx.isValid() && virtIfaceModel->DataList.count()>idx.row() ) {
        //qDebug()<<virtIfaceModel->DataList.at(idx.row())->getName();
        params.insert(
                    "name",
                    virtIfaceModel->DataList.at(idx.row())->getName());
        params.insert(
                    "MAC",
                    virtIfaceModel->DataList.at(idx.row())->getMAC());
        params.insert(
                    "state",
                    virtIfaceModel->DataList.at(idx.row())->getState());
        params.insert(
                    "changing",
                    virtIfaceModel->DataList.at(idx.row())->getChanging());
    } else {
        entityList->clearSelection();
    };
    bool state = toolBar->getAutoReloadState();
    IfaceControlMenu *ifaceControlMenu =
            new IfaceControlMenu(this, params, state);
    connect(ifaceControlMenu, SIGNAL(execMethod(const Act_Param&)),
            this, SLOT(execAction(const Act_Param&)));
    ifaceControlMenu->move(QCursor::pos());
    ifaceControlMenu->exec();
    disconnect(ifaceControlMenu, SIGNAL(execMethod(const Act_Param&)),
               this, SLOT(execAction(const Act_Param&)));
    ifaceControlMenu->deleteLater();
}
void VirtInterfaceControl::entityDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        qDebug()<<virtIfaceModel->DataList.at(index.row())->getName();
    }
}
void VirtInterfaceControl::execAction(const Act_Param &param)
{
    QModelIndex idx = entityList->currentIndex();
    if ( idx.isValid() && virtIfaceModel->DataList.count()>idx.row() ) {
        QString networkName = virtIfaceModel->DataList.at(idx.row())->getName();
        TASK task;
        task.type       = VIRT_ENTITY::VIRT_INTERFACE;
        task.srcConnPtr = ptr_ConnPtr;
        task.srcConName = currConnName;
        task.object     = networkName;
        task.method     = param.method;
        if        ( param.method==Methods::startEntity ) {
            task.action = Actions::START_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==Methods::destroyEntity ) {
            task.action = Actions::DESTROY_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==Methods::defineEntity ) {
            //newVirtEntityFromXML(l);
        } else if ( param.method==Methods::undefineEntity ) {
            task.action = Actions::UNDEFINE_ENTITY;
            emit addNewTask(&task);
        } else if ( param.method==Methods::changeBeginVirtInterface ) {
            task.action = Actions::IFACE_CHANGE_BEGIN;
            emit addNewTask(&task);
        } else if ( param.method==Methods::changeCommitVirtInterface ) {
            task.action = Actions::IFACE_CHANGE_COMMIT;
            emit addNewTask(&task);
        } else if ( param.method==Methods::changeRollbackVirtInterface ) {
            task.action = Actions::IFACE_CHANGE_ROLLBACK;
            emit addNewTask(&task);
        } else if ( param.method==Methods::getEntityXMLDesc ) {
            task.action = Actions::GET_XML_DESCRIPTION;
            emit addNewTask(&task);
        } else if ( param.method==Methods::reloadEntity ) {
            reloadState();
        };
    } else if ( param.method==Methods::reloadEntity ) {
        reloadState();
    } else if ( param.method==Methods::defineEntity ) {
        //newVirtEntityFromXML(l);
    };
}
void VirtInterfaceControl::newVirtEntityFromXML(const Act_Param &args)
{
    TASK task;
    task.type       = VIRT_ENTITY::VIRT_INTERFACE;
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    task.method     = Methods::defineEntity;
    task.action     = Actions::DEFINE_ENTITY;
    task.args.path  = args.path;
    if ( args.context==HOW_TO_DO::DO_AsIs ) {
        emit addNewTask(&task);
    } else if ( args.context==HOW_TO_DO::DO_Edit ) {
        emit ifaceToEditor(&task);
    } else{
        QString xml;
        bool show = false;
        // show SRC Creator widget
        CreateInterface *createIface = new CreateInterface(this);
        int result = createIface->exec();
        if ( createIface!=nullptr && result==QDialog::Accepted ) {
            xml = createIface->getXMLDescFileName();
            show = createIface->getShowing();
            QStringList data;
            data.append(tr("New Interface XML'ed"));
            data.append(QString(tr("to <a href='%1'>%1</a>")).arg(xml));
            QString msg = data.join(" ");
            msgRepeater(msg);
            if ( show ) QDesktopServices::openUrl(QUrl(xml));
        };
        createIface->deleteLater();
        if ( result==QDialog::Rejected ) return;
        //qDebug()<<xml<<"path"<<result;
        task.args.path = xml;
        emit addNewTask(&task);
    };
}
