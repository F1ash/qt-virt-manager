#ifndef STORAGE_POOL_CONTROL_THREAD_H
#define STORAGE_POOL_CONTROL_THREAD_H

#include "virt_objects/control_thread.h"

class StoragePoolControlThread : public ControlThread
{
    Q_OBJECT
public:
    explicit StoragePoolControlThread(QObject *parent = NULL);

signals:

private:

public slots:
    void                  stop();
    void                  execAction(uint, TASK);

private slots:
    void                  run();
    Result                getAllStoragePoolList();
    Result                createStoragePool();
    Result                defineStoragePool();
    Result                startStoragePool();
    Result                destroyStoragePool();
    Result                undefineStoragePool();
    Result                changeAutoStartStoragePool();
    Result                deleteStoragePool();
    Result                getStoragePoolXMLDesc();
};

#endif // STORAGE_POOL_CONTROL_THREAD_H
