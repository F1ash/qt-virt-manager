#include "storage_pool_control_thread.h"
#include <QDomDocument>

StoragePoolControlThread::StoragePoolControlThread(QObject *parent) :
    ControlThread(parent)
{
}

/* public slots */
void StoragePoolControlThread::execAction(uint _num, TASK _task)
{
    number = _num;
    task = _task;
    keep_alive = false;
    if ( nullptr!=task.srcConnPtr ) {
        // for new virConnect usage create the new virConnectRef[erence]
        int ret = virConnectRef(*task.srcConnPtr);
        if ( ret<0 ) {
            task.srcConnPtr = nullptr;
            sendConnErrors();
        } else
            keep_alive = true;
    };
    if ( keep_alive && !isRunning() ) {
        ptr_ConnPtr = task.srcConnPtr;
        start();
    } else {
        Result result;
        result.type   = VIRT_ENTITY::VIRT_STORAGE_POOL;
        result.number = number;
        result.action = Actions::_NONE_ACTION;
        emit resultData(result);
    };
}

/* private slots */
void StoragePoolControlThread::run()
{
    Result result;
    switch (task.action) {
    case Actions::GET_ALL_ENTITY_STATE :
        result = getAllStoragePoolList();
        break;
    case Actions::GET_ALL_ENTITY_DATA0 :
        result = getAllStoragePoolDataList();
        break;
    case Actions::CREATE_ENTITY :
        result = createStoragePool();
        break;
    case Actions::DEFINE_ENTITY :
        result = defineStoragePool();
        break;
    case Actions::START_ENTITY :
        result = startStoragePool();
        break;
    case Actions::DESTROY_ENTITY :
        result = destroyStoragePool();
        break;
    case Actions::UNDEFINE_ENTITY :
        result = undefineStoragePool();
        break;
    case Actions::CHANGE_ENTITY_AUTOSTART :
        result = changeAutoStartStoragePool();
        break;
    case Actions::DELETE_ENTITY :
        result = deleteStoragePool();
        break;
    case Actions::GET_XML_DESCRIPTION :
        result = getStoragePoolXMLDesc();
        break;
    default:
        break;
    };
    // task.srcConnPtr reference will closed in destructor as ptr_ConnPtr
    //virConnectClose(*task.srcConnPtr);
    result.type   = VIRT_ENTITY::VIRT_STORAGE_POOL;
    result.number = number;
    result.action = task.action;
    emit resultData(result);
}
Result StoragePoolControlThread::getAllStoragePoolList()
{
    Result result;
    ACT_RESULT storagePoolList;
    if ( task.srcConnPtr!=nullptr && keep_alive ) {
        virStoragePoolPtr *storagePool = nullptr;
        unsigned int flags =
                VIR_CONNECT_LIST_STORAGE_POOLS_ACTIVE |
                VIR_CONNECT_LIST_STORAGE_POOLS_INACTIVE;
        int ret = virConnectListAllStoragePools(
                    *task.srcConnPtr, &storagePool, flags);
        if ( ret<0 ) {
            result.err = sendConnErrors();
            result.result = false;
            return result;
        };

        // therefore correctly to use for() command,
        // because storagePool[0] can not exist.
        for (int i = 0; i < ret; i++) {
            QVariantMap currentAttr;
            int is_autostart = 0;
            virStoragePoolGetAutostart(storagePool[i], &is_autostart);
            currentAttr.insert(
                        "name",
                        QString::fromUtf8(
                            virStoragePoolGetName(storagePool[i])));
            currentAttr.insert(
                        "active",
                        (virStoragePoolIsActive(storagePool[i]))
                        ? "active" : "inactive" );
            currentAttr.insert(
                        "auto", (is_autostart)? true : false);
            currentAttr.insert(
                        "persistent",
                        (virStoragePoolIsPersistent(storagePool[i]))
                        ? true : false );
            storagePoolList.append(currentAttr);
            //qDebug()<<currentAttr;
            virStoragePoolFree(storagePool[i]);
        };
        if (storagePool) free(storagePool);
        result.result = true;
    } else {
        result.result = false;
    };
    result.data       = storagePoolList;
    return result;
}
Result StoragePoolControlThread::getAllStoragePoolDataList()
{
    Result result;
    ACT_RESULT storagePoolDataList;
    if ( task.srcConnPtr!=nullptr && keep_alive ) {
        virStoragePoolPtr *storagePool = nullptr;
        unsigned int flags =
                VIR_CONNECT_LIST_STORAGE_POOLS_ACTIVE |
                VIR_CONNECT_LIST_STORAGE_POOLS_INACTIVE;
        int ret = virConnectListAllStoragePools(
                    *task.srcConnPtr, &storagePool, flags);
        if ( ret<0 ) {
            result.err = sendConnErrors();
            result.result = false;
            return result;
        };

        // therefore correctly to use for() command,
        // because storagePool[0] can not exist.
        for (int i = 0; i < ret; i++) {
            QVariantMap currentAttr;
            QString type, source, target;
            char *Returns = virStoragePoolGetXMLDesc(
                        storagePool[i], VIR_STORAGE_XML_INACTIVE);
            if ( Returns!=nullptr ) {
                QDomDocument doc;
                QString s;
                QTextStream str;
                str.setString(&s);
                doc.setContent(QString(Returns));
                QDomElement _pool, _el;
                _pool= doc.firstChildElement("pool");
                type = _pool.attribute("type");
                _el = _pool.firstChildElement("source");
                _el.save(str, 4);
                source = str.readAll();
                _el = _pool.firstChildElement("target");
                _el.save(str, 4);
                target = str.readAll();
                free(Returns);
            };
            currentAttr.insert(
                        "name",
                        QString::fromUtf8(
                              virStoragePoolGetName(storagePool[i])));
            currentAttr.insert(
                        "type", type);
            currentAttr.insert(
                        "source", source);
            currentAttr.insert(
                        "target", target);
            storagePoolDataList.append(currentAttr);
            //qDebug()<<currentAttr;
            virStoragePoolFree(storagePool[i]);
        };
        if (storagePool) free(storagePool);
        result.result = true;
    } else {
        result.result = false;
    };
    result.data = storagePoolDataList;
    return result;
}
Result StoragePoolControlThread::createStoragePool()
{
    Result result;
    QString path = task.args.path;
    QByteArray xmlData;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    QFile f;
    f.setFileName(path);
    if ( !f.open(QIODevice::ReadOnly) ) {
        QString msg = QString(tr("File \"%1\"\nnot opened.")).arg(path);
        emit errorMsg( msg, number );
        result.result = false;
        result.err = msg;
        return result;
    };
    xmlData = f.readAll();
    f.close();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStoragePoolPtr storagePool = virStoragePoolCreateXML(
                *task.srcConnPtr, xmlData.data(), flags);
    if ( storagePool==nullptr ) {
        result.err = sendConnErrors();
        result.result = false;
        return result;
    };
    result.name = QString::fromUtf8( virStoragePoolGetName(storagePool) );
    result.msg.append(QString(tr("'<b>%1</b>' StoragePool from\n\"%2\"\nis Created."))
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    QFile f;
    f.setFileName(path);
    if ( !f.open(QIODevice::ReadOnly) ) {
        QString msg = QString(tr("File \"%1\"\nnot opened.")).arg(path);
        emit errorMsg( msg, number );
        result.result = false;
        result.err = msg;
        return result;
    };
    xmlData = f.readAll();
    f.close();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStoragePoolPtr storagePool = virStoragePoolDefineXML(
                *task.srcConnPtr, xmlData.data(), flags);
    if ( storagePool==nullptr ) {
        result.err = sendConnErrors();
        result.result = false;
        return result;
    };
    result.name = QString::fromUtf8( virStoragePoolGetName(storagePool) );
    result.msg.append(
                QString(tr("'<b>%1</b>' StoragePool from\n\"%2\"\nis Defined."))
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    // flags: extra flags; not used yet, so callers should always pass 0
    flags = 0;
    virStoragePoolPtr storagePool = virStoragePoolLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( storagePool!=nullptr ) {
        started = (virStoragePoolCreate(storagePool, flags)+1) ? true : false;
        if (!started)
            result.err = sendConnErrors();
        virStoragePoolFree(storagePool);
    } else
        result.err = sendConnErrors();
    result.msg.append(QString(tr("'<b>%1</b>' StoragePool %2 Started."))
                  .arg(name).arg((started)? "": tr("don't")));
    result.name = name;
    result.result = started;
    return result;
}
Result StoragePoolControlThread::destroyStoragePool()
{
    Result result;
    QString name = task.object;
    bool deleted = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    virStoragePoolPtr storagePool = virStoragePoolLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( storagePool!=nullptr ) {
        deleted = (virStoragePoolDestroy(storagePool)+1) ? true : false;
        if (!deleted)
            result.err = sendConnErrors();
        virStoragePoolFree(storagePool);
    } else
        result.err = sendConnErrors();
    result.msg.append(QString(tr("'<b>%1</b>' StoragePool %2 Destroyed."))
                  .arg(name).arg((deleted)? "": tr("don't")));
    result.name = name;
    result.result = deleted;
    return result;
}
Result StoragePoolControlThread::undefineStoragePool()
{
    Result result;
    QString name = task.object;
    bool deleted = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    virStoragePoolPtr storagePool = virStoragePoolLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( storagePool!=nullptr ) {
        deleted = (virStoragePoolUndefine(storagePool)+1) ? true : false;
        if (!deleted)
            result.err = sendConnErrors();
        virStoragePoolFree(storagePool);
    } else
        result.err = sendConnErrors();
    result.msg.append(QString(tr("'<b>%1</b>' StoragePool %2 Undefined."))
                  .arg(name).arg((deleted)? "": tr("don't")));
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    virStoragePoolPtr storagePool = virStoragePoolLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( storagePool!=nullptr ) {
        set = (virStoragePoolSetAutostart(storagePool, autostart)+1) ? true : false;
        if (!set)
            result.err = sendConnErrors();
        virStoragePoolFree(storagePool);
    } else
        result.err = sendConnErrors();
    result.msg.append(QString(tr("'<b>%1</b>' StoragePool autostart %2 Set."))
                  .arg(name).arg((set)? "": tr("don't")));
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    virStoragePoolPtr storagePool = virStoragePoolLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( storagePool!=nullptr ) {
        deleted = (virStoragePoolDelete(storagePool, flags)+1) ? true : false;
        if (!deleted)
            result.err = sendConnErrors();
        virStoragePoolFree(storagePool);
    } else
        result.err = sendConnErrors();
    result.msg.append(QString(tr("'<b>%1</b>' StoragePool %2 Deleted."))
                  .arg(name).arg((deleted)? "": tr("don't")));
    result.name = name;
    result.result = deleted;
    return result;
}
Result StoragePoolControlThread::getStoragePoolXMLDesc()
{
    Result result;
    QString name = task.object;
    bool read = false;
    char *Returns = nullptr;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    virStoragePoolPtr storagePool = virStoragePoolLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( storagePool!=nullptr ) {
        Returns = virStoragePoolGetXMLDesc(storagePool, VIR_STORAGE_XML_INACTIVE);
        if ( Returns==nullptr )
            result.err = sendConnErrors();
        else read = true;
        virStoragePoolFree(storagePool);
    } else
        result.err = sendConnErrors();
    QTemporaryFile f;
    f.setAutoRemove(false);
    f.setFileTemplate(QString("%1%2XML_Desc-XXXXXX.xml")
                      .arg(QDir::tempPath()).arg(QDir::separator()));
    read = f.open();
    if (read) f.write(Returns);
    result.fileName.append(f.fileName());
    f.close();
    if ( Returns!=nullptr ) free(Returns);
    result.msg.append(QString(tr("'<b>%1</b>' StoragePool %2 XML'ed."))
                  .arg(name).arg((read)? "": tr("don't")));
    result.name = name;
    result.result = read;
    return result;
}
