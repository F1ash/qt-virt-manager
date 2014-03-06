#include "storage_pool_control_thread.h"

StoragePoolControlThread::StoragePoolControlThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<StoragePoolActions>("StoragePoolActions");
}

/* public slots */
bool StoragePoolControlThread::setCurrentWorkConnect(virConnectPtr conn)
{
    keep_alive = true;
    currWorkConnect = conn;
    //qDebug()<<"net_thread"<<currWorkConnect;
}
void StoragePoolControlThread::stop() { keep_alive = false; }
void StoragePoolControlThread::execAction(StoragePoolActions act, QStringList _args)
{
    action = StPOOL_EMPTY_ACTION;
    args.clear();
    if ( keep_alive && !isRunning() ) {
        action = act;
        args = _args;
        start();
    };
}

/* private slots */
void StoragePoolControlThread::run()
{
    QStringList result;
    switch (action) {
    case GET_ALL_StPOOL :
        result.append(getAllStoragePoolList());
        break;
    case CREATE_StPOOL :
        result.append(createStoragePool());
        break;
    case DEFINE_StPOOL :
        result.append(defineStoragePool());
        break;
    case START_StPOOL :
        result.append(startStoragePool());
        break;
    case DESTROY_StPOOL :
        result.append(destroyStoragePool());
        break;
    case UNDEFINE_StPOOL :
        result.append(undefineStoragePool());
        break;
    case CHANGE_StPOOL_AUTOSTART :
        result.append(changeAutoStartStoragePool());
        break;
    case GET_StPOOL_XML_DESC :
        result.append(getStoragePoolXMLDesc());
        break;
    default:
        break;
    };
    emit resultData(action, result);
}
QStringList StoragePoolControlThread::getAllStoragePoolList()
{
    QStringList storagePoolList;
    if ( currWorkConnect!=NULL && keep_alive ) {
        virStoragePoolPtr *storagePool;
        unsigned int flags = VIR_CONNECT_LIST_STORAGE_POOLS_ACTIVE |
                             VIR_CONNECT_LIST_STORAGE_POOLS_INACTIVE;
        int ret = virConnectListAllStoragePools( currWorkConnect, &storagePool, flags);
        if ( ret<0 ) {
            sendConnErrors();
            free(storagePool);
            return storagePoolList;
        };

        int i = 0;
        while ( storagePool[i] != NULL ) {
            QStringList currentAttr;
            QString autostartStr;
            int is_autostart = 0;
            if (virStoragePoolGetAutostart(storagePool[i], &is_autostart) < 0) {
                autostartStr.append("no autostart");
            } else autostartStr.append( is_autostart ? "yes" : "no" );
            currentAttr<< QString( virStoragePoolGetName(storagePool[i]) )
                       << QString( virStoragePoolIsActive(storagePool[i]) ? "active" : "inactive" )
                       << autostartStr
                       << QString( virStoragePoolIsPersistent(storagePool[i]) ? "yes" : "no" );
            storagePoolList.append(currentAttr.join(" "));
            //qDebug()<<currentAttr;
            virStoragePoolFree(storagePool[i]);
            i++;
        };
        free(storagePool);
    };
    return storagePoolList;
}
QStringList StoragePoolControlThread::createStoragePool()
{
    QStringList result;
    QString path = args.first();
    QByteArray xmlData;
    QFile f;
    f.setFileName(path);
    if ( !f.open(QIODevice::ReadOnly) ) {
        emit errorMsg( QString("File \"%1\"\nnot opened.").arg(path) );
        return result;
    };
    xmlData = f.readAll();
    f.close();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStoragePoolPtr storagePool = virStoragePoolCreateXML(currWorkConnect, xmlData.data(), flags);
    if ( storagePool==NULL ) {
        sendConnErrors();
        return result;
    };
    result.append(QString("'%1' StoragePool from\n\"%2\"\nis created.").arg(virStoragePoolGetName(storagePool)).arg(path));
    virStoragePoolFree(storagePool);
    return result;
}
QStringList StoragePoolControlThread::defineStoragePool()
{
    QStringList result;
    QString path = args.first();
    QByteArray xmlData;
    QFile f;
    f.setFileName(path);
    if ( !f.open(QIODevice::ReadOnly) ) {
        emit errorMsg( QString("File \"%1\"\nnot opened.").arg(path) );
        return result;
    };
    xmlData = f.readAll();
    f.close();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStoragePoolPtr storagePool = virStoragePoolDefineXML(currWorkConnect, xmlData.data(), flags);
    if ( storagePool==NULL ) {
        sendConnErrors();
        return result;
    };
    result.append(QString("'%1' StoragePool from\n\"%2\"\nis defined.").arg(virStoragePoolGetName(storagePool)).arg(path));
    virStoragePoolFree(storagePool);
    return result;
}
QStringList StoragePoolControlThread::startStoragePool()
{
    QStringList result;
    QString name = args.first();
    virStoragePoolPtr *storagePool;
    unsigned int flags = VIR_CONNECT_LIST_STORAGE_POOLS_ACTIVE |
                         VIR_CONNECT_LIST_STORAGE_POOLS_INACTIVE;
    int ret = virConnectListAllStoragePools( currWorkConnect, &storagePool, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(storagePool);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));

    int i = 0;
    bool started = false;
    // flags: extra flags; not used yet, so callers should always pass 0
    flags = 0;
    while ( storagePool[i] != NULL ) {
        QString currNetName = QString( virStoragePoolGetName(storagePool[i]) );
        if ( !started && currNetName==name ) {
            started = (virStoragePoolCreate(storagePool[i], flags)+1) ? true : false;
            if (!started) sendGlobalErrors();
        };
        virStoragePoolFree(storagePool[i]);
        i++;
    };
    free(storagePool);
    result.append(QString("'%1' StoragePool %2 Started.").arg(name).arg((started)?"":"don't"));
    return result;
}
QStringList StoragePoolControlThread::destroyStoragePool()
{
    QStringList result;
    QString name = args.first();
    virStoragePoolPtr *storagePool;
    unsigned int flags = VIR_CONNECT_LIST_STORAGE_POOLS_ACTIVE |
                         VIR_CONNECT_LIST_STORAGE_POOLS_INACTIVE;
    int ret = virConnectListAllStoragePools( currWorkConnect, &storagePool, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(storagePool);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));

    int i = 0;
    bool deleted = false;
    while ( storagePool[i] != NULL ) {
        QString currDomName = QString( virStoragePoolGetName(storagePool[i]) );
        if ( !deleted && currDomName==name ) {
            deleted = (virStoragePoolDestroy(storagePool[i])+1) ? true : false;
            if (!deleted) sendGlobalErrors();
        };
        qDebug()<<QVariant(deleted).toString()<<currDomName<<name;
        virStoragePoolFree(storagePool[i]);
        i++;
    };
    free(storagePool);
    result.append(QString("'%1' StoragePool %2 Destroyed.").arg(name).arg((deleted)?"":"don't"));
    return result;
}
QStringList StoragePoolControlThread::undefineStoragePool()
{
    QStringList result;
    QString name = args.first();
    virStoragePoolPtr *storagePool;
    unsigned int flags = VIR_CONNECT_LIST_STORAGE_POOLS_ACTIVE |
                         VIR_CONNECT_LIST_STORAGE_POOLS_INACTIVE;
    int ret = virConnectListAllStoragePools( currWorkConnect, &storagePool, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(storagePool);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));

    int i = 0;
    bool deleted = false;
    while ( storagePool[i] != NULL ) {
        QString currDomName = QString( virStoragePoolGetName(storagePool[i]) );
        if ( !deleted && currDomName==name ) {
            deleted = (virStoragePoolUndefine(storagePool[i])+1) ? true : false;
            if (!deleted) sendGlobalErrors();
        };
        qDebug()<<QVariant(deleted).toString()<<currDomName<<name;
        virStoragePoolFree(storagePool[i]);
        i++;
    };
    free(storagePool);
    result.append(QString("'%1' StoragePool %2 Undefined.").arg(name).arg((deleted)?"":"don't"));
    return result;
}
QStringList StoragePoolControlThread::changeAutoStartStoragePool()
{
    QStringList result;
    QString name = args.first();
    int autostart;
    if ( args.count()<2 || args.at(1).isEmpty() ) {
        result.append("Incorrect parameters.");
        return result;
    } else {
        bool converted;
        int res = args.at(1).toInt(&converted);
        if (converted) autostart = (res) ? 1 : 0;
        else {
            result.append("Incorrect parameters.");
            return result;
        };
    };
    virStoragePoolPtr *storagePool;
    unsigned int flags = VIR_CONNECT_LIST_STORAGE_POOLS_ACTIVE |
                         VIR_CONNECT_LIST_STORAGE_POOLS_INACTIVE;
    int ret = virConnectListAllStoragePools( currWorkConnect, &storagePool, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(storagePool);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));

    int i = 0;
    bool set = false;
    while ( storagePool[i] != NULL ) {
        QString currNetName = QString( virStoragePoolGetName(storagePool[i]) );
        if ( !set && currNetName==name ) {
            set = (virStoragePoolSetAutostart(storagePool[i], autostart)+1) ? true : false;
            if (!set) sendGlobalErrors();
        };
        virStoragePoolFree(storagePool[i]);
        i++;
    };
    free(storagePool);
    result.append(QString("'%1' StoragePool autostart %2 Set.").arg(name).arg((set)?"":"don't"));
    return result;
}
QStringList StoragePoolControlThread::getStoragePoolXMLDesc()
{
    QStringList result;
    QString name = args.first();
    virStoragePoolPtr *storagePool;
    unsigned int flags = VIR_CONNECT_LIST_STORAGE_POOLS_ACTIVE |
                         VIR_CONNECT_LIST_STORAGE_POOLS_INACTIVE;
    int ret = virConnectListAllStoragePools( currWorkConnect, &storagePool, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(storagePool);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));

    int i = 0;
    bool read = false;
    char *Returns = NULL;
    while ( storagePool[i] != NULL ) {
        QString currNetName = QString( virStoragePoolGetName(storagePool[i]) );
        if ( !read && currNetName==name ) {
            Returns = (virStoragePoolGetXMLDesc(storagePool[i], VIR_STORAGE_XML_INACTIVE));
            if ( Returns==NULL ) sendGlobalErrors();
            else read = true;
        };
        virStoragePoolFree(storagePool[i]);
        i++;
    };
    free(storagePool);
    QTemporaryFile f;
    f.setAutoRemove(false);
    read = f.open();
    if (read) f.write(Returns);
    result.append(f.fileName());
    f.close();
    free(Returns);
    result.append(QString("'%1' StoragePool %2 XML'ed").arg(name).arg((read)?"":"don't"));
    return result;
}

void StoragePoolControlThread::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnect);
    if ( virtErrors!=NULL ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void StoragePoolControlThread::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message) );
    virResetLastError();
}
