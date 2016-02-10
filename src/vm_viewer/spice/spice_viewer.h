#ifndef SPICE_VIEWER_H
#define SPICE_VIEWER_H

#include <QMainWindow>
#include <QDomDocument>
#include <QShortcut>
#include <QTimerEvent>
#include "vm_viewer/vm_viewer.h"
#include "vm_viewer/qspice_widgets/qspicewidget.h"

class spcHlpThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit spcHlpThread(
            QObject        *parent  = NULL,
            virConnectPtr*  connPtrPtr = NULL,
            QString         _domain = QString());
    const QString    domain;
    bool             domainIsActive = false;
    virDomainPtr     domainPtr = NULL;
    QString          uri, runXmlDesc;
    void             run();
};

class Spice_Viewer : public VM_Viewer
{
    Q_OBJECT
public:
    explicit Spice_Viewer(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtrPtr = NULL,
            QString         arg1    = QString(),
            QString         arg2    = QString());

private:
    spcHlpThread    *hlpThread;
    QString          addr;
    uint             port = 0;
    QSpiceWidget    *spiceWdg = NULL;
    QShortcut       *actFullScreen = NULL;

public slots:
    void             init();
    void             reconnectToVirtDomain();
    void             sendKeySeqToVirtDomain(Qt::Key);
    void             getScreenshotFromVirtDomain();
    void             copyFilesToVirtDomain();
    void             copyToClipboardFromVirtDomain();
    void             pasteClipboardToVirtDomain();

private slots:
    void             initSpiceWidget();
    void             timerEvent(QTimerEvent*);
    void             DisplayResize(const QSize&);
    void             FullScreenTriggered();
    void             resizeEvent(QResizeEvent*);
    QSize            getWidgetSizeAroundDisplay();
};

#endif // SPICE_VIEWER_H
