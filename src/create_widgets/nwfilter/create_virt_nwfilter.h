#ifndef CREATE_VIRT_NWFILTER_H
#define CREATE_VIRT_NWFILTER_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
#include <QTemporaryFile>
#include <QPushButton>
#include <QCloseEvent>
#include <QTime>
#include <virt_objects/virt_entity_config.h>
#include <virt_objects/_virt_thread.h>
#include <QDebug>

class NetFilterHelperThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit NetFilterHelperThread(
            QObject        *parent      = nullptr,
            virConnectPtr  *connPtrPtr  = nullptr);
    void             run();
};

class CreateVirtNWFilter : public QMainWindow
{
    Q_OBJECT
public:
    explicit CreateVirtNWFilter(
            QWidget *parent = nullptr,
            TASK     _task  = TASK());
    ~CreateVirtNWFilter();

signals:
    void             errorMsg(QString&);
    void             finished(QString&);
    void             addNewTask(TASK);

private:
    QSettings        settings;
    virConnectPtr*   ptr_ConnPtr = nullptr;
    QString          xmlFileName, xmlDesc;
    virErrorPtr      virtErrors;

    QTemporaryFile  *xml = nullptr;
    TASK             task;

    NetFilterHelperThread
                    *hlpThread;

public slots:
    void             closeEvent(QCloseEvent*);
    void             readCapabilities();
    void             set_Result(bool);
    void             setNewWindowTitle(const QString&);
    void             sendMsg(QString&);
};

#endif // CREATE_VIRT_NWFILTER_H
