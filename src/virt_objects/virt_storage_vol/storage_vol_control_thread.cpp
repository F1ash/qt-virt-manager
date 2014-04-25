#include "storage_vol_control_thread.h"

StorageVolControlThread::StorageVolControlThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<StorageVolActions>("StorageVolActions");
}

/* public slots */
bool StorageVolControlThread::setCurrentStoragePoolName(virConnect *conn, QString &poolName)
{
    keep_alive = true;
    currWorkConnect = conn;
    currPoolName = poolName;
    if (currStoragePool!=NULL) {
        virStoragePoolFree(currStoragePool);
        currStoragePool = NULL;
    };
    //qDebug()<<"stVol_thread (setPoolData)\n\tConnect\t\t"<<currWorkConnect
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
    //qDebug()<<"stVol_thread (stop)\n\tConnect\t\t"<<currWorkConnect
    //        <<"\n\tPool\t\t"<<currStoragePool
    //        <<"\n\tName\t\t"<<currPoolName;
}
void StorageVolControlThread::execAction(StorageVolActions act, QStringList _args)
{
    if ( keep_alive && !isRunning() ) {
        action = act;
        args = _args;
        start();
        //qDebug()<<"stVolThread started\n\targs\t\t"<<_args<<"\n\taction\t\t"<<act;
        //qDebug()<<"stVol_thread (execAct)\n\tConnect\t\t"<<currWorkConnect
        //        <<"\n\tPool\t\t"<<currStoragePool
        //        <<"\n\tName\t\t"<<currPoolName;
    };
}

/* private slots */
void StorageVolControlThread::run()
{
    QStringList result;
    switch (action) {
    case GET_ALL_StVOL :
        result.append(getAllStorageVolList());
        break;
    case CREATE_StVOL :
        result.append(createStorageVol());
        break;
    case DELETE_StVOL :
        result.append(deleteStorageVol());
        break;
    case DOWNLOAD_StVOL :
        result.append(downloadStorageVol());
        break;
    case UPLOAD_StVOL :
        result.append(uploadStorageVol());
        break;
    case RESIZE_StVOL :
        result.append(resizeStorageVol());
        break;
    case WIPE_StVOL :
        result.append(wipeStorageVol());
        break;
    case GET_StVOL_XML_DESC :
        result.append(getStorageVolXMLDesc());
        break;
    default:
        break;
    };
    //qDebug()<<result<<"stVolThread res";
    emit resultData(action, result);
}
QStringList StorageVolControlThread::getAllStorageVolList()
{
    QStringList storageVolList;
    if ( currStoragePool==NULL ) {
        if ( currWorkConnect!=NULL && keep_alive ) {
            currStoragePool = virStoragePoolLookupByName(currWorkConnect, currPoolName.toUtf8().data());
        };
    };
    if ( currStoragePool!=NULL && keep_alive ) {
        virStorageVolPtr *storageVol;
        // flags: extra flags; not used yet, so callers should always pass 0
        unsigned int flags = 0;
        int ret = virStoragePoolListAllVolumes( currStoragePool, &storageVol, flags );
        if ( ret<0 ) {
            sendConnErrors();
            return storageVolList;
        };

        int i = 0;
        while ( storageVol[i] != NULL ) {
            QString type, use;
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
                use.append(QString("%1").arg(info.allocation));
            } else {
                sendConnErrors();
                type.append("-");
                use.append("-");
            };
            QStringList currentAttr;
            currentAttr<< QString().fromUtf8( virStorageVolGetName(storageVol[i]) )
                       << QString().fromUtf8( virStorageVolGetPath(storageVol[i]) )
                       << QString( type )
                       << QString( use );
            storageVolList.append(currentAttr);
            //qDebug()<<currentAttr<<"Volume";
            virStorageVolFree(storageVol[i]);
            i++;
        };
        free(storageVol);
    };
    return storageVolList;
}
QStringList StorageVolControlThread::createStorageVol()
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
    virStorageVolPtr storageVol = virStorageVolCreateXML(currStoragePool, xmlData.data(), VIR_STORAGE_VOL_CREATE_PREALLOC_METADATA);
    if ( storageVol==NULL ) {
        sendConnErrors();
        return result;
    };
    result.append(QString("'%1' StorageVol from\n\"%2\"\nis created.").arg(virStorageVolGetName(storageVol)).arg(path));
    virStorageVolFree(storageVol);
    return result;
}
QStringList StorageVolControlThread::deleteStorageVol()
{
    QStringList result;
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
    result.append(QString("'%1' StorageVol %2 Deleted.").arg(name).arg((deleted)?"":"don't"));
    return result;
}
QStringList StorageVolControlThread::downloadStorageVol()
{
    QStringList result;
    QString name, path;
    name = args.first();
    args.removeFirst();
    path = args.first();
    args.removeFirst();
    //qDebug()<<args.first()<<"download";
    QFile *f = new QFile(path);
    f->open(QIODevice::WriteOnly);

    bool downloaded = false;
    virStreamPtr stream = virStreamNew(currWorkConnect, 0);
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
                else emit errorMsg( QString("WriteError after (%2): %1 bytes").arg(length).arg(step) );
            };
            virStreamFinish(stream);
        };
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    if ( stream!=NULL ) virStreamFree(stream);
    f->close();
    delete f; f = 0;
    result.append(QString("'%1' StorageVol %2 Downloaded into %3 (%4).")
                  .arg(name).arg((downloaded)?"":"don't")
                  .arg(path).arg(length));
    return result;
}
QStringList StorageVolControlThread::resizeStorageVol()
{
    QStringList result;
    QString name = args.first();
    args.removeFirst();

    unsigned long long capacity = 0;
    if ( args.count() && !args.first().isEmpty() ) {
        capacity = args.first().toULongLong();
    };
    bool resized = false;
    virStorageVol *storageVol = virStorageVolLookupByName(currStoragePool, name.toUtf8().data());
    if ( storageVol!=NULL ) {
        int ret = virStorageVolResize(storageVol, capacity,
                                      VIR_STORAGE_VOL_RESIZE_ALLOCATE |
                                      VIR_STORAGE_VOL_RESIZE_SHRINK);
        if ( ret<0 ) {
            sendConnErrors();
        } else resized = true;
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    result.append(QString("'%1' StorageVol %2 Resized to %3.").arg(name).arg((resized)?"":"don't").arg(capacity));
    return result;
}
QStringList StorageVolControlThread::uploadStorageVol()
{
    QStringList result;
    QString name, path;
    name = args.first();
    args.removeFirst();
    path = args.first();
    //qDebug()<<path<<"upload";
    QFile *f = new QFile(path);
    f->open(QIODevice::ReadOnly);

    bool uploaded = false;
    virStreamPtr stream = virStreamNew(currWorkConnect, 0);
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
                    emit errorMsg( QString("ReadError after (%2): %1 bytes")
                                   .arg(length).arg(step) );
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
    result.append(QString("'%1' StorageVol %2 Uploaded from %3 (%4).")
                  .arg(name).arg((uploaded)?"":"don't")
                  .arg(path).arg(length));
    return result;
}
QStringList StorageVolControlThread::wipeStorageVol()
{
    QStringList result;
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
    result.append(QString("'%1' StorageVol %2 Wiped with %3 algorithm.").arg(name).arg((wiped)?"":"don't").arg(algorithm));
    return result;
}
QStringList StorageVolControlThread::getStorageVolXMLDesc()
{
    QStringList result;
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
    f.setFileTemplate(QString("%1%2XML_Desc-XXXXXX.xml").arg(QDir::tempPath()).arg(QDir::separator()));
    read = f.open();
    if (read) f.write(Returns);
    result.append(f.fileName());
    f.close();
    free(Returns);
    result.append(QString("'%1' StorageVol %2 XML'ed").arg(name).arg((read)?"":"don't"));
    return result;
}

void StorageVolControlThread::sendConnErrors()
{
    virtErrors = virConnGetLastError( currWorkConnect );
    if ( virtErrors!=NULL ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void StorageVolControlThread::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message) );
    virResetLastError();
}
