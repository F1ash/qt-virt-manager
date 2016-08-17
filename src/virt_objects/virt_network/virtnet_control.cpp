#include "virtnet_control.h"

VirtNetControl::VirtNetControl(QWidget *parent) :
    VirtEntityControl(parent)
{
    setObjectName("VirtNetControl");
    setWindowTitle("VirtNet Control");
    setWindowIcon(QIcon::fromTheme("network"));
    virtNetModel = new VirtNetModel();
    entityList->setModel(virtNetModel);
    netListDlg = new NetworkListDelegate();
    entityList->setItemDelegate(netListDlg);
    //connect(entityList, SIGNAL(doubleClicked(const QModelIndex&)),
    //        this, SLOT(entityDoubleClicked(const QModelIndex&)));
    connect(entityList, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(entityClicked(const QPoint&)));
    settings.beginGroup("VirtNetControl");
    entityList->setColumnWidth(0, settings.value("column0", 132).toInt());
    entityList->setColumnWidth(1, settings.value("column1", 32).toInt());
    entityList->setColumnWidth(2, settings.value("column2", 32).toInt());
    entityList->setColumnWidth(3, settings.value("column3", 32).toInt());
    int area_int = settings.value("ToolBarArea", 4).toInt();
    settings.endGroup();
    toolBar = new VirtNetToolBar(this);
    addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
    connect(toolBar, SIGNAL(fileForMethod(const Act_Param&)),
            this, SLOT(newVirtEntityFromXML(const Act_Param&)));
    connect(toolBar, SIGNAL(execMethod(const Act_Param&)),
            this, SLOT(execAction(const Act_Param&)));
}
VirtNetControl::~VirtNetControl()
{
    settings.beginGroup("VirtNetControl");
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
void VirtNetControl::stopProcessing()
{
    setEnabled(false);
    // clear Network list
    while ( virtNetModel->DataList.count() ) {
        virtNetModel->removeRow(0);
    };
    virtNetModel->setHeaderData(
                0,
                Qt::Horizontal,
                QString("Name"),
                Qt::EditRole);

}
bool VirtNetControl::setCurrentWorkConnect(virConnectPtr *connPtrPtr)
{
    stopProcessing();
    ptr_ConnPtr = connPtrPtr;
    toolBar->enableAutoReload();
    return true;
}
void VirtNetControl::setListHeader(const QString &connName)
{
    virtNetModel->setHeaderData(
                0,
                Qt::Horizontal,
                QString("Network in [ %1 ]").arg(connName),
                Qt::EditRole);
    currConnName = connName;
    setEnabled(true);
    // for initiation content
    reloadState();
}
void VirtNetControl::resultReceiver(Result data)
{
    //qDebug()<<data.action<<data.name<<"result";
    if ( data.action == GET_ALL_ENTITY_STATE ) {
        entityList->setEnabled(true);
        if ( data.msg.count() > virtNetModel->DataList.count() ) {
            int _diff = data.msg.count() - virtNetModel->DataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtNetModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( virtNetModel->DataList.count() > data.msg.count() ) {
            int _diff = virtNetModel->DataList.count() - data.msg.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtNetModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QString _data, data.msg) {
            QStringList chain = _data.split(DFR);
            if (chain.isEmpty()) continue;
            int count = chain.size();
            for (int j=0; j<count; j++) {
                virtNetModel->setData(virtNetModel->index(i,j), chain.at(j), Qt::EditRole);
            };
            i++;
        };
    } else if ( data.action == GET_XML_DESCRIPTION ) {
        QString xml = data.fileName;
        data.msg.append(QString("to <a href='%1'>%1</a>").arg(xml));
        QString msg = data.msg.join(" ");
        msgRepeater(msg);
        if ( data.result )
            QDesktopServices::openUrl(QUrl(xml));
    } else if ( data.action == EDIT_ENTITY ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
        };
        if ( data.result ) {
            // show SRC Creator widget in Edit-mode
            TASK task;
            task.type       = VIRT_NETWORK;
            task.srcConnPtr = ptr_ConnPtr;
            task.srcConName = currConnName;
            task.object     = data.name;
            task.args.path  = data.fileName;
            task.method     = editEntity;
            task.action     = DEFINE_ENTITY;
            emit networkToEditor(task);
        };
    } else if ( data.action < GET_XML_DESCRIPTION ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
        };
        if ( data.result ) {
            reloadState();
            // for different action's specified manipulation
            switch (data.action) {
            case _NONE_ACTION:
                // some job;
                break;
            default:
                break;
            }
        };
    };
}

/* private slots */
void VirtNetControl::reloadState()
{
    entityList->setEnabled(false);
    entityList->clearSelection();
    TASK task;
    task.type = VIRT_NETWORK;
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    task.action     = GET_ALL_ENTITY_STATE;
    task.method     = reloadEntity;
    emit addNewTask(task);
}
void VirtNetControl::changeDockVisibility()
{
    toolBar->setEnabled( !toolBar->isEnabled() );
    entityList->setEnabled( !entityList->isEnabled() );
}
void VirtNetControl::entityClicked(const QPoint &p)
{
    //qDebug()<<"custom Menu request";
    QModelIndex idx = entityList->indexAt(p);
    QStringList params;
    if ( idx.isValid() && virtNetModel->DataList.count()>idx.row() ) {
        //qDebug()<<virtNetModel->DataList.at(idx.row())->getName();
        params<<virtNetModel->DataList.at(idx.row())->getName();
        params<<virtNetModel->DataList.at(idx.row())->getState();
        params<<virtNetModel->DataList.at(idx.row())->getAutostart();
        params<<virtNetModel->DataList.at(idx.row())->getPersistent();
    } else {
        entityList->clearSelection();
    };
    bool state = toolBar->getAutoReloadState();
    VirtNetControlMenu *netControlMenu =
            new VirtNetControlMenu(this, params, state);
    connect(netControlMenu, SIGNAL(execMethod(const Act_Param&)),
            this, SLOT(execAction(const Act_Param&)));
    netControlMenu->move(QCursor::pos());
    netControlMenu->exec();
    disconnect(netControlMenu, SIGNAL(execMethod(const Act_Param&)),
               this, SLOT(execAction(const Act_Param&)));
    netControlMenu->deleteLater();
}
void VirtNetControl::entityDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        qDebug()<<virtNetModel->DataList.at(index.row())->getName();
    }
}
void VirtNetControl::execAction(const Act_Param &param)
{
    QModelIndex idx = entityList->currentIndex();
    if ( idx.isValid() && virtNetModel->DataList.count()>idx.row() ) {
        QString networkName = virtNetModel->DataList.at(idx.row())->getName();
        TASK task;
        task.type       = VIRT_NETWORK;
        task.srcConnPtr = ptr_ConnPtr;
        task.srcConName = currConnName;
        task.object     = networkName;
        task.method     = param.method;
        if        ( param.method==startEntity ) {
            task.action = START_ENTITY;
            emit addNewTask(task);
        } else if ( param.method==destroyEntity ) {
            task.action = DESTROY_ENTITY;
            emit addNewTask(task);
        } else if ( param.method==defineEntity ) {
            task.action = DEFINE_ENTITY;
            emit addNewTask(task);
        } else if ( param.method==undefineEntity ) {
            task.action = UNDEFINE_ENTITY;
            emit addNewTask(task);
        } else if ( param.method==setAutostartEntity ) {
            /* set the opposite value */
            uint autostartState =
                (virtNetModel->DataList.at(idx.row())->getAutostart()=="yes")
                 ? 0 : 1;
            task.action = CHANGE_ENTITY_AUTOSTART;
            task.args.sign = autostartState;
            emit addNewTask(task);
        } else if ( param.method==editEntity ) {
            task.action     = EDIT_ENTITY;
            emit addNewTask(task);
        } else if ( param.method==getEntityXMLDesc ) {
            task.action = GET_XML_DESCRIPTION;
            emit addNewTask(task);
        } else if ( param.method==reloadEntity ) {
            reloadState();
        };
    } else if ( param.method==reloadEntity ) {
        reloadState();
    };
}
void VirtNetControl::newVirtEntityFromXML(const Act_Param &args)
{
    TASK task;
    task.type = VIRT_NETWORK;
    Methods method;
    if ( args.act==CREATE_ENTITY ) {
        method = createEntity;
    } else {
        method = defineEntity;
    };
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    task.method     = method;
    task.action     = args.act;
    task.args.path  = args.path;
    if ( args.context==DO_AsIs ) {
        emit addNewTask(task);
    } else {
        emit networkToEditor(task);
    };
}
