#ifndef SPICE_VIEWER_ONLY_H
#define SPICE_VIEWER_ONLY_H

#include <QDomDocument>
#include <QShortcut>
#include <QTimerEvent>
#include <QScrollArea>
#include "vm_viewer/vm_viewer_only.h"
#include "vm_viewer/qspice_widgets/qspice-widget.h"

class Spice_Viewer_Only : public VM_Viewer_Only
{
    Q_OBJECT
public:
    explicit Spice_Viewer_Only(
            QWidget        *parent  = nullptr,
            const QString   url     = "");

private:
    QString          addr;
    uint             port = 0;
    QScrollArea     *scrolled = nullptr;
    QSpiceWidget    *spiceWdg = nullptr;
    QShortcut       *actFullScreen = nullptr;

public slots:
    void             reconnectToVirtDomain();
    void             sendKeySeqToVirtDomain(Qt::Key);
    void             getScreenshotFromVirtDomain();
    void             copyFilesToVirtDomain();
    void             cancelCopyFilesToVirtDomain();
    void             copyToClipboardFromVirtDomain();
    void             pasteClipboardToVirtDomain();
    void             fullScreenVirtDomain();
    void             scaledScreenVirtDomain();

private slots:
    void             initSpiceWidget();
    void             timerEvent(QTimerEvent*);
    void             resizeViewer(const QSize&);
    void             fullScreenTriggered();
    void             resizeEvent(QResizeEvent*);
    QSize            getWidgetSizeAroundDisplay();
};

#endif // SPICE_VIEWER_ONLY_H
