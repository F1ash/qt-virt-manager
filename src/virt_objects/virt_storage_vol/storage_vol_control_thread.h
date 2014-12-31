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
    void                  resultData(Actions, QStringList);

private:
    QStringList           args;
    QString               currPoolName;
    bool                  keep_alive;
    virConnect           *currWorkConnect = NULL;
    virStoragePool       *currStoragePool = NULL;
    virErrorPtr           virtErrors;

public slots:
    bool                  setCurrentStoragePoolName(virConnect*, QString&);
    void                  stop();
    void                  execAction(Actions, QStringList);

private slots:
    void                  run();
    QStringList           getAllStorageVolList();
    QStringList           createStorageVol();
    QStringList           downloadStorageVol();
    QStringList           deleteStorageVol();
    QStringList           uploadStorageVol();
    QStringList           resizeStorageVol();
    QStringList           wipeStorageVol();
    QStringList           getStorageVolXMLDesc();

};

#endif // STORAGE_VOL_CONTROL_THREAD_H
