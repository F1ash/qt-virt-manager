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
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include "vm_viewer/viewer_toolbar.h"
#include "virt_objects/control_thread.h"
#include <QDebug>

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
    QString          connName, domain;
    virConnect      *jobConnect = NULL;
    virErrorPtr      virtErrors = NULL;
    bool             VM_State;
    QSettings        settings;
    ViewerToolBar   *viewerToolBar = NULL;
    QProgressBar    *closeProcess = NULL;

signals:
    void             finished(QString&);
    void             errorMsg(QString&);
    void             addNewTask(virConnectPtr, QStringList&);
    //void             addNewTask(virConnectPtr, QStringList&, virConnectPtr);

public slots:
    virtual bool     isActive() const;
    virtual void     closeEvent(QCloseEvent *ev);
    virtual void     closeEvent();
    virtual void     closeViewer();
    void             sendErrMsg(QString&);
    void             sendConnErrors();
    void             sendGlobalErrors();
    void             resendExecMethod(const QStringList&);
};

#endif // VM_VIEWER_H
