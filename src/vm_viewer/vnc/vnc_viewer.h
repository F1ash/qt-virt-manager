#ifndef VNC_VIEWER_H
#define VNC_VIEWER_H

#include <QDomDocument>
#include <QShortcut>
#include <QTimerEvent>
#include "vm_viewer/vm_viewer.h"
#include "vm_viewer/krdc_vnc_qtonly/Machine_View.h"
#include "ssh_tunnel/ssh_tunnel.h"

class VNC_Viewer : public VM_Viewer
{
    Q_OBJECT
public:
    explicit VNC_Viewer(
            QWidget        *parent     = nullptr,
            virConnectPtr  *connPtrPtr = nullptr,
            QString         arg1       = QString(),
            QString         arg2       = QString(),
            QString         arg3       = QString());

private:
    QString          transport, addr, user, host;
    uint             port = 0;
    MachineView     *vncWdg = nullptr;
    QShortcut       *actFullScreen = nullptr;
    bool             sshTunnelUsed = false;
    SSH_Tunnel      *sshTunnelThread = nullptr;

public slots:
    void             init();
    void             reconnectToVirtDomain();
    void             sendKeySeqToVirtDomain(Qt::Key);
    void             getScreenshotFromVirtDomain();
    void             copyFilesToVirtDomain();
    void             copyToClipboardFromVirtDomain();
    void             pasteClipboardToVirtDomain();
    void             fullScreenVirtDomain();
    void             scaleScreenVirtDomain();

private slots:
    void             initGraphicWidget();
    void             timerEvent(QTimerEvent*);
    void             resizeViewer(const int, const int);
    void             fullScreenTriggered();
    void             resizeEvent(QResizeEvent*);
    QSize            getWidgetSizeAroundDisplay();
};

#endif // VNC_VIEWER_H
