#include "storage_vol_control_thread.h"

StorageVolControlThread::StorageVolControlThread(QObject *parent) :
    ControlThread(parent)
{
    currWorkConnection = NULL;
}

/* public slots */
bool StorageVolControlThread::setCurrentStoragePoolName(virConnect *conn, QString &poolName, QString &connName)
{
    keep_alive = true;
    currConnName = connName;
    // close previous virConnectRef[erence]
    if ( NULL!=currWorkConnection ) {
        int ret = virConnectClose(currWorkConnection);
        //qDebug()<<"virConnectRef -1"<<"StorageVolControlThread"<<currPoolName<<(ret+1>0);
    };
    currWorkConnection = conn;
    // for new virConnect usage create the new virConnectRef[erence]
    int ret = virConnectRef(currWorkConnection);
    if ( ret<0 ) {
        currWorkConnection = NULL;
        sendConnErrors();
        keep_alive = false;
    };
    //qDebug()<<"virConnectRef +1"<<"StorageVolControlThread"<<poolName<<(ret+1>0);
    currPoolName = poolName;
    if (currStoragePool!=NULL) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = NULL;
    };
    currStoragePool = virStoragePoolLookupByName(
                currWorkConnection, currPoolName.toUtf8().data());
    if ( NULL==currStoragePool ) sendConnErrors();
    //qDebug()<<"stVol_thread (setPoolData)\n\tConnect\t\t"<<currWorkConnection
    //        <<"\n\tPool\t\t"<<currStoragePool
    //        <<"\n\tName\t\t"<<currPoolName;
}
void StorageVolControlThread::stop()
{
    keep_alive = false;
    if (currStoragePool!=NULL) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = NULL;
    };
    //qDebug()<<"stVol_thread (stop)\n\tConnect\t\t"<<currWorkConnection
    //        <<"\n\tPool\t\t"<<currStoragePool
    //        <<"\n\tName\t\t"<<currPoolName;
}
void StorageVolControlThread::execAction(Actions act, QStringList _args)
{
    if ( keep_alive && !isRunning() ) {
        action = act;
        args = _args;
        if ( NULL!=currWorkConnection ) start();
        else {
            Result result;
            result.type   = "volume";
            result.number = number;
            result.action = _EMPTY_ACTION;
            emit resultData(result);
        };
        //qDebug()<<"stVolThread started\n\targs\t\t"<<_args<<"\n\taction\t\t"<<act;
        //qDebug()<<"stVol_thread (execAct)\n\tConnect\t\t"<<currWorkConnection
        //        <<"\n\tPool\t\t"<<currStoragePool
        //        <<"\n\tName\t\t"<<currPoolName;
    };
}

/* private slots */
void StorageVolControlThread::run()
{
    Result result;
    switch (action) {
    case GET_ALL_ENTITY :
        result = getAllStorageVolList();
        break;
    case CREATE_ENTITY:
        result = createStorageVol();
        break;
    case DELETE_ENTITY :
        result = deleteStorageVol();
        break;
    case DOWNLOAD_ENTITY :
        result = downloadStorageVol();
        break;
    case UPLOAD_ENTITY :
        result = uploadStorageVol();
        break;
    case RESIZE_ENTITY :
        result = resizeStorageVol();
        break;
    case WIPE_ENTITY :
        result = wipeStorageVol();
        break;
    case GET_XML_DESCRIPTION :
        result = getStorageVolXMLDesc();
        break;
    default:
        break;
    };
    result.type   = "volume";
    result.number = number;
    result.action = action;
    emit resultData(result);
}
Result StorageVolControlThread::getAllStorageVolList()
{
    Result result;
    result.name = QString("%1_%2").arg(currConnName).arg(currPoolName);
    QStringList storageVolList;
    if ( currStoragePool==NULL ) {
        if ( currWorkConnection!=NULL && keep_alive ) {
            currStoragePool = virStoragePoolLookupByName(currWorkConnection, currPoolName.toUtf8().data());
        };
    };
    if ( currStoragePool!=NULL && keep_alive ) {
        virStorageVolPtr *storageVol = NULL;
        // flags: extra flags; not used yet, so callers should always pass 0
        unsigned int flags = 0;
        int ret = virStoragePoolListAllVolumes( currStoragePool, &storageVol, flags );
        if ( ret<0 ) {
            sendConnErrors();
            result.result = false;
            result.msg = storageVolList;
            return result;
        };

        // therefore correctly to use for() command, because storageVol[0] can not exist.
        for (int i = 0; i < ret; i++) {
            QString type, capacity, allocation;
            virStorageVolInfo info;
            if ( virStorageVolGetInfo(storageVol[i], &info)+1 ) {
                switch (info.type) {
                case VIR_STORAGE_VOL_FILE:
                    type.append("file");
                    break;
                case VIR_STORAGE_VOL_BLOCK:
                    type.append("block");
                    break;
                case VIR_STORAGE_VOL_DIR:
                    type.append("dir");
                    break;
                case VIR_STORAGE_VOL_NETWORK:
                    type.append("net");
                    break;
                default:
                    type.append("-");
                    break;
                };
                allocation.append(QString("%1").arg(info.allocation));
                capacity.append(QString("%1").arg(info.capacity));
            } else {
                sendConnErrors();
                type.append("-");
                allocation.append("-");
                capacity.append("-");
            };
            QStringList currentAttr;
            currentAttr<< QString().fromUtf8( virStorageVolGetName(storageVol[i]) )
                       << QString().fromUtf8( virStorageVolGetPath(storageVol[i]) )
                       << QString( type )
                       << QString( allocation )
                       << QString( capacity );;
            storageVolList.append(currentAttr);
            //qDebug()<<currentAttr<<"Volume";
            virStorageVolFree(storageVol[i]);
        };
        free(storageVol);
    };
    result.result = true;
    result.msg = storageVolList;
    return result;
}
Result StorageVolControlThread::createStorageVol()
{
    Result result;
    result.name = QString("%1_%2").arg(currConnName).arg(currPoolName);
    QString path = args.first();
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
    virStorageVolPtr storageVol = virStorageVolCreateXML(
                currStoragePool, xmlData.data(), VIR_STORAGE_VOL_CREATE_PREALLOC_METADATA);
    if ( storageVol==NULL ) {
        sendConnErrors();
        result.result = false;
        return result;
    };
    QString name = QString().fromUtf8( virStorageVolGetName(storageVol) );
    result.msg
            .append(QString("'<b>%1</b>' StorageVol from\n\"%2\"\nis created.")
                    .arg(name).arg(path));
    virStorageVolFree(storageVol);
    result.result = true;
    return result;
}
Result StorageVolControlThread::deleteStorageVol()
{
    Result result;
    result.name = QString("%1_%2").arg(currConnName).arg(currPoolName);
    QString name = args.first();

    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    bool deleted = false;
    virStorageVol *storageVol = virStorageVolLookupByName(currStoragePool, name.toUtf8().data());
    if ( storageVol!=NULL ) {
        deleted = (virStorageVolDelete(storageVol, flags)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    result.msg.append(QString("'<b>%1</b>' StorageVol %2 Deleted.")
                      .arg(name).arg((deleted)?"":"don't"));
    result.result = deleted;
    return result;
}
Result StorageVolControlThread::downloadStorageVol()
{
    Result result;
    result.name = QString("%1_%2").arg(currConnName).arg(currPoolName);
    QString name, path;
    name = args.first();
    args.removeFirst();
    path = args.first();
    args.removeFirst();
    //qDebug()<<args.first()<<"download";
    QFile *f = new QFile(path);
    f->open(QIODevice::WriteOnly);

    bool downloaded = false;
    virStreamPtr stream = virStreamNew(currWorkConnection, 0);
    unsigned long long offset = 0;
    unsigned long long length = args.first().toULongLong();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStorageVol *storageVol = virStorageVolLookupByName(currStoragePool, name.toUtf8().data());
    if ( storageVol!=NULL ) {
        int ret = virStorageVolDownload(storageVol, stream, offset, length, flags);
        if ( ret<0 ) sendConnErrors();
        else {
            downloaded = true;
            length = 0;
            char buf[BLOCK_SIZE];
            int got, saved, step;
            step = 0;
            while ( 1 && keep_alive ) {
                got = virStreamRecv(stream, buf, BLOCK_SIZE);
                if (got < 0) {
                    sendConnErrors();
                    downloaded = false;
                    break;
                };
                if (got == 0) break;
                step++;
                saved = f->write(buf, got);
                //qDebug()<<"got<>saved"<<got<<saved<<step;
                if ( saved+1 ) length += saved;
                else {
                    QString msg = QString("WriteError after (%2): %1 bytes")
                            .arg(length).arg(step);
                    emit errorMsg( msg, number );
                };
            };
            virStreamFinish(stream);
        };
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    if ( stream!=NULL ) virStreamFree(stream);
    f->close();
    delete f; f = NULL;
    result.msg.append(QString("'<b>%1</b>' StorageVol %2 Downloaded into %3 (%4).")
                  .arg(name).arg((downloaded)?"":"don't")
                  .arg(path).arg(length));
    result.result = downloaded;
    return result;
}
Result StorageVolControlThread::resizeStorageVol()
{
    Result result;
    result.name = QString("%1_%2").arg(currConnName).arg(currPoolName);
    QString name = args.first();
    args.removeFirst();

    unsigned long long capacity = 0;
    if ( args.count() && !args.first().isEmpty() ) {
        capacity = args.first().toULongLong();
    };
    bool resized = false;
    virStorageVol *storageVol = virStorageVolLookupByName(
                currStoragePool, name.toUtf8().data());
    if ( storageVol!=NULL ) {
        int ret = virStorageVolResize(storageVol, capacity,
                                      VIR_STORAGE_VOL_RESIZE_ALLOCATE);
    //                                 | VIR_STORAGE_VOL_RESIZE_SHRINK);
    // TODO: add SHRINK-flag when fixed
    // See for: <a href='https://bugzilla.redhat.com/show_bug.cgi?id=1021802'>Red Hat Bugzilla #1021802</a>
        if ( ret<0 ) {
            sendConnErrors();
            QString msg("ResizeError: Maybe <a href='https://bugzilla.redhat.com/show_bug.cgi?id=1021802'>Red Hat Bugzilla #1021802</a>");
            emit errorMsg(msg, number);
        } else resized = true;
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    result.msg.append(QString("'<b>%1</b>' StorageVol %2 Resized to %3 (bytes).")
                  .arg(name).arg((resized)?"":"don't").arg(capacity));
    result.result = resized;
    return result;
}
Result StorageVolControlThread::uploadStorageVol()
{
    Result result;
    result.name = QString("%1_%2").arg(currConnName).arg(currPoolName);
    QString name, path;
    name = args.first();
    args.removeFirst();
    path = args.first();
    //qDebug()<<path<<"upload";
    QFile *f = new QFile(path);
    f->open(QIODevice::ReadOnly);

    bool uploaded = false;
    virStreamPtr stream = virStreamNew(currWorkConnection, 0);
    unsigned long long offset = 0;
    unsigned long long length = f->size();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStorageVol *storageVol = virStorageVolLookupByName(currStoragePool, name.toUtf8().data());
    if ( storageVol!=NULL ) {
        int ret = virStorageVolUpload(storageVol, stream, offset, length, flags);
        if ( ret<0 ) {
            sendConnErrors();
        } else {
            uploaded = true;
            length = 0;
            int got, saved, step;
            step = 0;
            char buf[BLOCK_SIZE];
            while ( 1 && keep_alive ) {
                got = f->read(buf, BLOCK_SIZE);
                if (got == 0) break;
                if ( got<0 ) {
                    QString msg = QString("ReadError after (%2): %1 bytes")
                            .arg(length).arg(step);
                    emit errorMsg( msg, number );
                } else {
                    saved = virStreamSend(stream, buf, got);
                    if (saved < 0) {
                        sendConnErrors();
                        uploaded = false;
                        break;
                    };
                    step++;
                    length += saved;
                    //qDebug()<<"got<>saved:length"<<got<<saved<<step<<length;
                };
            };
            virStreamFinish(stream);
        };
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    if ( stream!=NULL ) virStreamFree(stream);
    f->close();
    delete f; f = 0;
    result.msg.append(QString("'<b>%1</b>' StorageVol %2 Uploaded from %3 (%4).")
                  .arg(name).arg((uploaded)?"":"don't")
                  .arg(path).arg(length));
    result.result = uploaded;
    return result;
}
Result StorageVolControlThread::wipeStorageVol()
{
    Result result;
    result.name = QString("%1_%2").arg(currConnName).arg(currPoolName);
    QString name, algorithm;
    name = args.first();
    args.removeFirst();

    //flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    unsigned int alg = 0;
    bool wiped = false;
    if ( !args.isEmpty() ) alg = args.first().toUInt();
    virStorageVol *storageVol = virStorageVolLookupByName(currStoragePool, name.toUtf8().data());
    if ( storageVol!=NULL ) {
        int ret = virStorageVolWipePattern(storageVol, alg, flags);
        if ( ret<0 ) {
            sendConnErrors();
        } else wiped = true;
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    switch (alg) {
    case VIR_STORAGE_VOL_WIPE_ALG_ZERO:
        algorithm.append("ZERO");
        break;
    case VIR_STORAGE_VOL_WIPE_ALG_NNSA:
        algorithm.append("NNSA");
        break;
    case VIR_STORAGE_VOL_WIPE_ALG_DOD:
        algorithm.append("DOD");
        break;
    case VIR_STORAGE_VOL_WIPE_ALG_BSI:
        algorithm.append("BSI");
        break;
    case VIR_STORAGE_VOL_WIPE_ALG_GUTMANN:
        algorithm.append("GUTMANN");
        break;
    case VIR_STORAGE_VOL_WIPE_ALG_PFITZNER7:
        algorithm.append("PFITZNER7");
        break;
    case VIR_STORAGE_VOL_WIPE_ALG_PFITZNER33:
        algorithm.append("PFITZNER33");
        break;
    case VIR_STORAGE_VOL_WIPE_ALG_RANDOM:
        algorithm.append("RANDOM");
        break;
    case VIR_STORAGE_VOL_WIPE_ALG_SCHNEIER:
        algorithm.append("SCHNEIER");
        break;
    default:
        algorithm.append("NONE");
        break;
    };
    result.msg.append(QString("'<b>%1</b>' StorageVol %2 Wiped with %3 algorithm.")
                  .arg(name).arg((wiped)?"":"don't").arg(algorithm));
    result.result = wiped;
    return result;
}
Result StorageVolControlThread::getStorageVolXMLDesc()
{
    Result result;
    result.name = QString("%1_%2").arg(currConnName).arg(currPoolName);
    QString name = args.first();

    bool read = false;
    char *Returns = NULL;
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStorageVol *storageVol = virStorageVolLookupByName(currStoragePool, name.toUtf8().data());
    if ( storageVol!=NULL ) {
        Returns = virStorageVolGetXMLDesc(storageVol, flags);
        if ( Returns==NULL ) sendConnErrors();
        else read = true;
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    QTemporaryFile f;
    f.setAutoRemove(false);
    f.setFileTemplate(QString("%1%2XML_Desc-XXXXXX.xml")
                      .arg(QDir::tempPath()).arg(QDir::separator()));
    read = f.open();
    if (read) f.write(Returns);
    result.msg.append(f.fileName());
    f.close();
    if ( Returns!=NULL ) free(Returns);
    result.msg.append(QString("'<b>%1</b>' StorageVol %2 XML'ed")
                  .arg(name).arg((read)?"":"don't"));
    result.result = read;
    return result;
}
