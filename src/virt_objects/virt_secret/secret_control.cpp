#include "secret_control.h"

VirtSecretControl::VirtSecretControl(QWidget *parent) :
    VirtEntityControl(parent)
{
    setObjectName("VirtSecretControl");
    setWindowTitle("VirtSecret Control");
    setWindowIcon(QIcon::fromTheme("security-high"));
    virtSecretModel = new VirtSecretModel();
    entityList->setModel(virtSecretModel);
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
    connect(toolBar, SIGNAL(execMethod(const QStringList&)),
            this, SLOT(execAction(const QStringList&)));
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
                0, Qt::Horizontal, QString("UUID"), Qt::EditRole);

}
bool VirtSecretControl::setCurrentWorkConnect(virConnect *conn)
{
    stopProcessing();
    currWorkConnection = conn;
    toolBar->enableAutoReload();
    return true;
}
void VirtSecretControl::setListHeader(QString &connName)
{
    virtSecretModel->setHeaderData(
                0, Qt::Horizontal,
                QString("UUID (Conn: \"%1\")").arg(connName),
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
void VirtSecretControl::resultReceiver(Result data)
{
    //qDebug()<<data.action<<data.name<<"result";
    if ( data.action == GET_ALL_ENTITY_STATE ) {
        if ( data.msg.count() > virtSecretModel->DataList.count() ) {
            int _diff = data.msg.count() - virtSecretModel->DataList.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtSecretModel->insertRow(1);
                //qDebug()<<i<<"insert";
            };
        };
        if ( virtSecretModel->DataList.count() > data.msg.count() ) {
            int _diff = virtSecretModel->DataList.count() - data.msg.count();
            for ( int i = 0; i<_diff; i++ ) {
                virtSecretModel->removeRow(0);
                //qDebug()<<i<<"remove";
            };
        };
        int i = 0;
        foreach (QString _data, data.msg) {
            QStringList chain = _data.split(DFR);
            if (chain.isEmpty()) continue;
            /*
            int count = chain.size();
            for (int j=0; j<count; j++) {
                virtSecretModel->setData(virtSecretModel->index(i,j), chain.at(j), Qt::EditRole);
            };
            */
            virtSecretModel->DataList.at(i)->setUUID(chain.at(0));
            virtSecretModel->DataList.at(i)->setUsageID(chain.at(1));
            virtSecretModel->DataList.at(i)->setType(chain.at(2));
            virtSecretModel->DataList.at(i)->setDescription(chain.at(3));
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
void VirtSecretControl::reloadState()
{
    TASK task;
    task.type = "secret";
    task.sourceConn = currWorkConnection;
    task.srcConName = currConnName;
    task.action     = GET_ALL_ENTITY_STATE;
    task.method     = "reloadVirtSecret";
    emit addNewTask(task);
}
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
    connect(secControlMenu, SIGNAL(execMethod(const QStringList&)),
            this, SLOT(execAction(const QStringList&)));
    secControlMenu->move(QCursor::pos());
    secControlMenu->exec();
    disconnect(secControlMenu, SIGNAL(execMethod(const QStringList&)),
               this, SLOT(execAction(const QStringList&)));
    secControlMenu->deleteLater();
}
void VirtSecretControl::entityDoubleClicked(const QModelIndex &index)
{
    if ( index.isValid() ) {
        qDebug()<<virtSecretModel->DataList.at(index.row())->getUUID();
    }
}
void VirtSecretControl::execAction(const QStringList &l)
{
    TASK task;
    task.type = "secret";
    task.sourceConn = currWorkConnection;
    task.srcConName = currConnName;
    QModelIndex idx = entityList->currentIndex();
    if ( idx.isValid() && virtSecretModel->DataList.count()>idx.row() ) {
        QString uuid = virtSecretModel->DataList.at(idx.row())->getUUID();
        task.object = uuid;
        if        ( l.first()=="defineVirtSecret" ) {
            QString xml;
            bool show = false;
            // show Secret Creator widget
            CreateVirtSecret *createVirtSec = new CreateVirtSecret(this, currWorkConnection);
            int result = createVirtSec->exec();
            if ( createVirtSec!=NULL && result==QDialog::Accepted ) {
                xml = createVirtSec->getXMLDescFileName();
                show = createVirtSec->getShowing();
                QStringList data;
                data.append("New Secret XML'ed");
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
            delete createVirtSec;
            createVirtSec = NULL;
            //qDebug()<<xml<<"path"<<result;
        } else if ( l.first()=="undefineVirtSecret" ) {
            task.action     = UNDEFINE_ENTITY;
            task.method     = l.first();
            emit addNewTask(task);
        } else if ( l.first()=="getVirtSecretXMLDesc" ) {
            task.action     = GET_XML_DESCRIPTION;
            task.method     = l.first();
            emit addNewTask(task);
        } else if ( l.first()=="reloadVirtSecret" ) {
            reloadState();
        };
    } else if ( l.first()=="reloadVirtSecret" ) {
        reloadState();
    } else if ( l.first()=="defineVirtSecret" ) {
        QString xml;
        bool show = false;
        // show Secret Creator widget
        CreateVirtSecret *createVirtSec = new CreateVirtSecret(this, currWorkConnection);
        int result = createVirtSec->exec();
        if ( createVirtSec!=NULL && result==QDialog::Accepted ) {
            xml = createVirtSec->getXMLDescFileName();
            show = createVirtSec->getShowing();
            QStringList data;
            data.append("New Secret XML'ed");
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
        delete createVirtSec;
        createVirtSec = NULL;
        //qDebug()<<xml<<"path"<<result;
    };
}
