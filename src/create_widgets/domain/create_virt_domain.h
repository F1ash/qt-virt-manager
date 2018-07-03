#ifndef CREATE_VIRT_DOMAIN_H
#define CREATE_VIRT_DOMAIN_H

#include <QMainWindow>
#include <QMap>
#include <QSettings>
#include <QDir>
#include <QTemporaryFile>
#include <QTabWidget>
#include <QPushButton>
#include <QCloseEvent>
#include "virt_objects/virt_entity_config.h"
#include "create_widgets/common/_qwidget_threaded.h"
//#include <QDebug>

class HelperThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit HelperThread(
            QObject        *parent      = nullptr,
            virConnectPtr  *connPtrPtr  = nullptr);
    uint             cores = 0;
    QString          capabilities;
    void             run();
};

typedef QMap<QString, _QWidget*> WidgetList;

class CreateVirtDomain : public QMainWindow
{
    Q_OBJECT
public:
    explicit CreateVirtDomain(
            QWidget *parent = nullptr,
            TASK     _task  = TASK());
    ~CreateVirtDomain();

signals:
    void             errorMsg(const QString&);
    void             finished(const QString&);
    void             addNewTask(TASK*);

private:
    QSettings        settings;
    virConnectPtr*   ptr_ConnPtr = nullptr;
    QString          xmlFileName, xmlDesc;
    virErrorPtr      virtErrors;
    QString          type;
    QTabWidget      *tabWidget = nullptr;
    //QCheckBox       *showDescription;
    QLabel          *about;
    QPushButton     *ok;
    QPushButton     *restore;
    QPushButton     *cancel;
    QWidget         *buttons;
    QHBoxLayout     *buttonLayout;
    QVBoxLayout     *commonLayout;

    QTemporaryFile  *xml = nullptr;
    WidgetList       wdgList;
    bool             ready = false;
    TASK             task;

    HelperThread    *hlpThread;
    QWidget         *baseWdg = nullptr;

private slots:
    void             closeEvent(QCloseEvent*);
    void             readCapabilities();
    void             readDataLists();
    bool             buildXMLDescription();
    void             set_Result();
    void             create_specified_widgets();
    void             set_specified_Tabs();
    void             restoreParameters();
    void             setBootOrder(QDomElement*);
    void             setNewWindowTitle(const QString&);
    void             sendMsg(const QString&);
};

#endif // CREATE_VIRT_DOMAIN_H
