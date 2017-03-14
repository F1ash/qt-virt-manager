#include "spice_viewer_only.h"
#include <QApplication>
extern "C" {
#include <spice/vd_agent.h>
}

Spice_Viewer_Only::Spice_Viewer_Only(
        QWidget        *parent,
        const QString   url) :
    VM_Viewer_Only(parent, url)
{
    startId = startTimer(1000);
}

/* public slots */
void Spice_Viewer_Only::reconnectToVirtDomain()
{
    if ( nullptr!=spiceWdg ) {
        delete spiceWdg;
        spiceWdg = nullptr;
        // resizing to any,
        // because will need to init new display configuration
        //resize(getWidgetSizeAroundDisplay());
        initSpiceWidget();
        QSize around_size = getWidgetSizeAroundDisplay();
        if ( nullptr!=spiceWdg ) {
            spiceWdg->updateSize(
                        size().width()-around_size.width(),
                        size().height()-around_size.height());
        };
    };
}
void Spice_Viewer_Only::sendKeySeqToVirtDomain(Qt::Key key)
{
    if ( nullptr==spiceWdg ) return;
    spiceWdg->sendKeySequience(key);
}
void Spice_Viewer_Only::getScreenshotFromVirtDomain()
{
    if ( nullptr==spiceWdg ) return;
    spiceWdg->getScreenshot();
}
void Spice_Viewer_Only::copyFilesToVirtDomain()
{
    if ( nullptr==spiceWdg ) return;
    QStringList fileNames = QFileDialog::getOpenFileNames(
                this, "Copy files to Guest", "~");
    spiceWdg->fileCopyAsync(fileNames);
}
void Spice_Viewer_Only::cancelCopyFilesToVirtDomain()
{
    if ( nullptr==spiceWdg ) return;
    spiceWdg->cancelFileCopyAsync();
}
void Spice_Viewer_Only::copyToClipboardFromVirtDomain()
{
    if ( nullptr!=spiceWdg )
        spiceWdg->copyClipboardDataFromGuest();
}
void Spice_Viewer_Only::pasteClipboardToVirtDomain()
{
    if ( nullptr!=spiceWdg )
        spiceWdg->pasteClipboardDataToGuest();
}
void Spice_Viewer_Only::fullScreenVirtDomain()
{
    fullScreenTriggered();
}

/* private slots */
void Spice_Viewer_Only::initSpiceWidget()
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
    connect(spiceWdg, SIGNAL(clipboardsReleased(bool)),
            viewerToolBar, SLOT(setCopyPasteState(bool)));
    connect(spiceWdg, SIGNAL(boarderTouched()),
            this, SLOT(startAnimatedShow()));
    connect(spiceWdg, SIGNAL(mouseClickedInto()),
            this, SLOT(startAnimatedHide()));
    connect(spiceWdg, SIGNAL(displayChannelChanged(bool)),
            this, SLOT(displayChannelState(bool)));
    spiceWdg->connectToSpiceSource(url);
}

void Spice_Viewer_Only::timerEvent(QTimerEvent *ev)
{
    if ( ev->timerId()==killTimerId ) {
        counter++;
        viewerToolBar->vm_stateWdg->setCloseProcessValue(counter*PERIOD*6);
        if ( TIMEOUT<counter*PERIOD*6 ) {
            killTimer(killTimerId);
            killTimerId = 0;
            counter = 0;
            close();
        };
    } else if ( ev->timerId()==toolBarTimerId ) {
        startAnimatedHide();
    } else if ( ev->timerId()==startId ) {
        if ( cycles==0 ) {
            initSpiceWidget();
        } else if ( cycles==9 ) {
            killTimer(startId);
            startId = 0;
            if ( spiceWdg==nullptr || !spiceWdg->isConnectedWithDisplay() ) {
                delete spiceWdg;
                spiceWdg = nullptr;
                delete scrolled;
                scrolled = nullptr;
                showErrorInfo("");
            };
        };
        ++cycles;
    }
}

void Spice_Viewer_Only::resizeViewer(const QSize &_size)
{
    if ( spiceWdg==nullptr ||
         !spiceWdg->isConnectedWithDisplay() ) return;
    QSize around_size = getWidgetSizeAroundDisplay();
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
    if ( nullptr!=spiceWdg ) {
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
    viewerToolBar->getContentsMargins(&left, &top, &right, &bottom);
    _width = left+right;
    _height = top +bottom;
    if ( nullptr!=scrolled ) {
        scrolled->getContentsMargins(&left, &top, &right, &bottom);
        _width += left+right;
        _height += top +bottom;
    };
    if ( nullptr!=spiceWdg ) {
        spiceWdg->getContentsMargins(&left, &top, &right, &bottom);
        _width += left+right;
        _height += top +bottom;
    };
    getContentsMargins(&left, &top, &right, &bottom);
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
        killTimer(startId);
        setWindowTitle(QString("Qt Remote Viewer -- %1").arg(url));
    } else {
        delete spiceWdg;
        spiceWdg = nullptr;
        delete scrolled;
        scrolled = nullptr;
        showErrorInfo("");
    };
}
