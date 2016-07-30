#ifndef VNC_VIEWER_ONLY_H
#define VNC_VIEWER_ONLY_H

#include <QDomDocument>
#include <QShortcut>
#include <QTimerEvent>
#include "vm_viewer/vm_viewer_only.h"
#include "vm_viewer/krdc_vnc_qtonly/Machine_View.h"

class VNC_Viewer_Only : public VM_Viewer_Only
{
    Q_OBJECT
public:
    explicit VNC_Viewer_Only(
            QWidget        *parent  = nullptr,
            const QString   url     = "");

private:
    QString          addr;
    uint             port = 0;
    MachineView     *vncWdg = nullptr;
    QShortcut       *actFullScreen = nullptr;

public slots:
    void             reconnectToVirtDomain();
    void             sendKeySeqToVirtDomain(Qt::Key);
    void             getScreenshotFromVirtDomain();
    void             copyFilesToVirtDomain();
    void             copyToClipboardFromVirtDomain();
    void             pasteClipboardToVirtDomain();
    void             fullScreenVirtDomain();
    void             scaledScreenVirtDomain();

private slots:
    void             initVNCWidget();
    void             timerEvent(QTimerEvent*);
    void             resizeViewer(const int, const int);
    void             fullScreenTriggered();
    void             resizeEvent(QResizeEvent*);
    QSize            getWidgetSizeAroundDisplay();
};

#endif // VNC_VIEWER_ONLY_H
