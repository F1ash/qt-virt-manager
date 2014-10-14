#ifndef VM_VIEWER_H
#define VM_VIEWER_H

#include <QMainWindow>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include "viewer_toolbar.h"
#include "vm_viewer/lxc/lxc_viewer.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QTime>
#include <QSettings>
#include <QDebug>

class VM_Viewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit VM_Viewer(QWidget *parent = NULL,
                       virConnect *conn = NULL,
                       QString arg1 = QString(),
                       QString arg2 = QString());
    ~VM_Viewer();

signals:
    void finished();
    void errorMsg(QString&);

private:
    QString          connName, domain, type;
    virConnect      *jobConnect = NULL;
    QWidget         *viewer = NULL;
    ViewerToolBar   *toolBar;
    QMenu           *actionsMenu = NULL;
    QMenuBar        *menuBar = NULL;

    bool             VM_State;
    QSettings        settings;

public slots:
    bool isActive() const;
    void stopProcessing();

private slots:
    void closeEvent(QCloseEvent *ev);
    void closeViewer();
    void receiveErrMsg(QString&);

};

#endif // VM_VIEWER_H
