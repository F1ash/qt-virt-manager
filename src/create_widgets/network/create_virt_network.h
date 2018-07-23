#ifndef CREATE_VIRT_NETWORK_H
#define CREATE_VIRT_NETWORK_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
#include <QTemporaryFile>
#include <QPushButton>
#include <QCloseEvent>
#include <QTime>
#include <virt_objects/virt_entity_config.h>
#include <virt_objects/_virt_thread.h>
#include "create_virt_network_adv.h"
#include "create_virt_network_ass.h"
//#include <QDebug>

class NetHelperThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit NetHelperThread(
            QObject        *parent      = Q_NULLPTR,
            virConnectPtr  *connPtrPtr  = Q_NULLPTR);
    void             run();
};

class CreateVirtNetwork : public QMainWindow
{
    Q_OBJECT
public:
    explicit CreateVirtNetwork(
            QWidget *parent = Q_NULLPTR,
            TASK     _task  = TASK());
    ~CreateVirtNetwork();

signals:
    void             errorMsg(const QString&);
    void             finished(const QString&);
    void             addNewTask(TASK*);

private:
    QSettings        settings;
    virConnectPtr*   ptr_ConnPtr = Q_NULLPTR;
    QString          xmlFileName, xmlDesc;
    virErrorPtr      virtErrors;

    QTemporaryFile  *xml = Q_NULLPTR;
    bool             newbe = false;
    TASK             task;

    NetHelperThread *hlpThread;
    CreateVirtNetwork_Adv
                    *advancedWdg = Q_NULLPTR;
    CreateVirtNetwork_Ass
                    *assistantWdg = Q_NULLPTR;

public slots:
    void             closeEvent(QCloseEvent*);
    void             readCapabilities();
    void             set_Result(bool);
    void             setNewWindowTitle(const QString&);
    void             sendMsg(const QString&);
};

#endif // CREATE_VIRT_NETWORK_H
