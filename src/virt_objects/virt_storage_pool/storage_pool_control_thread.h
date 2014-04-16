#ifndef STORAGE_POOL_CONTROL_THREAD_H
#define STORAGE_POOL_CONTROL_THREAD_H

#include <QThread>
#include <QDir>
#include <QTemporaryFile>
#include <QStringList>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

enum StoragePoolActions {
    GET_ALL_StPOOL,
    CREATE_StPOOL,
    DEFINE_StPOOL,
    START_StPOOL,
    DESTROY_StPOOL,
    UNDEFINE_StPOOL,
    CHANGE_StPOOL_AUTOSTART,
    GET_StPOOL_XML_DESC,
    StPOOL_EMPTY_ACTION
};

class StoragePoolControlThread : public QThread
{
    Q_OBJECT
public:
    explicit StoragePoolControlThread(QObject *parent = 0);

signals:
    void errorMsg(QString);
    void resultData(StoragePoolActions, QStringList);

private:
    StoragePoolActions    action;
    QStringList           args;
    bool                  keep_alive;
    virConnect           *currWorkConnect = NULL;
    virErrorPtr           virtErrors;

public slots:
    bool setCurrentWorkConnect(virConnectPtr);
    void stop();
    void execAction(StoragePoolActions, QStringList);

private slots:
    void run();
    QStringList getAllStoragePoolList();
    QStringList createStoragePool();
    QStringList defineStoragePool();
    QStringList startStoragePool();
    QStringList destroyStoragePool();
    QStringList undefineStoragePool();
    QStringList changeAutoStartStoragePool();
    QStringList getStoragePoolXMLDesc();

    void sendConnErrors();
    void sendGlobalErrors();

};

#endif // STORAGE_POOL_CONTROL_THREAD_H
