#ifndef VM_VIEWER_H
#define VM_VIEWER_H

#include <QMainWindow>
#include <QProgressBar>
#include <QStatusBar>
#include <QCloseEvent>
#include <QMessageBox>
#include <QTime>
#include <QFileDialog>
#include <QSettings>
#include "vm_viewer/viewer_toolbar.h"
#include "virt_objects/virt_entity_config.h"
#include "create_widgets/snapshot/create_snapshot_dialog.h"
#include "create_widgets/snapshot/snapshot_action_dialog.h"
#include <QDebug>

#define TIMEOUT     60*1000
#define PERIOD      333

class VM_Viewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit VM_Viewer(
            QWidget *parent = NULL,
            virConnect *conn = NULL,
            QString arg1 = QString(),
            QString arg2 = QString());
    ~VM_Viewer();
    QString          connName, domain, TYPE;
    virConnect      *jobConnect = NULL;
    virErrorPtr      virtErrors = NULL;
    bool             VM_State;
    QSettings        settings;
    ViewerToolBar   *viewerToolBar = NULL;
    QProgressBar    *closeProcess = NULL;
    uint             timerId = 0;
    uint             killTimerId = 0;
    uint             counter = 0;
    virDomain       *domainPtr = NULL;

signals:
    void             finished(QString&);
    void             errorMsg(QString&);
    void             addNewTask(TASK);

public slots:
    virtual bool     isActive() const;
    virDomain*       getDomainPtr() const;
    virtual void     closeEvent(QCloseEvent *ev);
    void             sendErrMsg(QString&, uint);
    void             sendConnErrors();
    void             sendGlobalErrors();
    void             resendExecMethod(const QStringList&);
    void             startCloseProcess();
    virtual void     reconnectToDomain();
};

#endif // VM_VIEWER_H
