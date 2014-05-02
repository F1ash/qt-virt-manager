#ifndef VM_VIEWER_H
#define VM_VIEWER_H

#include <QMainWindow>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include "viewer_toolbar.h"
#include "vm_viewer/lxc/lxc_viewer.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>

class VM_Viewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit VM_Viewer(QWidget *parent = 0, virConnect *conn = NULL, QString str = QString());
    ~VM_Viewer();

signals:

private:
    QString          domain, type;
    virConnect      *jobConnect = NULL;
    QWidget         *viewer = NULL;
    ViewerToolBar   *toolBar;

    bool             VM_State;

public slots:
    bool isActive() const;
    void stopProcessing();

private slots:
    void closeEvent();

};

#endif // VM_VIEWER_H
