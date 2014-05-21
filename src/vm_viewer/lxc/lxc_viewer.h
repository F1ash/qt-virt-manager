#ifndef LXC_VIEWER_H
#define LXC_VIEWER_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include "qtermwidget/lib/vm_qtermwidget.h"
#include <QDebug>

class LXC_Viewer : public QWidget
{
    Q_OBJECT
public:
    explicit LXC_Viewer(QWidget *parent = 0, virConnect *conn = NULL, QString str = QString());
    ~LXC_Viewer();

signals:
    void errorMsg(QString&);

private:
    QString              domain;
    virDomain           *domainPtr = NULL;
    virConnect          *jobConnect = NULL;
    QTermWidget         *display = NULL;
    QLabel              *dontActive;
    QVBoxLayout         *commonLayout;
    virErrorPtr          virtErrors = NULL;
    virStream           *stream = NULL;

public slots:
    void        close();

private slots:
    virDomain*      getDomainPtr() const;
    void            registerStreamEvents();
    void            unregisterStreamEvents();
    static void     freeData(void*);
    static void     streamCallBack(virStreamPtr, int, void*);
    void            sendData(const QString&);

    void            sendConnErrors();
    void            sendGlobalErrors();

};

#endif // LXC_VIEWER_H
