#ifndef VNC_VIEWER_H
#define VNC_VIEWER_H

#include <QDomDocument>
#include "vm_viewer/vm_viewer.h"
#include "vm_viewer/krdc_vnc_qtonly/Machine_View.h"

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
    MachineView     *vncWdg = nullptr;

private slots:
    void             reconnectToVirtDomain();
    void             disconnectFromVirtDomain();
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
};

#endif // VNC_VIEWER_H
