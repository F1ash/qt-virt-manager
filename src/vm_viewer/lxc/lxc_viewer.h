#ifndef LXC_VIEWER_H
#define LXC_VIEWER_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include "qtermwidget/lib/qtermwidget.h"
#include <unistd.h>
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
    QString         terminalToStream;
    QString         domain;
    virDomain      *domainPtr = NULL;
    virConnect     *jobConnect = NULL;
    QTermWidget    *display = NULL;
    QLabel         *dontActive;
    QVBoxLayout    *commonLayout;
    virErrorPtr     virtErrors = NULL;
    virStream      *stream = NULL;
    int             ptySlaveFd = -1;

public slots:
    void            close();

private slots:
    virDomain*      getDomainPtr() const;
    int             registerStreamEvents();
    int             unregisterStreamEvents();
    static void     freeData(void*);
    static void     streamEventCallBack(virStreamPtr, int, void*);
    void            updateStreamEvents(virStreamPtr, int);
    void            sendDataToDisplay(virStreamPtr);
    void            sendDataToVMachine(virStreamPtr);
    void            sendDataToVMachine(const QByteArray&);
    void            streamClosed();

    void            sendConnErrors();
    void            sendGlobalErrors();

};

#endif // LXC_VIEWER_H
