#include "nwfilter_control.h"

VirtNWFilterControl::VirtNWFilterControl(QWidget *parent) :
    VirtEntityControl(parent)
{
    setObjectName("VirtNWFilterControl");
    setWindowTitle("VirtNWFilter Control");
    setWindowIcon(QIcon::fromTheme("nwfilter"));
    virtNWFilterModel = new VirtNWFilterModel();
    entityList->setModel(virtNWFilterModel);
    nwfilterListDlg = new NWFilterListDelegate();
    entityList->setItemDelegate(nwfilterListDlg);
    //connect(entityList, SIGNAL(doubleClicked(const QModelIndex&)),
    //        this, SLOT(entityDoubleClicked(const QModelIndex&)));
    connect(entityList, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(entityClicked(const QPoint&)));
    settings.beginGroup("VirtNWFilterControl");
    entityList->setColumnWidth(0, settings.value("column0", 132).toInt());
    entityList->setColumnWidth(1, settings.value("column1", 32).toInt());
    int area_int = settings.value("ToolBarArea", 4).toInt();
    settings.endGroup();
    toolBar = new VirtNWFilterToolBar(this);
    addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
    connect(toolBar, SIGNAL(execMethod(const QStringList&)),
            this, SLOT(execAction(const QStringList&)));
}
VirtNWFilterControl::~VirtNWFilterControl()
{
    settings.beginGroup("VirtNWFilterControl");
    settings.setValue("column0", entityList->columnWidth(0));
    settings.setValue("column1", entityList->columnWidth(1));
    settings.setValue("ToolBarArea", toolBarArea(toolBar));
    settings.endGroup();
    settings.sync();

    stopProcessing();
}

/* public slots */
void VirtNWFilterControl::stopProcessing()
{
    setEnabled(false);
    // clear NWFilter list
    while ( virtNWFilterModel->DataList.count() ) {
        virtNWFilterModel->removeRow(0);
    };
    virtNWFilterModel->setHeaderData(
                0, Qt::Horizontal, QString("Name"), Qt::EditRole);

}
bool VirtNWFilterControl::setCurrentWorkConnect(virConnectPtr *connPtrPtr)
{
    stopProcessing();
    ptr_ConnPtr = connPtrPtr;
    toolBar->enableAutoReload();
    return true;
}
void VirtNWFilterControl::setListHeader(QString &connName)
{
    virtNWFilterModel->setHeaderData(
                0, Qt::Horizontal,
                QString("NWFilter in [ %1 ]").arg(connName),
                Qt::EditRole);
    currConnName = connName;
    setEnabled(true);
    // for initiation content
    reloadState();
}
QString VirtNWFilterControl::getCurrentNFName() const
{
    QModelIndex index = entityList->currentIndex();
    if ( !index.isValid() ) return QString();
    return virtNWFilterModel->DataList.at(index.row())->getName();
}
QString VirtNWFilterControl::getCurrentNFUUID() const
{
    QModelIndex index = entityList->currentIndex();
    if ( !index.isValid() ) return QString();
    return virtNWFilterModel->DataList.at(index.row())->getUUID();
}
void VirtNWFilterControl::resultReceiver(Result data)
{
    //qDebug()<<data.action<<data.name<<"result";
    if ( data.action == GET_ALL_ENTITY_STATE ) {
        entityList->setEnabled(true);
        if ( data.msg.count() > virtNWFilterModel->DataList.count() ) {
            int _diff = data.msg.count() - virtNWFilterModel->DataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtNWFilterModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( virtNWFilterModel->DataList.count() > data.msg.count() ) {
            int _diff = virtNWFilterModel->DataList.count() - data.msg.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtNWFilterModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QString _data, data.msg) {
            QStringList chain = _data.split(DFR);
            if (chain.isEmpty()) continue;
            virtNWFilterModel->DataList.at(i)->setName(chain.at(0));
            virtNWFilterModel->DataList.at(i)->setUUID(chain.at(1));
            i++;
        };
    } else if ( data.action == GET_XML_DESCRIPTION ) {
        QString xml = data.fileName;
        data.msg.append(QString("to <a href='%1'>%1</a>").arg(xml));
        QString msg = data.msg.join(" ");
        msgRepeater(msg);
        if ( data.result )
            QDesktopServices::openUrl(QUrl(xml));
    } else if ( data.action < GET_XML_DESCRIPTION ) {
        if ( !data.msg.isEmpty() ) {
            QString msg = data.msg.join(" ");
            msgRepeater(msg);
        };
        if ( data.result ) {
            reloadState();
            // for different action's specified manipulation
            switch (data.action) {
            case _EMPTY_ACTION:
                // some job;
                break;
            default:
                break;
            }
        };
    };
}

/* private slots */
void VirtNWFilterControl::reloadState()
{
    entityList->setEnabled(false);
    entityList->clearSelection();
    TASK task;
    task.type = "nwfilter";
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    task.action     = GET_ALL_ENTITY_STATE;
    task.method     = "reloadVirtNWFilter";
    emit addNewTask(task);
}
void VirtNWFilterControl::changeDockVisibility()
{
    toolBar->setEnabled( !toolBar->isEnabled() );
    entityList->setEnabled( !entityList->isEnabled() );
}
void VirtNWFilterControl::entityClicked(const QPoint &p)
{
    //qDebug()<<"custom Menu request";
    QModelIndex idx = entityList->indexAt(p);
    QStringList params;
    if ( idx.isValid() && virtNWFilterModel->DataList.count()>idx.row() ) {
        //qDebug()<<virtNWFilterModel->DataList.at(idx.row())->getName();
        params<<virtNWFilterModel->DataList.at(idx.row())->getUUID();
        params<<virtNWFilterModel->DataList.at(idx.row())->getName();
    } else {
        entityList->clearSelection();
    };
    bool state = toolBar->getAutoReloadState();
    VirtNWFilterControlMenu *secControlMenu =
            new VirtNWFilterControlMenu(this, params, state);
    connect(secControlMenu, SIGNAL(execMethod(const QStringList&)),
            this, SLOT(execAction(const QStringList&)));
    secControlMenu->move(QCursor::pos());
    secControlMenu->exec();
    disconnect(secControlMenu, SIGNAL(execMethod(const QStringList&)),
               this, SLOT(execAction(const QStringList&)));
    secControlMenu->deleteLater();
}
void VirtNWFilterControl::entityDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        qDebug()<<virtNWFilterModel->DataList.at(index.row())->getUUID();
    }
}
void VirtNWFilterControl::execAction(const QStringList &l)
{
    TASK task;
    task.type = "nwfilter";
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = currConnName;
    QModelIndex idx = entityList->currentIndex();
    if ( idx.isValid() && virtNWFilterModel->DataList.count()>idx.row() ) {
        QString uuid = virtNWFilterModel->DataList.at(idx.row())->getUUID();
        task.object = uuid;
        if        ( l.first()=="defineVirtNWFilter" ) {
            QString xml;
            bool show = false;
            // show NWFilter Creator widget
            CreateVirtSecret *createVirtSec = new CreateVirtSecret(this, ptr_ConnPtr);
            int result = createVirtSec->exec();
            if ( createVirtSec!=nullptr && result==QDialog::Accepted ) {
                xml = createVirtSec->getXMLDescFileName();
                show = createVirtSec->getShowing();
                QStringList data;
                data.append("New NWFilter XML'ed");
                data.append(QString("to <a href='%1'>%1</a>").arg(xml));
                QString msg = data.join(" ");
                msgRepeater(msg);
                if ( show ) QDesktopServices::openUrl(QUrl(xml));
                task.action     = DEFINE_ENTITY;
                task.method     = l.first();
                task.args.path  = xml;
                task.secret->setSecretValue(
                            createVirtSec->getSecretValue());
                emit addNewTask(task);
            };
            createVirtSec->deleteLater();
            //qDebug()<<xml<<"path"<<result;
        } else if ( l.first()=="undefineVirtNWFilter" ) {
            task.action     = UNDEFINE_ENTITY;
            task.method     = l.first();
            emit addNewTask(task);
        } else if ( l.first()=="getVirtNWFilterXMLDesc" ) {
            task.action     = GET_XML_DESCRIPTION;
            task.method     = l.first();
            emit addNewTask(task);
        } else if ( l.first()=="reloadVirtNWFilter" ) {
            reloadState();
        };
    } else if ( l.first()=="reloadVirtNWFilter" ) {
        reloadState();
    } else if ( l.first()=="defineVirtNWFilter" ) {
        QString xml;
        bool show = false;
        // show NWFilter Creator widget
        CreateVirtSecret *createVirtSec = new CreateVirtSecret(this, ptr_ConnPtr);
        int result = createVirtSec->exec();
        if ( createVirtSec!=nullptr && result==QDialog::Accepted ) {
            xml = createVirtSec->getXMLDescFileName();
            show = createVirtSec->getShowing();
            QStringList data;
            data.append("New NWFilter XML'ed");
            data.append(QString("to <a href='%1'>%1</a>").arg(xml));
            QString msg = data.join(" ");
            msgRepeater(msg);
            if ( show ) QDesktopServices::openUrl(QUrl(xml));
            task.action     = DEFINE_ENTITY;
            task.method     = l.first();
            task.args.path  = xml;
            task.secret->setSecretValue(
                        createVirtSec->getSecretValue());
            emit addNewTask(task);
        };
        createVirtSec->deleteLater();
        //qDebug()<<xml<<"path"<<result;
    };
}
