#ifndef TASK_WAREHOUSE_H
#define TASK_WAREHOUSE_H

#include <QMainWindow>
#include <QString>
#include <QSettings>
#include <QTime>
#include <QCloseEvent>
#include <QListWidget>
#include <QMap>
#include <QVBoxLayout>
#include "virt_objects/virt_domain/domain_control_thread.h"
#include "virt_objects/virt_network/net_control_thread.h"
#include "virt_objects/virt_storage_pool/storage_pool_control_thread.h"
#include "virt_objects/virt_storage_vol/storage_vol_control_thread.h"

typedef QMap<QString, ControlThread*> THREAD_POOL;

class TaskWareHouse : public QMainWindow
{
    Q_OBJECT
public:
    explicit TaskWareHouse(QWidget *parent = NULL);

signals:
    void             visibilityChanged(bool);
    void             taskMsg(QString&);
    void             domResult(Result);
    void             netResult(Result);
    void             poolResult(Result);
    void             volResult(Result);

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
    void             msgRepeater(QString);
    void             taskResultReceiver(Result);
    void             setNewTooltip(QListWidgetItem*);
};

#endif // TASK_WAREHOUSE_H
