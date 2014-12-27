#ifndef TASK_WAREHOUSE_H
#define TASK_WAREHOUSE_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QListWidget>
#include <QMap>
#include <QVBoxLayout>
#include "virt_objects/virt_domain/domain_control_thread.h"
#include "virt_objects/virt_network/net_control_thread.h"
#include "virt_objects/virt_storage_pool/storage_pool_control_thread.h"
#include "virt_objects/virt_storage_vol/storage_vol_control_thread.h"

typedef QMap<QString, QThread*> THREAD_POOL;

class TaskWareHouse : public QMainWindow
{
    Q_OBJECT
public:
    explicit TaskWareHouse(QWidget *parent = NULL);

signals:
    void             visibilityChanged(bool);
    void             taskMsg(QString&);

private:
    uint             counter = 0;
    THREAD_POOL     *threadPool;
    QSettings        settings;
    QListWidget     *taskList;

public slots:
    void             changeVisibility();
    void             saveCurrentState();
    void             stopTaskComputing();
    void             addNewTask(virConnectPtr, QStringList&);

private slots:
    void             closeEvent(QCloseEvent*);
    void             taskStateReceiver(uint, bool);
    void             taskResultReceiver(uint, int, Result);
};

#endif // TASK_WAREHOUSE_H
