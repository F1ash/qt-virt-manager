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
    case DOWNLOAD_StVOL :
        result.append(downloadStorageVol());
        break;
    case UPLOAD_StVOL :
        result.append(uploadStorageVol());
        break;
    case DELETE_StVOL :
        result.append(deleteStorageVol());
        break;
    case RESIZE_StVOL :
        result.append(resizeStorageVol());
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
                use.append(QString("%1").arg(info.capacity));
            } else {
                sendConnErrors();
                type.append("-");
                use.append("-");
            };
            QStringList currentAttr;
            currentAttr<< QString( virStorageVolGetName(storageVol[i]) )
                       << QString( virStorageVolGetPath(storageVol[i]) )
                       << QString( type )
                       << QString( use );
            storageVolList.append(currentAttr.join(" "));
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
QStringList StorageVolControlThread::downloadStorageVol()
{
    QStringList result;
    QString name = args.first();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    /*
    virStorageVolPtr *storageVol;
    int ret = virStoragePoolListAllVolumes( currStoragePool, &storageVol, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(storageVol);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currStoragePool));
    */

    bool downloaded = false;
    virStreamPtr stream;
    unsigned long long offset = 0;
    unsigned long long length = 0;
    // flags: extra flags; not used yet, so callers should always pass 0
    flags = 0;
    /*
    int i = 0;
    while ( storageVol[i] != NULL ) {
        QString currVolName = QString( virStorageVolGetName(storageVol[i]) );
        if ( !downloaded && currVolName==name ) {
            int ret = virStorageVolDownload(storageVol[i], stream, offset, length, flags);
            if ( ret<0 ) {
                sendConnErrors();
                return result;
            } else downloaded = true;
        };
        virStorageVolFree(storageVol[i]);
        i++;
    };
    free(storageVol);
    */
    virStorageVol *storageVol = virStorageVolLookupByName(currStoragePool, name.toUtf8().data());
    if ( storageVol!=NULL ) {
        int ret = virStorageVolDownload(storageVol, stream, offset, length, flags);
        if ( ret<0 ) sendConnErrors();
        else downloaded = true;
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    result.append(QString("'%1' StorageVol %2 Downloaded.").arg(name).arg((downloaded)?"":"don't"));
    return result;
}
QStringList StorageVolControlThread::uploadStorageVol()
{
    QStringList result;
    QString name = args.first();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    /*
    virStorageVolPtr *storageVol;
    int ret = virStoragePoolListAllVolumes( currStoragePool, &storageVol, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(storageVol);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currStoragePool));
    */

    virStreamPtr stream;
    unsigned long long offset = 0;
    unsigned long long length = 0;
    // flags: extra flags; not used yet, so callers should always pass 0
    flags = 0;
    bool uploaded = false;
    /*
    int i = 0;
    while ( storageVol[i] != NULL ) {
        QString currVolName = QString( virStorageVolGetName(storageVol[i]) );
        if ( !uploaded && currVolName==name ) {
            int ret = virStorageVolUpload(storageVol[i], stream, offset, length, flags);
            if ( ret<0 ) {
                sendConnErrors();
                return result;
            } else uploaded = true;
        };
        virStorageVolFree(storageVol[i]);
        i++;
    };
    free(storageVol);
    */
    virStorageVol *storageVol = virStorageVolLookupByName(currStoragePool, name.toUtf8().data());
    if ( storageVol!=NULL ) {
        int ret = virStorageVolUpload(storageVol, stream, offset, length, flags);
        if ( ret<0 ) {
            sendConnErrors();
        } else uploaded = true;
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    result.append(QString("'%1' StorageVol %2 Uploaded.").arg(name).arg((uploaded)?"":"don't"));
    return result;
}
QStringList StorageVolControlThread::deleteStorageVol()
{
    QStringList result;
    QString name = args.first();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    /*
    virStorageVolPtr *storageVol;
    int ret = virStoragePoolListAllVolumes( currStoragePool, &storageVol, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(storageVol);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currStoragePool));
    */

    // flags: extra flags; not used yet, so callers should always pass 0
    flags = 0;
    bool deleted = false;
    /*
    int i = 0;
    while ( storageVol[i] != NULL ) {
        QString currVolName = QString( virStorageVolGetName(storageVol[i]) );
        if ( !deleted && currVolName==name ) {
            deleted = (virStorageVolDelete(storageVol[i], flags)+1) ? true : false;
            if (!deleted) sendGlobalErrors();
        };
        qDebug()<<QVariant(deleted).toString()<<currVolName<<name;
        virStorageVolFree(storageVol[i]);
        i++;
    };
    free(storageVol);
    */
    virStorageVol *storageVol = virStorageVolLookupByName(currStoragePool, name.toUtf8().data());
    if ( storageVol!=NULL ) {
        deleted = (virStorageVolDelete(storageVol, flags)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    result.append(QString("'%1' StorageVol %2 Deleted.").arg(name).arg((deleted)?"":"don't"));
    return result;
}
QStringList StorageVolControlThread::resizeStorageVol()
{
    QStringList result;
    QString name = args.first();
    /*
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    virStorageVolPtr *storageVol;
    int ret = virStoragePoolListAllVolumes( currStoragePool, &storageVol, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(storageVol);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currStoragePool));
    */

    unsigned long long capacity = 0;
    bool resized = false;
    /*
    int i = 0;
    while ( storageVol[i] != NULL ) {
        QString currVolName = QString( virStorageVolGetName(storageVol[i]) );
        if ( !resized && currVolName==name ) {
            int ret = virStorageVolResize(storageVol[i], capacity, VIR_STORAGE_VOL_RESIZE_ALLOCATE);
            if ( ret<0 ) {
                sendConnErrors();
                return result;
            } else resized = true;
        };
        qDebug()<<QVariant(resized).toString()<<currVolName<<name;
        virStorageVolFree(storageVol[i]);
        i++;
    };
    free(storageVol);
    */
    virStorageVol *storageVol = virStorageVolLookupByName(currStoragePool, name.toUtf8().data());
    if ( storageVol!=NULL ) {
        int ret = virStorageVolResize(storageVol, capacity, VIR_STORAGE_VOL_RESIZE_ALLOCATE);
        if ( ret<0 ) {
            sendConnErrors();
        } else resized = true;
        virStorageVolFree(storageVol);
    } else sendConnErrors();
    result.append(QString("'%1' StorageVol %2 Resized.").arg(name).arg((resized)?"":"don't"));
    return result;
}
QStringList StorageVolControlThread::getStorageVolXMLDesc()
{
    QStringList result;
    QString name = args.first();
    // flags: extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;
    /*
    virStorageVolPtr *storageVol;
    int ret = virStoragePoolListAllVolumes( currStoragePool, &storageVol, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(storageVol);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currStoragePool));
    */

    bool read = false;
    char *Returns = NULL;
    // flags: extra flags; not used yet, so callers should always pass 0
    flags = 0;
    /*
    int i = 0;
    while ( storageVol[i] != NULL ) {
        QString currVolName = QString( virStorageVolGetName(storageVol[i]) );
        if ( !read && currVolName==name ) {
            Returns = (virStorageVolGetXMLDesc(storageVol[i], flags));
            if ( Returns==NULL ) sendGlobalErrors();
            else read = true;
        };
        virStorageVolFree(storageVol[i]);
        i++;
    };
    free(storageVol);
    */
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
