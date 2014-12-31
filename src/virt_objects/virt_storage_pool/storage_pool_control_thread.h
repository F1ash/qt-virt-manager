#ifndef STORAGE_POOL_CONTROL_THREAD_H
#define STORAGE_POOL_CONTROL_THREAD_H

#include "virt_objects/control_thread.h"

class StoragePoolControlThread : public ControlThread
{
    Q_OBJECT
public:
    explicit StoragePoolControlThread(QObject *parent = NULL);

signals:
    void                  resultData(Actions, QStringList);

private:
    QStringList           args;
    bool                  keep_alive;
    virConnect           *currWorkConnect = NULL;
    virErrorPtr           virtErrors;

public slots:
    void                  stop();
    void                  execAction(Actions, QStringList);

private slots:
    void                  run();
    QStringList           getAllStoragePoolList();
    QStringList           createStoragePool();
    QStringList           defineStoragePool();
    QStringList           startStoragePool();
    QStringList           destroyStoragePool();
    QStringList           undefineStoragePool();
    QStringList           changeAutoStartStoragePool();
    QStringList           deleteStoragePool();
    QStringList           getStoragePoolXMLDesc();
};

#endif // STORAGE_POOL_CONTROL_THREAD_H
