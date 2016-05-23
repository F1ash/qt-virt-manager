#ifndef VNC_VIEWER_H
#define VNC_VIEWER_H

#include <QDomDocument>
#include <QShortcut>
#include <QTimerEvent>
#include "vm_viewer/vm_viewer.h"
#include "vm_viewer/krdc_vnc_qtonly/Machine_View.h"

class vncHlpThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit vncHlpThread(
            QObject        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr,
            QString         _domain    = QString());
    const QString    domain;
    bool             domainIsActive = false;
    virDomainPtr     domainPtr = nullptr;
    QString          uri, runXmlDesc;
    void             run();
};

class VNC_Viewer : public VM_Viewer
{
    Q_OBJECT
public:
    explicit VNC_Viewer(
            QWidget        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr,
            QString         arg1       = QString(),
            QString         arg2       = QString());

private:
    vncHlpThread    *hlpThread;
    QString          addr, guestName;
    uint             port = 0;
    MachineView     *vncWdg = nullptr;
    QShortcut       *actFullScreen = nullptr;

public slots:
    void             init();
    void             reconnectToVirtDomain();
    void             sendKeySeqToVirtDomain(Qt::Key);
    void             getScreenshotFromVirtDomain();
    void             copyFilesToVirtDomain();
    void             copyToClipboardFromVirtDomain();
    void             pasteClipboardToVirtDomain();
    void             fullScreenVirtDomain();

private slots:
    void             initVNCWidget();
    void             timerEvent(QTimerEvent*);
    void             resizeViewer(const int, const int);
    void             fullScreenTriggered();
    void             resizeEvent(QResizeEvent*);
    QSize            getWidgetSizeAroundDisplay();
};

#endif // VNC_VIEWER_H
