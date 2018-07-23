#ifndef SPICE_VIEWER_ONLY_H
#define SPICE_VIEWER_ONLY_H

#include <QScrollArea>
#include "vm_viewer/vm_viewer_only.h"
#include "vm_viewer/qspice_widgets/qspice-widget.h"

class Spice_Viewer_Only : public VM_Viewer_Only
{
    Q_OBJECT
public:
    explicit Spice_Viewer_Only(
            QWidget        *parent  = Q_NULLPTR,
            const QString   _url    = "");

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
    void             displayChannelState(bool);

};

#endif // SPICE_VIEWER_ONLY_H
