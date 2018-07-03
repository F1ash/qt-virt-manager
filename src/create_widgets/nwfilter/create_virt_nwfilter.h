#ifndef CREATE_VIRT_NWFILTER_H
#define CREATE_VIRT_NWFILTER_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTableWidget>
#include <QSettings>
#include <QDir>
#include <QTemporaryFile>
#include <QCloseEvent>
#include <QTime>
#include <virt_objects/virt_entity_config.h>
#include <virt_objects/_virt_thread.h>
#include "filterref_widget/filterref_widget.h"
#include "chain_widget/chain_rules.h"
//#include <QDebug>

class NetFilterHelperThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit NetFilterHelperThread(
            QObject        *parent      = nullptr,
            virConnectPtr  *connPtrPtr  = nullptr);
    QStringList      filterList;
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
    void             errorMsg(const QString&);
    void             finished(const QString&);
    void             addNewTask(TASK*);

private:
    QSettings        settings;
    virConnectPtr*   ptr_ConnPtr = nullptr;
    QString          xmlFileName, xmlDesc;
    virErrorPtr      virtErrors;

    QTemporaryFile  *xml = nullptr;
    TASK             task;

    NetFilterHelperThread
                    *hlpThread;

    QLineEdit       *name, *uuid;
    QTabWidget      *tabs;
    QLabel          *about;
    QPushButton     *ok;
    QPushButton     *cancel;
    QHBoxLayout     *buttonLayout;
    QWidget         *buttons;
    FilterrefWidget *filterRefs;
    ChainRules      *chainRules;
    QVBoxLayout     *commonLayout;
    QWidget         *commonWdg;

    void             readXmlDescData(const QString&);
    void             buildXMLDescription();
    void             sendMsg(const QString&);

private slots:
    void             closeEvent(QCloseEvent*);
    void             readCapabilities();
    void             set_Result();
    void             setNewWindowTitle(const QString&);
};

#endif // CREATE_VIRT_NWFILTER_H
