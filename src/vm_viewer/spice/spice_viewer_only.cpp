#include "spice_viewer_only.h"
#include <QApplication>
extern "C" {
#include <spice/vd_agent.h>
}

Spice_Viewer_Only::Spice_Viewer_Only(
        QWidget        *parent,
        const QString   _url) :
    VM_Viewer_Only(parent, _url)
{
    init();
}

/* private slots */
void Spice_Viewer_Only::reconnectToVirtDomain()
{
    if ( Q_NULLPTR!=spiceWdg ) {
        delete spiceWdg;
        spiceWdg = Q_NULLPTR;
        // resizing to any,
        // because will need to init new display configuration
        //resize(getWidgetSizeAroundDisplay());
        initGraphicWidget();
        QSize around_size = getWidgetSizeAroundDisplay();
        if ( Q_NULLPTR!=spiceWdg ) {
            spiceWdg->updateSize(
                        size().width()-around_size.width(),
                        size().height()-around_size.height());
        };
    } else {
        initGraphicWidget();
    };
}
void Spice_Viewer_Only::disconnectFromVirtDomain()
{
    if ( spiceWdg!=Q_NULLPTR ) {
        spiceWdg->disconnectFromSpiceSource();
    };
}
void Spice_Viewer_Only::sendKeySeqToVirtDomain(Qt::Key key)
{
    if ( Q_NULLPTR==spiceWdg ) return;
    spiceWdg->sendKeySequience(key);
}
void Spice_Viewer_Only::getScreenshotFromVirtDomain()
{
    if ( Q_NULLPTR==spiceWdg ) return;
    spiceWdg->getScreenshot();
}
void Spice_Viewer_Only::copyFilesToVirtDomain()
{
    if ( Q_NULLPTR==spiceWdg ) return;
    QStringList fileNames = QFileDialog::getOpenFileNames(
                this, tr("Copy files to Guest"), "~");
    spiceWdg->fileCopyAsync(fileNames);
}
void Spice_Viewer_Only::cancelCopyFilesToVirtDomain()
{
    if ( Q_NULLPTR==spiceWdg ) return;
    spiceWdg->cancelFileCopyAsync();
}
void Spice_Viewer_Only::copyToClipboardFromVirtDomain()
{
    if ( Q_NULLPTR!=spiceWdg )
        spiceWdg->copyToClipboardFromGuest();
}
void Spice_Viewer_Only::pasteClipboardToVirtDomain()
{
    if ( Q_NULLPTR!=spiceWdg )
        spiceWdg->pasteClipboardToGuest();
}
void Spice_Viewer_Only::fullScreenVirtDomain()
{
    fullScreenTriggered();
}

void Spice_Viewer_Only::initGraphicWidget()
{
    spiceWdg = new QSpiceWidget(this);
    spiceWdg->setAttribute(Qt::WA_OpaquePaintEvent, true);
    scrolled = new QScrollArea(this);
    scrolled->setWidgetResizable(true);
    scrolled->setAlignment(Qt::AlignCenter);
    scrolled->setWidget(spiceWdg);
    setCentralWidget(scrolled);
    spiceWdg->setFullScreen(isFullScreen());
    connect(spiceWdg, SIGNAL(displayResized(const QSize&)),
            SLOT(resizeViewer(const QSize&)));
    connect(spiceWdg, SIGNAL(downloaded(int,int)),
            viewerToolBar->vm_stateWdg,
            SLOT(setDownloadProcessValue(int,int)));
    connect(spiceWdg, SIGNAL(fileTransferIsCancelled()),
            viewerToolBar, SLOT(downloadCancelled()));
    connect(spiceWdg, SIGNAL(fileTransferIsCompleted()),
            viewerToolBar, SLOT(downloadCompleted()));
    connect(spiceWdg, SIGNAL(displayChannelChanged(bool)),
            viewerToolBar->vm_stateWdg, SLOT(changeDisplayState(bool)));
    connect(spiceWdg, SIGNAL(cursorChannelChanged(bool)),
            viewerToolBar->vm_stateWdg, SLOT(changeMouseState(bool)));
    connect(spiceWdg, SIGNAL(inputsChannelChanged(bool)),
            viewerToolBar->vm_stateWdg, SLOT(changeKeyboardState(bool)));
    connect(spiceWdg, SIGNAL(usbredirChannelChanged(bool)),
            viewerToolBar->vm_stateWdg, SLOT(changeUsbredirState(bool)));
    connect(spiceWdg, SIGNAL(smartcardChannelChanged(bool)),
            viewerToolBar->vm_stateWdg, SLOT(changeSmartcardState(bool)));
    connect(spiceWdg, SIGNAL(webdavChannelChanged(bool)),
            viewerToolBar->vm_stateWdg, SLOT(changeWebDAVState(bool)));
    connect(spiceWdg, SIGNAL(playbackChannelChanged(bool)),
            viewerToolBar->vm_stateWdg, SLOT(changePlaybackState(bool)));
    connect(spiceWdg, SIGNAL(recordChannelChanged(bool)),
            viewerToolBar->vm_stateWdg, SLOT(changeRecordState(bool)));
    connect(viewerToolBar->vm_stateWdg, SIGNAL(showUsbDevWidget()),
            spiceWdg, SLOT(showUsbDevWidget()));
    connect(viewerToolBar->vm_stateWdg, SIGNAL(showSmartCardWidget()),
            spiceWdg, SLOT(showSmartCardWidget()));
    connect(viewerToolBar->vm_stateWdg,
            SIGNAL(transformationMode(Qt::TransformationMode)),
            spiceWdg, SLOT(setTransformationMode(Qt::TransformationMode)));
    //connect(spiceWdg, SIGNAL(errMsg(const QString&)),
    //        this, SLOT(sendErrMsg(const QString&)));
    connect(spiceWdg, SIGNAL(copyPasteStateChanged(bool)),
            viewerToolBar, SLOT(setCopyPasteState(bool)));
    connect(spiceWdg, SIGNAL(boarderTouched()),
            this, SLOT(startAnimatedShow()));
    connect(spiceWdg, SIGNAL(mouseClickedInto()),
            this, SLOT(startAnimatedHide()));
    connect(spiceWdg, SIGNAL(displayChannelChanged(bool)),
            this, SLOT(displayChannelState(bool)));

    QSize around_size = getWidgetSizeAroundDisplay();
    //QTextStream s(stdout);
    //s<<"address: "<<addr<<":"<<port<<endl;
    QString _uri = QString("spice://%1:%2").arg(addr).arg(port);
    spiceWdg->connectToSpiceSource(_uri, passwd);
    spiceWdg->setNewSize(around_size.width(), around_size.height());
}

void Spice_Viewer_Only::resizeViewer(const QSize &_size)
{
    if ( spiceWdg==Q_NULLPTR ||
         !spiceWdg->isConnectedWithDisplay() ) return;
    QSize around_size = getWidgetSizeAroundDisplay();
    // will be showed when occured successful connection to VM
    if ( !isVisible() ) {
        this->show();
    };
    if ( _size+around_size==size() ) {
        return;
    };
    resize(_size+around_size);
}

void Spice_Viewer_Only::fullScreenTriggered()
{
    if (isFullScreen()) {
        setWindowState(Qt::WindowNoState);
        //scrolled->setWindowFlags( Qt::Widget );
        //scrolled->showNormal();
        spiceWdg->setFullScreen(false);
        scrolled->setPalette( QPalette() );
    } else {
        setWindowState(Qt::WindowFullScreen);
        //scrolled->setWindowFlags( Qt::Window );
        //scrolled->showFullScreen();
        QPalette p;
        p.setColor( QPalette::Background, QColor(22,22,22) );
        scrolled->setPalette( p );
        spiceWdg->setFullScreen(true);
        //qreal newW = scrolled->maximumViewportSize().width();
        //qreal newH = scrolled->maximumViewportSize().height();
        //spiceWdg->setMaximumSize(newW, newH);
        //spiceWdg->resize(newW, newH);
    };
    startAnimatedHide();
}

void Spice_Viewer_Only::scaleScreenVirtDomain()
{
    spiceWdg->setScaledScreen(true);
}

void Spice_Viewer_Only::resizeEvent(QResizeEvent *ev)
{
    ev->accept();
    if ( Q_NULLPTR!=spiceWdg ) {
        if ( !spiceWdg->isConnectedWithDisplay() ) return;
        QSize around_size = getWidgetSizeAroundDisplay();
        spiceWdg->updateSize(
                    ev->size().width()-around_size.width(),
                    ev->size().height()-around_size.height());
    } else
        return;
    return QMainWindow::resizeEvent(ev);
}

QSize Spice_Viewer_Only::getWidgetSizeAroundDisplay()
{
    int left, top, right, bottom, _width, _height;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QMargins _m = viewerToolBar->contentsMargins();
    left    = _m.left();
    top     = _m.top();
    right   = _m.right();
    bottom  = _m.bottom();
#else
    viewerToolBar->getContentsMargins(&left, &top, &right, &bottom);
#endif
    _width = left+right;
    _height = top +bottom;
    if ( Q_NULLPTR!=scrolled ) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        _m = scrolled->contentsMargins();
        left    = _m.left();
        top     = _m.top();
        right   = _m.right();
        bottom  = _m.bottom();
#else
        scrolled->getContentsMargins(&left, &top, &right, &bottom);
#endif
        _width += left+right;
        _height += top +bottom;
    };
    if ( Q_NULLPTR!=spiceWdg ) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        _m = spiceWdg->contentsMargins();
        left    = _m.left();
        top     = _m.top();
        right   = _m.right();
        bottom  = _m.bottom();
#else
        spiceWdg->getContentsMargins(&left, &top, &right, &bottom);
#endif
        _width += left+right;
        _height += top +bottom;
    };
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    _m = contentsMargins();
    left    = _m.left();
    top     = _m.top();
    right   = _m.right();
    bottom  = _m.bottom();
#else
    getContentsMargins(&left, &top, &right, &bottom);
#endif
    _width += left+right;
    _height += top +bottom;
    QSize _size(_width, _height);
    return _size;
}

void Spice_Viewer_Only::displayChannelState(bool state)
{
    if ( state ) {
        QSize around_size = getWidgetSizeAroundDisplay();
        spiceWdg->setNewSize(around_size.width(), around_size.height());
        setWindowTitle(QString(tr("Qt Remote Viewer -- %1")).arg(url));
    } else {
        delete spiceWdg;
        spiceWdg = Q_NULLPTR;
        delete scrolled;
        scrolled = Q_NULLPTR;
        showErrorInfo("");
    };
}
