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
#include <QDebug>

class NetHelperThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit NetHelperThread(
            QObject        *parent      = nullptr,
            virConnectPtr  *connPtrPtr  = nullptr);
    void             run();
};

class CreateVirtNetwork : public QMainWindow
{
    Q_OBJECT
public:
    explicit CreateVirtNetwork(
            QWidget *parent = nullptr,
            TASK     _task  = TASK());
    ~CreateVirtNetwork();

signals:
    void             errorMsg(const QString&);
    void             finished(const QString&);
    void             addNewTask(TASK);

private:
    QSettings        settings;
    virConnectPtr*   ptr_ConnPtr = nullptr;
    QString          xmlFileName, xmlDesc;
    virErrorPtr      virtErrors;

    QTemporaryFile  *xml = nullptr;
    bool             newbe = false;
    TASK             task;

    NetHelperThread *hlpThread;
    CreateVirtNetwork_Adv
                    *advancedWdg = nullptr;
    CreateVirtNetwork_Ass
                    *assistantWdg = nullptr;

public slots:
    void             closeEvent(QCloseEvent*);
    void             readCapabilities();
    void             set_Result(bool);
    void             setNewWindowTitle(const QString&);
    void             sendMsg(QString&);
};

#endif // CREATE_VIRT_NETWORK_H
