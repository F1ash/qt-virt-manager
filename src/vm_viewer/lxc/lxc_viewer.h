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
            QObject        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr,
            QString         _domain     = QString());
    const QString    domain;
    bool             domainIsActive = false;
    virDomainPtr     domainPtr      = nullptr;
    void             run();
};

class LXC_Viewer : public TermMainWindow
{
    Q_OBJECT
public:
    explicit LXC_Viewer(
            QWidget        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr,
            QString         arg1       = QString(),
            QString         arg2       = QString(),
            const QString&  work_dir   = nullptr,
            const QString&  command    = nullptr);
    ~LXC_Viewer();

signals:

private:
    lxcHlpThread        *hlpThread;
    LXC_ViewerThread    *viewerThread = nullptr;

public slots:
    void                 init();

private slots:
    void                 timerEvent(QTimerEvent*);
    void                 setTerminalParameters();
    void                 closeEvent(QCloseEvent*);
};

#endif // LXC_VIEWER_H
