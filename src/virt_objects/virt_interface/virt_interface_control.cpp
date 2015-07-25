#include "virt_interface_control.h"

VirtInterfaceControl::VirtInterfaceControl(QWidget *parent) :
    VirtEntityControl(parent)
{
    setObjectName("VirtIfaceControl");
    setWindowTitle("VirtIface Control");
    setWindowIcon(QIcon::fromTheme("network-wired"));
    virtIfaceModel = new VirtIfaceModel();
    entityList->setModel(virtIfaceModel);
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
    connect(toolBar, SIGNAL(fileForMethod(const QStringList&)),
            this, SLOT(newVirtEntityFromXML(const QStringList&)));
    connect(toolBar, SIGNAL(execMethod(const QStringList&)),
            this, SLOT(execAction(const QStringList&)));
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
    //disconnect(entityList, SIGNAL(doubleClicked(const QModelIndex&)),
    //           this, SLOT(entityDoubleClicked(const QModelIndex&)));
    disconnect(entityList, SIGNAL(customContextMenuRequested(const QPoint&)),
               this, SLOT(entityClicked(const QPoint&)));
    disconnect(toolBar, SIGNAL(fileForMethod(const QStringList&)),
               this, SLOT(newVirtEntityFromXML(const QStringList&)));
    disconnect(toolBar, SIGNAL(execMethod(const QStringList&)),
               this, SLOT(execAction(const QStringList&)));

    stopProcessing();

    delete toolBar;
    toolBar = NULL;

    if (virtIfaceModel!=NULL) {
        delete virtIfaceModel;
        virtIfaceModel = NULL;
    };

    if (entityList!=NULL) {
        delete entityList;
        entityList = NULL;
    };
}

/* public slots */
bool VirtInterfaceControl::getThreadState() const
{
    return true;
}
void VirtInterfaceControl::stopProcessing()
{
    // clear Network list
    while ( virtIfaceModel->DataList.count() ) {
        virtIfaceModel->removeRow(0);
    };
    virtIfaceModel->setHeaderData(0, Qt::Horizontal, QString("Name"), Qt::EditRole);
    setEnabled(false);

}
bool VirtInterfaceControl::setCurrentWorkConnect(virConnect *conn)
{
    stopProcessing();
    currWorkConnection = conn;
    toolBar->enableAutoReload();
    return true;
}
void VirtInterfaceControl::setListHeader(QString &connName)
{
    virtIfaceModel->setHeaderData(
                0, Qt::Horizontal,
                QString("Name (Conn: \"%1\")")
                .arg(connName), Qt::EditRole);
    currConnName = connName;
    setEnabled(true);
    // for initiation content
    reloadState();
}
void VirtInterfaceControl::resultReceiver(Result data)
{
    //qDebug()<<data.action<<data.msg<<"result";
    if ( data.action == GET_ALL_ENTITY ) {
        if ( data.msg.count() > virtIfaceModel->DataList.count() ) {
            int _diff = data.msg.count() - virtIfaceModel->DataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtIfaceModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( virtIfaceModel->DataList.count() > data.msg.count() ) {
            int _diff = virtIfaceModel->DataList.count() - data.msg.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtIfaceModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QString _data, data.msg) {
            QStringList chain = _data.split(DFR);
            if (chain.isEmpty()) continue;
            int count = chain.size();
            for (int j=0; j<count; j++) {
                virtIfaceModel->setData(virtIfaceModel->index(i,j), chain.at(j), Qt::EditRole);
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
    } else if ( data.action != GET_XML_DESCRIPTION ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
        };
        if ( data.result ) {
            reloadState();
            int row = -1;
            for ( int i=0; i<virtIfaceModel->DataList.count(); i++ ) {
                if ( virtIfaceModel->DataList.at(i)->getName()==data.name ) {
                    row = i;
                    break;
                };
            };
            // for different action's specified manipulation
            switch (data.action) {
            case IFACE_CHANGE_BEGIN:
                if (row+1)
                    virtIfaceModel->DataList.at(row)->setChanging("yes");
                break;
            case IFACE_CHANGE_COMMIT:
                if (row+1)
                    virtIfaceModel->DataList.at(row)->setChanging("");
                break;
            case IFACE_CHANGE_ROLLBACK:
                if (row+1)
                    virtIfaceModel->DataList.at(row)->setChanging("");
                break;
            default:
                break;
            }
        };
    };
}

/* private slots */
void VirtInterfaceControl::reloadState()
{
    TASK task;
    task.type = "iface";
    task.sourceConn = currWorkConnection;
    task.srcConName = currConnName;
    task.action     = GET_ALL_ENTITY;
    task.method     = "reloadVirtInterface";
    emit addNewTask(task);
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
    QStringList params;
    if ( idx.isValid() && virtIfaceModel->DataList.count()>idx.row() ) {
        //qDebug()<<virtIfaceModel->DataList.at(idx.row())->getName();
        params<<virtIfaceModel->DataList.at(idx.row())->getName();
        params<<virtIfaceModel->DataList.at(idx.row())->getMAC();
        params<<virtIfaceModel->DataList.at(idx.row())->getState();
        params<<virtIfaceModel->DataList.at(idx.row())->getChanging();
    } else {
        entityList->clearSelection();
    };
    bool state = toolBar->getAutoReloadState();
    IfaceControlMenu *ifaceControlMenu = new IfaceControlMenu(this, params, state);
    connect(ifaceControlMenu, SIGNAL(execMethod(const QStringList&)),
            this, SLOT(execAction(const QStringList&)));
    ifaceControlMenu->move(QCursor::pos());
    ifaceControlMenu->exec();
    disconnect(ifaceControlMenu, SIGNAL(execMethod(const QStringList&)),
               this, SLOT(execAction(const QStringList&)));
    ifaceControlMenu->deleteLater();
}
void VirtInterfaceControl::entityDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        qDebug()<<virtIfaceModel->DataList.at(index.row())->getName();
    }
}
void VirtInterfaceControl::execAction(const QStringList &l)
{
    QModelIndex idx = entityList->currentIndex();
    if ( idx.isValid() && virtIfaceModel->DataList.count()>idx.row() ) {
        QString networkName = virtIfaceModel->DataList.at(idx.row())->getName();
        TASK task;
        task.type = "iface";
        task.sourceConn = currWorkConnection;
        task.srcConName = currConnName;
        task.object     = networkName;
        if        ( l.first()=="startVirtInterface" ) {
            task.method = l.first();
            task.action = START_ENTITY;
            emit addNewTask(task);
        } else if ( l.first()=="destroyVirtInterface" ) {
            task.method = l.first();
            task.action = DESTROY_ENTITY;
            emit addNewTask(task);
        } else if ( l.first()=="undefineVirtInterface" ) {
            task.method = l.first();
            task.action = UNDEFINE_ENTITY;
            emit addNewTask(task);
        } else if ( l.first()=="changeBeginVirtInterface" ) {
            task.method = l.first();
            task.action = IFACE_CHANGE_BEGIN;
            emit addNewTask(task);
        } else if ( l.first()=="changeCommitVirtInterface" ) {
            task.method = l.first();
            task.action = IFACE_CHANGE_COMMIT;
            emit addNewTask(task);
        } else if ( l.first()=="changeRollbackVirtInterface" ) {
            task.method = l.first();
            task.action = IFACE_CHANGE_ROLLBACK;
            emit addNewTask(task);
        } else if ( l.first()=="getVirtInterfaceXMLDesc" ) {
            task.method = l.first();
            task.action = GET_XML_DESCRIPTION;
            emit addNewTask(task);
        } else if ( l.first()=="reloadVirtInterface" ) {
            reloadState();
        };
    } else if ( l.first()=="reloadVirtInterface" ) {
        reloadState();
    };
}
void VirtInterfaceControl::newVirtEntityFromXML(const QStringList &_args)
{
    QStringList args = _args;
    if ( !args.isEmpty() ) {
        TASK task;
        task.type = "iface";
        Actions act;
        QString actName;
        act = DEFINE_ENTITY;
        actName = "defineVirtInterface";
        args.removeFirst();
        if ( !args.isEmpty() ) {
            if ( args.first()=="manually" ) {
                QString xml;
                bool show = false;
                // show SRC Creator widget
                //qDebug()<<xml<<"path"<<result;
                task.args.path = xml;
            } else {
                QString xml = args.first();
                task.args.path = xml;
            };
            task.sourceConn = currWorkConnection;
            task.srcConName = currConnName;
            task.method     = actName;
            task.action     = act;
            emit addNewTask(task);
        };
    };
}
