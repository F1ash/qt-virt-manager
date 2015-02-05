#ifndef VM_VIEWER_H
#define VM_VIEWER_H

#include <QMainWindow>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QTime>
#include <QSettings>
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

signals:
    void             finished();
    void             errorMsg(QString&);

public slots:
    bool             isActive() const;
    void             stopProcessing();
    void             closeEvent(QCloseEvent *ev);
    void             closeViewer();
    void             receiveErrMsg(QString&);
    void             sendConnErrors();
    void             sendGlobalErrors();
};

#endif // VM_VIEWER_H
