#ifndef VM_VIEWER_H
#define VM_VIEWER_H

#include <QMainWindow>
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

signals:
    void             finished();
    void             errorMsg(QString&);
    void             addNewTask(virConnectPtr, QStringList&);
    //void             addNewTask(virConnectPtr, QStringList&, virConnectPtr);

public slots:
    bool             isActive() const;
    void             stopProcessing();
    void             closeEvent(QCloseEvent *ev);
    void             closeViewer();
    void             receiveErrMsg(QString&);
    void             sendConnErrors();
    void             sendGlobalErrors();
    void             resendExecMethod(const QStringList&);
};

#endif // VM_VIEWER_H
