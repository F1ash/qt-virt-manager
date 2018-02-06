#include "storage_vol_control_thread.h"

StorageVolControlThread::StorageVolControlThread(QObject *parent) :
    ControlThread(parent)
{
}

/* public slots */
void StorageVolControlThread::stop()
{
    keep_alive = false;
    if (currStoragePool!=nullptr) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = nullptr;
    };
    //qDebug()<<"stVol_thread (stop)\n\tConnect\t\t"<<ptr_ConnPtr
    //        <<"\n\tPool\t\t"<<currStoragePool
    //        <<"\n\tName\t\t"<<currPoolName;
}
void StorageVolControlThread::execAction(uint _num, TASK _task)
{
    number = _num;
    task = _task;
    keep_alive = false;
    currPoolName = task.args.object;
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
        result.type   = VIRT_ENTITY::VIRT_STORAGE_VOLUME;
        result.number = number;
        result.action = Actions::_NONE_ACTION;
        emit resultData(result);
    };
}

/* private slots */
QString StorageVolControlThread::intToRangedStr(unsigned long long i)
{
    QString res;
    if        ( i >= TiB ) {
        res = QString("%1 TiB").arg(QString::number((float)i/TiB, 'f', 2));
    } else if ( i >= GiB ) {
        res = QString("%1 GiB").arg(QString::number((float)i/GiB, 'f', 2));
    } else if ( i >= MiB ) {
        res = QString("%1 MiB").arg(QString::number((float)i/MiB, 'f', 2));
    } else if ( i >= KiB ) {
        res = QString("%1 KiB").arg(QString::number((float)i/KiB, 'f', 2));
    } else {
        res = QString("%1 Bytes").arg(QString::number(i));
    };
    return res;
}
void StorageVolControlThread::run()
{
    Result result;
    switch (task.action) {
    case Actions::GET_ALL_ENTITY_STATE :
        result = getAllStorageVolList();
        break;
    case Actions::CREATE_ENTITY:
        result = createStorageVol();
        break;
    case Actions::DELETE_ENTITY :
        result = deleteStorageVol();
        break;
    case Actions::DOWNLOAD_ENTITY :
        result = downloadStorageVol();
        break;
    case Actions::UPLOAD_ENTITY :
        result = uploadStorageVol();
        break;
    case Actions::RESIZE_ENTITY :
        result = resizeStorageVol();
        break;
    case Actions::WIPE_ENTITY :
        result = wipeStorageVol();
        break;
    case Actions::REFRESH_ENTITY :
        result = refreshStorageVolList();
        break;
    case Actions::GET_XML_DESCRIPTION :
        result = getStorageVolXMLDesc();
        break;
    default:
        break;
    };
    // task.srcConnPtr reference will closed in destructor as ptr_ConnPtr
    //virConnectClose(*task.srcConnPtr);
    result.type   = VIRT_ENTITY::VIRT_STORAGE_VOLUME;
    result.number = number;
    result.action = task.action;
    emit resultData(result);
}
Result StorageVolControlThread::getAllStorageVolList()
{
    Result result;
    result.name = QString("%1_%2").arg(task.srcConName).arg(currPoolName);
    ACT_RESULT storageVolList;
    if (currStoragePool!=nullptr) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = nullptr;
    };
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    currStoragePool = virStoragePoolLookupByName(
                *task.srcConnPtr, currPoolName.toUtf8().data());
    if ( currStoragePool!=nullptr && keep_alive ) {
        virStorageVolPtr *storageVol = nullptr;
        // flags: extra flags; not used yet, so callers should always pass 0
        unsigned int flags = 0;
        int ret = virStoragePoolListAllVolumes(
                    currStoragePool, &storageVol, flags);
        if ( ret<0 ) {
            result.err = sendConnErrors();
            result.result = false;
            return result;
        };

        // therefore correctly to use for() command,
        // because storageVol[0] can not exist.
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
                case VIR_STORAGE_VOL_NETDIR:
                    type.append("netdir");
                    break;
#if LIBVIR_VERSION_NUMBER > 1003003
                case VIR_STORAGE_VOL_PLOOP:
                    type.append("ploop");
                    break;
#endif
                default:
                    type.append("-");
                    break;
                };
                allocation.append(intToRangedStr(info.allocation));
                capacity.append(intToRangedStr(info.capacity));
            } else {
                sendConnErrors();
                type.append("-");
                allocation.append("-");
                capacity.append("-");
            };
            QVariantMap currentAttr;
            currentAttr.insert(
                        "name",
                        QString::fromUtf8( virStorageVolGetName(storageVol[i]) ));
            currentAttr.insert(
                        "path",
                        QString::fromUtf8( virStorageVolGetPath(storageVol[i]) ));
            currentAttr.insert(
                        "type", type);
            currentAttr.insert(
                        "allocation", allocation );
            currentAttr.insert(
                        "capacity", capacity );
            storageVolList.append(currentAttr);
            //qDebug()<<currentAttr<<"Volume";
            virStorageVolFree(storageVol[i]);
        };
        if (storageVol) free(storageVol);
    } else
        result.err = sendConnErrors();
    result.result = true;
    result.data   = storageVolList;
    return result;
}
Result StorageVolControlThread::createStorageVol()
{
    Result result;
    result.name = QString("%1_%2").arg(task.srcConName).arg(currPoolName);
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
    if (currStoragePool!=nullptr) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = nullptr;
    };
    currStoragePool = virStoragePoolLookupByName(
                *task.srcConnPtr, currPoolName.toUtf8().data());
    virStorageVolPtr storageVol = virStorageVolCreateXML(
                currStoragePool, xmlData.data(), VIR_STORAGE_VOL_CREATE_PREALLOC_METADATA);
    if ( storageVol==nullptr ) {
        result.err = sendConnErrors();
        result.result = false;
        return result;
    };
    QString name = QString::fromUtf8( virStorageVolGetName(storageVol) );
    result.msg.append(
                QString(tr("'<b>%1</b>' StorageVol from\n\"%2\"\nis created."))
                .arg(name).arg(path));
    virStorageVolFree(storageVol);
    result.result = true;
    return result;
}
Result StorageVolControlThread::deleteStorageVol()
{
    Result result;
    result.name = QString("%1_%2").arg(task.srcConName).arg(currPoolName);
    QString name = task.object;
    if (currStoragePool!=nullptr) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = nullptr;
    };
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    currStoragePool = virStoragePoolLookupByName(
                *task.srcConnPtr, currPoolName.toUtf8().data());

    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    bool deleted = false;
    virStorageVol *storageVol = virStorageVolLookupByName(
                currStoragePool, name.toUtf8().data());
    if ( storageVol!=nullptr ) {
        deleted = (virStorageVolDelete(storageVol, flags)+1) ? true : false;
        if (!deleted)
            result.err = sendConnErrors();
        virStorageVolFree(storageVol);
    } else
        result.err = sendConnErrors();
    result.msg.append(QString(tr("'<b>%1</b>' StorageVol %2 Deleted."))
                      .arg(name).arg((deleted)? "": tr("not")));
    result.result = deleted;
    return result;
}
Result StorageVolControlThread::downloadStorageVol()
{
    Result result;
    result.name = QString("%1_%2").arg(task.srcConName).arg(currPoolName);
    QString name, path;
    name = task.object;
    path = task.args.path;
    //qDebug()<<args.first()<<"download";
    if (currStoragePool!=nullptr) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = nullptr;
    };
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    currStoragePool = virStoragePoolLookupByName(
                *task.srcConnPtr, currPoolName.toUtf8().data());
    QFile *f = new QFile(path);
    f->open(QIODevice::WriteOnly);

    bool downloaded = false;
    virStreamPtr stream = virStreamNew(*task.srcConnPtr, 0);
    unsigned long long offset = 0;
    // unsigned long long length = task.args.size;
    unsigned long long length = 0;
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStorageVol *storageVol = virStorageVolLookupByName(
                currStoragePool, name.toUtf8().data());
    if ( storageVol!=nullptr ) {
        int ret = virStorageVolDownload(
                    storageVol, stream, offset, length, flags);
        if ( ret<0 )
            result.err = sendConnErrors();
        else {
            downloaded = true;
            length = 0;
            char buf[BLOCK_SIZE];
            int got, saved, step;
            step = 0;
            while ( 1 && keep_alive ) {
                got = virStreamRecv(stream, buf, BLOCK_SIZE);
                if (got < 0) {
                    result.err = sendConnErrors();
                    downloaded = false;
                    break;
                };
                if (got == 0) break;
                step++;
                saved = f->write(buf, got);
                //qDebug()<<"got<>saved"<<got<<saved<<step;
                if ( saved+1 ) length += saved;
                else {
                    QString msg = QString(
                            tr("WriteError after (%2): %1 bytes"))
                            .arg(length).arg(step);
                    emit errorMsg( msg, number );
                    result.err = msg;
                };
            };
            virStreamFinish(stream);
        };
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    if ( stream!=nullptr ) virStreamFree(stream);
    f->close();
    f->deleteLater();
    result.msg.append(
                QString(
                tr("'<b>%1</b>' StorageVol %2 Downloaded into %3 (%4)."))
                .arg(name).arg((downloaded)?"":"not")
                .arg(path).arg(length));
    result.result = downloaded;
    return result;
}
Result StorageVolControlThread::resizeStorageVol()
{
    Result result;
    result.name = QString("%1_%2").arg(task.srcConName).arg(currPoolName);
    QString name = task.object;
    if (currStoragePool!=nullptr) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = nullptr;
    };
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    currStoragePool = virStoragePoolLookupByName(
                *task.srcConnPtr, currPoolName.toUtf8().data());

    unsigned long long capacity = task.args.size;
    bool resized = false;
    virStorageVol *storageVol = virStorageVolLookupByName(
                currStoragePool, name.toUtf8().data());
    if ( storageVol!=nullptr ) {
        int ret = virStorageVolResize(
                    storageVol, capacity,
                    VIR_STORAGE_VOL_RESIZE_ALLOCATE |
                    VIR_STORAGE_VOL_RESIZE_SHRINK);
        if ( ret<0 ) {
            result.err = sendConnErrors();
        } else
            resized = true;
        virStorageVolFree(storageVol);
    } else
        result.err = sendConnErrors();
    result.msg.append(
                QString(tr("'<b>%1</b>' StorageVol %2 Resized to %3 (bytes)."))
                .arg(name).arg((resized)? "": tr("not")).arg(capacity));
    result.result = resized;
    return result;
}
Result StorageVolControlThread::uploadStorageVol()
{
    Result result;
    result.name = QString("%1_%2").arg(task.srcConName).arg(currPoolName);
    QString name, path;
    name = task.object;
    path = task.args.path;
    //qDebug()<<path<<"upload";
    if (currStoragePool!=nullptr) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = nullptr;
    };
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    currStoragePool = virStoragePoolLookupByName(
                *task.srcConnPtr, currPoolName.toUtf8().data());
    QFile *f = new QFile(path);
    f->open(QIODevice::ReadOnly);

    bool uploaded = false;
    virStreamPtr stream = virStreamNew(*task.srcConnPtr, 0);
    unsigned long long offset = 0;
    unsigned long long length = f->size();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStorageVol *storageVol = virStorageVolLookupByName(
                currStoragePool, name.toUtf8().data());
    if ( storageVol!=nullptr ) {
        int ret = virStorageVolUpload(
                    storageVol, stream, offset, length, flags);
        if ( ret<0 ) {
            result.err = sendConnErrors();
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
                    QString msg = QString(
                            tr("ReadError after (%2): %1 bytes"))
                            .arg(length).arg(step);
                    emit errorMsg( msg, number );
                    result.err = msg;
                } else {
                    saved = virStreamSend(stream, buf, got);
                    if (saved < 0) {
                        result.err = sendConnErrors();
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
    } else
        result.err = sendConnErrors();
    if ( stream!=nullptr ) virStreamFree(stream);
    f->close();
    f->deleteLater();
    result.msg.append(
                QString(tr("'<b>%1</b>' StorageVol %2 Uploaded from %3 (%4)."))
                .arg(name).arg((uploaded)?"":"not")
                .arg(path).arg(length));
    result.result = uploaded;
    return result;
}
Result StorageVolControlThread::wipeStorageVol()
{
    Result result;
    result.name = QString("%1_%2").arg(task.srcConName).arg(currPoolName);
    QString name, algorithm;
    name = task.object;
    if (currStoragePool!=nullptr) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = nullptr;
    };
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    currStoragePool = virStoragePoolLookupByName(
                *task.srcConnPtr, currPoolName.toUtf8().data());

    //flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    unsigned int alg = task.args.sign;
    bool wiped = false;
    virStorageVol *storageVol = virStorageVolLookupByName(
                currStoragePool, name.toUtf8().data());
    if ( storageVol!=nullptr ) {
        int ret = virStorageVolWipePattern(storageVol, alg, flags);
        if ( ret<0 ) {
            result.err = sendConnErrors();
        } else wiped = true;
        virStorageVolFree(storageVol);
    } else
        result.err = sendConnErrors();
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
    result.msg.append(
                QString(tr("'<b>%1</b>' StorageVol %2 Wiped with %3 algorithm."))
                .arg(name).arg((wiped)? "": tr("not")).arg(algorithm));
    result.result = wiped;
    return result;
}
Result StorageVolControlThread::refreshStorageVolList()
{
    Result result;
    result.name = QString("%1_%2").arg(task.srcConName).arg(currPoolName);
    if (currStoragePool!=nullptr) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = nullptr;
    };
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    currStoragePool = virStoragePoolLookupByName(
                *task.srcConnPtr, currPoolName.toUtf8().data());

    bool done = false;
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    if ( virStoragePoolRefresh(currStoragePool, flags)+1 ) {
        done = true;
    } else {
        result.err = sendConnErrors();
    };
    result.msg.append(
                QString(tr("StoragePool %1 refreshed."))
                .arg((done)? "": tr("not")));
    result.result = done;
    return result;
}
Result StorageVolControlThread::getStorageVolXMLDesc()
{
    Result result;
    result.name = QString("%1_%2").arg(task.srcConName).arg(currPoolName);
    QString name = task.object;
    if (currStoragePool!=nullptr) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = nullptr;
    };
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    currStoragePool = virStoragePoolLookupByName(
                *task.srcConnPtr, currPoolName.toUtf8().data());

    bool read = false;
    char *Returns = nullptr;
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStorageVol *storageVol = virStorageVolLookupByName(
                currStoragePool, name.toUtf8().data());
    if ( storageVol!=nullptr ) {
        Returns = virStorageVolGetXMLDesc(storageVol, flags);
        if ( Returns==nullptr )
            result.err = sendConnErrors();
        else read = true;
        virStorageVolFree(storageVol);
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
    result.msg.append(QString(tr("'<b>%1</b>' StorageVol %2 XML'ed."))
                  .arg(name).arg((read)? "": tr("not")));
    result.result = read;
    return result;
}
