#ifndef LXC_VIEWER_H
#define LXC_VIEWER_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimerEvent>
#include <QSocketNotifier>
#include <QApplication>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include "qtermwidget/lib/qtermwidget.h"
#include <unistd.h>
#include <QDebug>

class LXC_Viewer : public QTermWidget
{
    Q_OBJECT
public:
    explicit LXC_Viewer(int startnow = 0, QWidget *parent = 0, virConnect *conn = NULL, QString str = QString());
    ~LXC_Viewer();

signals:
    void errorMsg(QString&);
    void jobFinished();

private:
    QString         domain;
    virDomain      *domainPtr = NULL;
    virConnect     *jobConnect = NULL;
    virErrorPtr     virtErrors = NULL;
    virStream      *stream = NULL;
    int             ptySlaveFd = -1;
    uint            timerId = 0;
    uint            counter = 0;
    QSocketNotifier
                   *readSlaveFd = NULL;

public slots:
    void            closeTerminal();

private slots:
    void            timerEvent(QTimerEvent*);
    void            closeEvent(QCloseEvent*);
    virDomain*      getDomainPtr() const;
    int             registerStreamEvents();
    int             unregisterStreamEvents();
    static void     freeData(void*);
    static void     streamEventCallBack(virStreamPtr, int, void*);
    void            updateStreamEvents(virStreamPtr, int);
    void            sendDataToDisplay(virStreamPtr);
    void            sendDataToVMachine(int);
    void            closeStream();

    void            sendConnErrors();
    void            sendGlobalErrors();

};

#endif // LXC_VIEWER_H
