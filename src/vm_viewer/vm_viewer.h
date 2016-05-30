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
#include "viewer_toolbar.h"
#include "virt_objects/virt_entity_config.h"
#include "virt_objects/_virt_thread.h"
#include "create_widgets/snapshot/create_snapshot_dialog.h"
#include "create_widgets/snapshot/snapshot_action_dialog.h"
#include <QDebug>

#define PERIOD      333

class VM_Viewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit VM_Viewer(
            QWidget        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr,
            QString         arg1       = QString(),
            QString         arg2       = QString());
    virtual ~VM_Viewer();
    QString          connName, domain, TYPE;
    virConnectPtr*   ptr_ConnPtr;
    virErrorPtr      virtErrors = nullptr;
    QSettings        settings;
    ViewerToolBar   *viewerToolBar = nullptr;
    uint             timerId = 0;
    uint             killTimerId = 0;
    uint             toolBarTimerId = 0;
    uint             counter = 0;

    QVBoxLayout     *infoLayout = nullptr;
    QLabel          *icon = nullptr, *msg = nullptr;
    QWidget         *info = nullptr;

signals:
    void             finished(QString&);
    void             errorMsg(QString&);
    void             addNewTask(TASK);

    /*
     * Emitted, when user touched top boarder.
     * Used for show toolbar.
     */
    void boarderTouched();

    /*
     * Emitted, when user clicked onto widget area.
     * Used for hide toolbar.
     */
    void mouseClickedInto();

private:
    QPoint           toolBarPoint;
    QPropertyAnimation
                    *animatedShowToolBar,
                    *animatedHideToolBar;

public slots:
    virtual void     init();
    virtual void     closeEvent(QCloseEvent *ev);
    void             sendErrMsg(QString&);
    void             sendErrMsg(QString&, uint);
    void             sendConnErrors();
    void             sendGlobalErrors();
    void             resendExecMethod(const QStringList&);
    void             startCloseProcess();
    virtual void     reconnectToVirtDomain();
    virtual void     sendKeySeqToVirtDomain(Qt::Key);
    virtual void     getScreenshotFromVirtDomain();
    virtual void     copyFilesToVirtDomain();
    virtual void     cancelCopyFilesToVirtDomain();
    virtual void     copyToClipboardFromVirtDomain();
    virtual void     pasteClipboardToVirtDomain();
    virtual void     fullScreenVirtDomain();
    virtual void     scaledScreenVirtDomain();
    void             showErrorInfo(QString&);
    void             startAnimatedShow();
    void             startAnimatedHide();

private slots:
    void             toolBarShowed();
    void             hideToolBar();
    void             setNewPosition(const QPoint&);
};

#endif // VM_VIEWER_H
