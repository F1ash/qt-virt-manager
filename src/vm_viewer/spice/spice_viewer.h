#ifndef SPICE_VIEWER_H
#define SPICE_VIEWER_H

#include <QDomDocument>
#include <QScrollArea>
#include "vm_viewer/vm_viewer.h"
#include "vm_viewer/qspice_widgets/qspice-widget.h"

class Spice_Viewer : public VM_Viewer
{
    Q_OBJECT
public:
    explicit Spice_Viewer(
            QWidget        *parent     = Q_NULLPTR,
            virConnectPtr  *connPtrPtr = Q_NULLPTR,
            QString         arg1       = QString(),
            QString         arg2       = QString(),
            QString         arg3       = QString());

private:
    QScrollArea     *scrolled = Q_NULLPTR;
    QSpiceWidget    *spiceWdg = Q_NULLPTR;

private slots:
    void             reconnectToVirtDomain();
    void             disconnectFromVirtDomain();
    void             sendKeySeqToVirtDomain(Qt::Key);
    void             getScreenshotFromVirtDomain();
    void             copyFilesToVirtDomain();
    void             cancelCopyFilesToVirtDomain();
    void             copyToClipboardFromVirtDomain();
    void             pasteClipboardToVirtDomain();
    void             fullScreenVirtDomain();
    void             scaleScreenVirtDomain();

    void             initGraphicWidget();
    void             resizeViewer(const QSize&);
    void             fullScreenTriggered();
    void             resizeEvent(QResizeEvent*);
    QSize            getWidgetSizeAroundDisplay();
};

#endif // SPICE_VIEWER_H
