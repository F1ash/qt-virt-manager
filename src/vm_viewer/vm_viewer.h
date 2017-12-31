#ifndef VM_VIEWER_H
#define VM_VIEWER_H

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
#include "virt_objects/virt_entity_config.h"
#include "create_widgets/snapshot/create_snapshot_dialog.h"
#include "create_widgets/snapshot/snapshot_action_dialog.h"
#include "ssh_tunnel/ssh_tunnel.h"
#include <QDebug>

#define PERIOD      333

class VM_Viewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit VM_Viewer(
            QWidget        *parent     = nullptr,
            virConnectPtr  *connPtrPtr = nullptr,
            QString         arg1       = QString(),
            QString         arg2       = QString(),
            QString         arg3       = QString());
    virtual ~VM_Viewer();
    virtual void     init();
    QString          connName, domain, addrData, TYPE;
    virConnectPtr*   ptr_ConnPtr;
    virErrorPtr      virtErrors = nullptr;
    QSettings        settings;
    ViewerToolBar   *viewerToolBar = nullptr;
    int              timerId = 0;
    int              killTimerId = 0;
    int              toolBarTimerId = 0;
    int              reinitTimerId = 0;
    uint             killCounter = 0;
    uint             reinitCounter = 0;

    QVBoxLayout     *infoLayout = nullptr;
    QLabel          *icon = nullptr, *err_msg = nullptr;
    QWidget         *info = nullptr;
    QShortcut       *actFullScreen = nullptr;
    SSH_Tunnel      *sshTunnelThread = nullptr;

    QString          transport, addr, socket, user, host;
    uint             port = 0;

signals:
    void             initGraphic();
    void             finished(const QString&);
    void             errorMsg(const QString&);
    void             addNewTask(TASK*);

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

public slots:
    virtual void     initGraphicWidget();
    virtual void     timerEvent(QTimerEvent*);
    virtual void     closeEvent(QCloseEvent*);
    void             useSSHTunnel(quint16);
    void             sendErrMsg(const QString&);
    void             sendErrMsg(const QString&, const uint);
    void             sendConnErrors();
    void             sendGlobalErrors();
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

#endif // VM_VIEWER_H
