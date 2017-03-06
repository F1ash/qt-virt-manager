#ifndef STORAGE_VOL_CONTROL_THREAD_H
#define STORAGE_VOL_CONTROL_THREAD_H

#include "virt_objects/control_thread.h"

#define BLOCK_SIZE 1024

class StorageVolControlThread : public ControlThread
{
    Q_OBJECT
public:
    explicit StorageVolControlThread(QObject *parent = nullptr);

signals:

private:
    QString               currPoolName;
    virStoragePool       *currStoragePool = nullptr;

    const unsigned long long   bytes = 1;
    const unsigned long long   KiB   = 1024;
    const unsigned long long   MiB   = 1048576;
    const unsigned long long   GiB   = 1073741824;
    const unsigned long long   TiB   = 1099511627776;

public slots:
    void                  stop();
    void                  execAction(uint, TASK);

private slots:
    QString               intToRangedStr(unsigned long long);
    void                  run();
    Result                getAllStorageVolList();
    Result                createStorageVol();
    Result                downloadStorageVol();
    Result                deleteStorageVol();
    Result                uploadStorageVol();
    Result                resizeStorageVol();
    Result                wipeStorageVol();
    Result                refreshStorageVolList();
    Result                getStorageVolXMLDesc();
};

#endif // STORAGE_VOL_CONTROL_THREAD_H
