#ifndef LXC_VIEWER_H
#define LXC_VIEWER_H

#include <QTimerEvent>
#include "vm_viewer/qterminal/term_mainwindow.h"
#include "lxc_viewer_thread.h"
#include <QDebug>

class lxcHlpThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit lxcHlpThread(
            QObject        *parent      = NULL,
            virConnectPtr*  connPtrPtr  = NULL,
            QString         _domain     = QString());
    const QString    domain;
    bool             domainIsActive = false;
    virDomainPtr     domainPtr = NULL;
    void             run();
};

class LXC_Viewer : public TermMainWindow
{
    Q_OBJECT
public:
    explicit LXC_Viewer(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtrPtr = NULL,
            QString         arg1    = QString(),
            QString         arg2    = QString(),
            const QString&  work_dir= NULL,
            const QString&  command = NULL);
    ~LXC_Viewer();

signals:

private:
    lxcHlpThread        *hlpThread;
    LXC_ViewerThread    *viewerThread = NULL;

public slots:
    void                 init();

private slots:
    void                 timerEvent(QTimerEvent*);
    void                 setTerminalParameters();
    void                 closeEvent(QCloseEvent*);
};

#endif // LXC_VIEWER_H
