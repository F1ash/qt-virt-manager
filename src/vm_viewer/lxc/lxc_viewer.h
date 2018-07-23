#ifndef LXC_VIEWER_H
#define LXC_VIEWER_H

#include <QTimerEvent>
#include "vm_viewer/qterminal/term_mainwindow.h"
#include "lxc_viewer_thread.h"
//#include <QDebug>

class lxcHlpThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit lxcHlpThread(
            QObject        *parent      = Q_NULLPTR,
            virConnectPtr  *connPtrPtr  = Q_NULLPTR,
            QString         _domain     = QString());
    const QString    domain;
    bool             domainIsActive = false;
    virDomainPtr     domainPtr      = Q_NULLPTR;
    void             run();
};

class LXC_Viewer : public TermMainWindow
{
    Q_OBJECT
public:
    explicit LXC_Viewer(
            QWidget        *parent     = Q_NULLPTR,
            virConnectPtr  *connPtrPtr = Q_NULLPTR,
            QString         arg1       = QString(),
            QString         arg2       = QString(),
            const QString&  work_dir   = Q_NULLPTR,
            const QString&  command    = Q_NULLPTR);
    ~LXC_Viewer();

signals:

private:
    lxcHlpThread        *hlpThread;
    LXC_ViewerThread    *viewerThread = Q_NULLPTR;

public slots:
    void                 init();

private slots:
    void                 timerEvent(QTimerEvent*);
    void                 setTerminalParameters();
    void                 closeEvent(QCloseEvent*);
};

#endif // LXC_VIEWER_H
