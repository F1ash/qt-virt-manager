#ifndef VNC_VIEWER_ONLY_H
#define VNC_VIEWER_ONLY_H

#include "vm_viewer/vm_viewer_only.h"
#include "vm_viewer/krdc_vnc_qtonly/Machine_View.h"

class VNC_Viewer_Only : public VM_Viewer_Only
{
    Q_OBJECT
public:
    explicit VNC_Viewer_Only(
            QWidget        *parent  = nullptr,
            const QString   _url    = "");

private:
    MachineView     *vncWdg = nullptr;

private slots:
    void             reconnectToVirtDomain();
    void             sendKeySeqToVirtDomain(Qt::Key);
    void             getScreenshotFromVirtDomain();
    void             copyFilesToVirtDomain();
    void             copyToClipboardFromVirtDomain();
    void             pasteClipboardToVirtDomain();
    void             fullScreenVirtDomain();
    void             scaleScreenVirtDomain();

    void             initGraphicWidget();
    void             resizeViewer(const int, const int);
    void             fullScreenTriggered();
    void             resizeEvent(QResizeEvent*);
    QSize            getWidgetSizeAroundDisplay();
    void             connectedToHost();
    void             cantConnect();
};

#endif // VNC_VIEWER_ONLY_H
