#include "storage_pool_control_thread.h"

StoragePoolControlThread::StoragePoolControlThread(QObject *parent) :
    ControlThread(parent)
{
}

/* public slots */
void StoragePoolControlThread::stop() { keep_alive = false; }
void StoragePoolControlThread::execAction(uint _num, TASK _task)
{
    number = _num;
    task = _task;
    keep_alive = true;
    // for new virConnect usage create the new virConnectRef[erence]
    int ret = virConnectRef(task.sourceConn);
    if ( ret<0 ) {
        task.sourceConn = NULL;
        sendConnErrors();
        keep_alive = false;
    };
    if ( keep_alive && !isRunning() ) {
        if ( NULL!=task.sourceConn ) start();
        else {
            Result result;
            result.type   = "pool";
            result.number = number;
            result.action = _EMPTY_ACTION;
            emit resultData(result);
        };
    };
}

/* private slots */
void StoragePoolControlThread::run()
{
    Result result;
    switch (task.action) {
    case GET_ALL_ENTITY :
        result = getAllStoragePoolList();
        break;
    case CREATE_ENTITY :
        result = createStoragePool();
        break;
    case DEFINE_ENTITY :
        result = defineStoragePool();
        break;
    case START_ENTITY :
        result = startStoragePool();
        break;
    case DESTROY_ENTITY :
        result = destroyStoragePool();
        break;
    case UNDEFINE_ENTITY :
        result = undefineStoragePool();
        break;
    case CHANGE_ENTITY_AUTOSTART :
        result = changeAutoStartStoragePool();
        break;
    case DELETE_ENTITY :
        result = deleteStoragePool();
        break;
    case GET_XML_DESCRIPTION :
        result = getStoragePoolXMLDesc();
        break;
    default:
        break;
    };
    virConnectClose(task.sourceConn);
    result.type   = "pool";
    result.number = number;
    result.action = task.action;
    emit resultData(result);
}
Result StoragePoolControlThread::getAllStoragePoolList()
{
    Result result;
    QStringList storagePoolList;
    if ( task.sourceConn!=NULL && keep_alive ) {
        virStoragePoolPtr *storagePool = NULL;
        unsigned int flags = VIR_CONNECT_LIST_STORAGE_POOLS_ACTIVE |
                             VIR_CONNECT_LIST_STORAGE_POOLS_INACTIVE;
        int ret = virConnectListAllStoragePools(
                    task.sourceConn, &storagePool, flags);
        if ( ret<0 ) {
            sendConnErrors();
            result.result = false;
            result.msg = storagePoolList;
            return result;
        };

        // therefore correctly to use for() command, because storagePool[0] can not exist.
        for (int i = 0; i < ret; i++) {
            QStringList currentAttr;
            QString autostartStr;
            int is_autostart = 0;
            if (virStoragePoolGetAutostart(storagePool[i], &is_autostart) < 0) {
                autostartStr.append("no autostart");
            } else autostartStr.append( is_autostart ? "yes" : "no" );
            currentAttr<< QString::fromUtf8( virStoragePoolGetName(storagePool[i]) )
                       << QString( virStoragePoolIsActive(storagePool[i]) ? "active" : "inactive" )
                       << autostartStr
                       << QString( virStoragePoolIsPersistent(storagePool[i]) ? "yes" : "no" );
            storagePoolList.append(currentAttr.join(DFR));
            //qDebug()<<currentAttr;
            virStoragePoolFree(storagePool[i]);
        };
        free(storagePool);
        result.result = true;
    } else {
        result.result = false;
    };
    result.msg = storagePoolList;
    return result;
}
Result StoragePoolControlThread::createStoragePool()
{
    Result result;
    QString path = task.args.path;
    QByteArray xmlData;
    QFile f;
    f.setFileName(path);
    if ( !f.open(QIODevice::ReadOnly) ) {
        QString msg = QString("File \"%1\"\nnot opened.").arg(path);
        emit errorMsg( msg, number );
        result.result = false;
        return result;
    };
    xmlData = f.readAll();
    f.close();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStoragePoolPtr storagePool = virStoragePoolCreateXML(
                task.sourceConn, xmlData.data(), flags);
    if ( storagePool==NULL ) {
        sendConnErrors();
        result.result = false;
        return result;
    };
    result.name = QString().fromUtf8( virStoragePoolGetName(storagePool) );
    result.msg.append(QString("'<b>%1</b>' StoragePool from\n\"%2\"\nis Created.")
                  .arg(result.name).arg(path));
    virStoragePoolFree(storagePool);
    result.result = true;
    return result;
}
Result StoragePoolControlThread::defineStoragePool()
{
    Result result;
    QString path = task.args.path;
    QByteArray xmlData;
    QFile f;
    f.setFileName(path);
    if ( !f.open(QIODevice::ReadOnly) ) {
        QString msg = QString("File \"%1\"\nnot opened.").arg(path);
        emit errorMsg( msg, number );
        result.result = false;
        return result;
    };
    xmlData = f.readAll();
    f.close();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStoragePoolPtr storagePool = virStoragePoolDefineXML(
                task.sourceConn, xmlData.data(), flags);
    if ( storagePool==NULL ) {
        sendConnErrors();
        result.result = false;
        return result;
    };
    result.name = QString().fromUtf8( virStoragePoolGetName(storagePool) );
    result.msg.append(
                QString("'<b>%1</b>' StoragePool from\n\"%2\"\nis Defined.")
                .arg(result.name).arg(path));
    virStoragePoolFree(storagePool);
    result.result = true;
    return result;
}
Result StoragePoolControlThread::startStoragePool()
{
    Result result;
    QString name = task.object;
    unsigned int flags = VIR_CONNECT_LIST_STORAGE_POOLS_ACTIVE |
                         VIR_CONNECT_LIST_STORAGE_POOLS_INACTIVE;
    bool started = false;
    // flags: extra flags; not used yet, so callers should always pass 0
    flags = 0;
    virStoragePoolPtr storagePool = virStoragePoolLookupByName(
                task.sourceConn, name.toUtf8().data());
    if ( storagePool!=NULL ) {
        started = (virStoragePoolCreate(storagePool, flags)+1) ? true : false;
        if (!started) sendConnErrors();
        virStoragePoolFree(storagePool);
    } else sendConnErrors();
    result.msg.append(QString("'<b>%1</b>' StoragePool %2 Started.")
                  .arg(name).arg((started)?"":"don't"));
    result.name = name;
    result.result = started;
    return result;
}
Result StoragePoolControlThread::destroyStoragePool()
{
    Result result;
    QString name = task.object;
    bool deleted = false;
    virStoragePoolPtr storagePool = virStoragePoolLookupByName(
                task.sourceConn, name.toUtf8().data());
    if ( storagePool!=NULL ) {
        deleted = (virStoragePoolDestroy(storagePool)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virStoragePoolFree(storagePool);
    } else sendConnErrors();
    result.msg.append(QString("'<b>%1</b>' StoragePool %2 Destroyed.")
                  .arg(name).arg((deleted)?"":"don't"));
    result.name = name;
    result.result = deleted;
    return result;
}
Result StoragePoolControlThread::undefineStoragePool()
{
    Result result;
    QString name = task.object;
    bool deleted = false;
    virStoragePoolPtr storagePool = virStoragePoolLookupByName(
                task.sourceConn, name.toUtf8().data());
    if ( storagePool!=NULL ) {
        deleted = (virStoragePoolUndefine(storagePool)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virStoragePoolFree(storagePool);
    } else sendConnErrors();
    result.msg.append(QString("'<b>%1</b>' StoragePool %2 Undefined.")
                  .arg(name).arg((deleted)?"":"don't"));
    result.name = name;
    result.result = deleted;
    return result;
}
Result StoragePoolControlThread::changeAutoStartStoragePool()
{
    Result result;
    QString name = task.object;
    int autostart = task.args.sign;
    bool set = false;
    virStoragePoolPtr storagePool = virStoragePoolLookupByName(
                task.sourceConn, name.toUtf8().data());
    if ( storagePool!=NULL ) {
        set = (virStoragePoolSetAutostart(storagePool, autostart)+1) ? true : false;
        if (!set) sendConnErrors();
        virStoragePoolFree(storagePool);
    } else sendConnErrors();
    result.msg.append(QString("'<b>%1</b>' StoragePool autostart %2 Set.")
                  .arg(name).arg((set)?"":"don't"));
    result.name = name;
    result.result = set;
    return result;
}
Result StoragePoolControlThread::deleteStoragePool()
{
    Result result;
    QString name = task.object;
    uint flags = (task.args.sign)? VIR_STORAGE_POOL_DELETE_ZEROED
                                 : VIR_STORAGE_POOL_DELETE_NORMAL;
    bool deleted = false;
    virStoragePoolPtr storagePool = virStoragePoolLookupByName(
                task.sourceConn, name.toUtf8().data());
    if ( storagePool!=NULL ) {
        deleted = (virStoragePoolDelete(storagePool, flags)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virStoragePoolFree(storagePool);
    } else sendConnErrors();
    result.msg.append(QString("'<b>%1</b>' StoragePool %2 Deleted.")
                  .arg(name).arg((deleted)?"":"don't"));
    result.name = name;
    result.result = deleted;
    return result;
}
Result StoragePoolControlThread::getStoragePoolXMLDesc()
{
    Result result;
    QString name = task.object;
    bool read = false;
    char *Returns = NULL;
    virStoragePoolPtr storagePool = virStoragePoolLookupByName(task.sourceConn, name.toUtf8().data());
    if ( storagePool!=NULL ) {
        Returns = (virStoragePoolGetXMLDesc(storagePool, VIR_STORAGE_XML_INACTIVE));
        if ( Returns==NULL ) sendConnErrors();
        else read = true;
        virStoragePoolFree(storagePool);
    } else sendConnErrors();
    QTemporaryFile f;
    f.setAutoRemove(false);
    f.setFileTemplate(QString("%1%2XML_Desc-XXXXXX.xml")
                      .arg(QDir::tempPath()).arg(QDir::separator()));
    read = f.open();
    if (read) f.write(Returns);
    result.fileName.append(f.fileName());
    f.close();
    if ( Returns!=NULL ) free(Returns);
    result.msg.append(QString("'<b>%1</b>' StoragePool %2 XML'ed")
                  .arg(name).arg((read)?"":"don't"));
    result.name = name;
    result.result = read;
    return result;
}
