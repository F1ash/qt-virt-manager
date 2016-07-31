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
#include "viewer_toolbar.h"
#include <QDebug>

#define PERIOD      333

class VM_Viewer_Only : public QMainWindow
{
    Q_OBJECT
public:
    explicit VM_Viewer_Only(
            QWidget        *parent  = nullptr,
            const QString   url     = "");
    virtual ~VM_Viewer_Only();
    const QString    url;
    QSettings        settings;
    ViewerToolBar   *viewerToolBar = nullptr;
    uint             timerId = 0;
    uint             killTimerId = 0;
    uint             toolBarTimerId = 0;
    int              startId;
    uint             counter = 0;

    QVBoxLayout     *infoLayout = nullptr;
    QLabel          *icon = nullptr, *msg = nullptr;
    QWidget         *info = nullptr;

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
    void             resendExecMethod(const Act_Param&);
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
    void             showErrorInfo(const QString&);
    void             startAnimatedShow();
    void             startAnimatedHide();

private slots:
    void             toolBarShowed();
    void             hideToolBar();
    void             setNewPosition(const QPoint&);
};

#endif // VM_VIEWER_ONLY_H
