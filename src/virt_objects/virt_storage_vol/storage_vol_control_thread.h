#ifndef STORAGE_VOL_CONTROL_THREAD_H
#define STORAGE_VOL_CONTROL_THREAD_H

#include "virt_objects/control_thread.h"

#define BLOCK_SIZE 1024

class StorageVolControlThread : public ControlThread
{
    Q_OBJECT
public:
    explicit StorageVolControlThread(QObject *parent = NULL);

signals:

private:
    QString               currPoolName;
    virStoragePool       *currStoragePool = NULL;

public slots:
    void                  stop();
    void                  execAction(uint, TASK);

private slots:
    void                  run();
    Result                getAllStorageVolList();
    Result                createStorageVol();
    Result                downloadStorageVol();
    Result                deleteStorageVol();
    Result                uploadStorageVol();
    Result                resizeStorageVol();
    Result                wipeStorageVol();
    Result                getStorageVolXMLDesc();
};

#endif // STORAGE_VOL_CONTROL_THREAD_H
