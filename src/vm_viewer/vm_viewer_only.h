#ifndef VM_VIEWER_ONLY_H
#define VM_VIEWER_ONLY_H

#include <QMainWindow>
#include <QStatusBar>
#include <QCloseEvent>
#include <QMessageBox>
#include <QTime>
#include <QFileDialog>
#include <QSettings>
#include <QIcon>
#include <QVBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPoint>
#include <QShortcut>
#include <QTimerEvent>
#include "viewer_toolbar.h"
#include "ssh_tunnel/ssh_tunnel.h"
#include <QDebug>

#define PERIOD      333

class VM_Viewer_Only : public QMainWindow
{
    Q_OBJECT
public:
    explicit VM_Viewer_Only(
            QWidget        *parent  = nullptr,
            const QString   _url    = "");
    virtual ~VM_Viewer_Only();
    virtual void     init();
    void             parseURL();
    const QString    url;
    QString          user, host, transport,
                     addr, port, socket;
    QSettings        settings;
    ViewerToolBar   *viewerToolBar = nullptr;
    uint             timerId = 0;
    int              killTimerId = 0;
    int              toolBarTimerId = 0;
    int              reinitTimerId = 0;
    uint             killCounter = 0;
    uint             reinitCounter = 0;

    QVBoxLayout     *infoLayout = nullptr;
    QLabel          *icon = nullptr, *msg = nullptr;
    QWidget         *info = nullptr;
    SSH_Tunnel      *sshTunnelThread = nullptr;
    QShortcut       *actFullScreen = nullptr;

signals:
    void             initGraphic();
    /*
     * Emitted, when user touched top boarder.
     * Used for show toolbar.
     */
    void             boarderTouched();

    /*
     * Emitted, when user clicked onto widget area.
     * Used for hide toolbar.
     */
    void             mouseClickedInto();

private:
    QPoint           toolBarPoint;
    QPropertyAnimation
                    *animatedShowToolBar,
                    *animatedHideToolBar;
    void             useFullScreen();
    void             startSSHTunnel(QString, QString);

public slots:
    virtual void     initGraphicWidget();
    virtual void     timerEvent(QTimerEvent*);
    void             useSSHTunnel(quint16);
    void             resendExecMethod(const Act_Param&);
    void             startCloseProcess();
    virtual void     reconnectToVirtDomain();
    virtual void     disconnectFromVirtDomain();
    virtual void     sendKeySeqToVirtDomain(Qt::Key);
    virtual void     getScreenshotFromVirtDomain();
    virtual void     copyFilesToVirtDomain();
    virtual void     cancelCopyFilesToVirtDomain();
    virtual void     copyToClipboardFromVirtDomain();
    virtual void     pasteClipboardToVirtDomain();
    virtual void     fullScreenVirtDomain();
    virtual void     scaleScreenVirtDomain();
    void             showErrorInfo(const QString&);
    void             startAnimatedShow();
    void             startAnimatedHide();

private slots:
    void             toolBarShowed();
    void             hideToolBar();
    void             setNewPosition(const QPoint&);
    void             sshThreadFinished();
};

#endif // VM_VIEWER_ONLY_H
