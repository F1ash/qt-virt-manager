#ifndef STORAGE_VOL_CONTROL_THREAD_H
#define STORAGE_VOL_CONTROL_THREAD_H

#include <QThread>
#include <QDir>
#include <QFile>
#include <QTemporaryFile>
#include <QStringList>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

#define BLOCK_SIZE 1024

enum StorageVolActions {
    GET_ALL_StVOL,
    CREATE_StVOL,
    DELETE_StVOL,
    DOWNLOAD_StVOL,
    RESIZE_StVOL,
    UPLOAD_StVOL,
    WIPE_StVOL,
    GET_StVOL_XML_DESC,
    StVOL_EMPTY_ACTION
};

class StorageVolControlThread : public QThread
{
    Q_OBJECT
public:
    explicit StorageVolControlThread(QObject *parent = NULL);

signals:
    void errorMsg(QString);
    void resultData(StorageVolActions, QStringList);

private:
    StorageVolActions     action;
    QStringList           args;
    QString               currPoolName;
    bool                  keep_alive;
    virConnect           *currWorkConnect = NULL;
    virStoragePool       *currStoragePool = NULL;
    virErrorPtr           virtErrors;

public slots:
    bool setCurrentStoragePoolName(virConnect*, QString&);
    void stop();
    void execAction(StorageVolActions, QStringList);

private slots:
    void run();
    QStringList getAllStorageVolList();
    QStringList createStorageVol();
    QStringList downloadStorageVol();
    QStringList deleteStorageVol();
    QStringList uploadStorageVol();
    QStringList resizeStorageVol();
    QStringList wipeStorageVol();
    QStringList getStorageVolXMLDesc();

    void sendConnErrors();
    void sendGlobalErrors();

};

#endif // STORAGE_VOL_CONTROL_THREAD_H
