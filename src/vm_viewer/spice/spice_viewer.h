#ifndef SPICE_VIEWER_H
#define SPICE_VIEWER_H

#include <QDomDocument>
#include <QShortcut>
#include <QTimerEvent>
#include <QScrollArea>
#include "vm_viewer/vm_viewer.h"
#include "vm_viewer/qspice_widgets/qspice-widget.h"

class spcHlpThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit spcHlpThread(
            QObject        *parent     = nullptr,
            virConnectPtr  *connPtrPtr = nullptr,
            QString         _domain    = QString());
    const QString    domain;
    bool             domainIsActive = false;
    virDomainPtr     domainPtr = nullptr;
    QString          uri, activeDomainXmlDesc;
    void             run();
};

class Spice_Viewer : public VM_Viewer
{
    Q_OBJECT
public:
    explicit Spice_Viewer(
            QWidget        *parent     = nullptr,
            virConnectPtr  *connPtrPtr = nullptr,
            QString         arg1       = QString(),
            QString         arg2       = QString());

private:
    spcHlpThread    *hlpThread;
    QString          addr;
    uint             port = 0;
    QScrollArea     *scrolled = nullptr;
    QSpiceWidget    *spiceWdg = nullptr;
    QShortcut       *actFullScreen = nullptr;

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
    void             scaledScreenVirtDomain();

private slots:
    void             initSpiceWidget();
    void             timerEvent(QTimerEvent*);
    void             resizeViewer(const QSize&);
    void             fullScreenTriggered();
    void             resizeEvent(QResizeEvent*);
    QSize            getWidgetSizeAroundDisplay();
};

#endif // SPICE_VIEWER_H
