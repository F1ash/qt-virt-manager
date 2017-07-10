#ifndef SPICE_VIEWER_H
#define SPICE_VIEWER_H

#include <QDomDocument>
#include <QShortcut>
#include <QTimerEvent>
#include <QScrollArea>
#include "vm_viewer/vm_viewer.h"
#include "vm_viewer/qspice_widgets/qspice-widget.h"
#include "ssh_tunnel/ssh_tunnel.h"

class Spice_Viewer : public VM_Viewer
{
    Q_OBJECT
public:
    explicit Spice_Viewer(
            QWidget        *parent     = nullptr,
            virConnectPtr  *connPtrPtr = nullptr,
            QString         arg1       = QString(),
            QString         arg2       = QString(),
            QString         arg3       = QString());

private:
    QString          transport, addr, user, host;
    uint             port = 0;
    QScrollArea     *scrolled = nullptr;
    QSpiceWidget    *spiceWdg = nullptr;
    QShortcut       *actFullScreen = nullptr;
    SSH_Tunnel      *sshTunnelThread = nullptr;

public slots:
    void             init();
    void             reconnectToVirtDomain();
    void             sendKeySeqToVirtDomain(Qt::Key);
    void             getScreenshotFromVirtDomain();
    void             copyFilesToVirtDomain();
    void             cancelCopyFilesToVirtDomain();
    void             copyToClipboardFromVirtDomain();
    void             pasteClipboardToVirtDomain();
    void             fullScreenVirtDomain();
    void             scaleScreenVirtDomain();

private slots:
    void             initGraphicWidget();
    void             useSSHTunnel(uint);
    void             timerEvent(QTimerEvent*);
    void             resizeViewer(const QSize&);
    void             fullScreenTriggered();
    void             resizeEvent(QResizeEvent*);
    QSize            getWidgetSizeAroundDisplay();
};

#endif // SPICE_VIEWER_H
