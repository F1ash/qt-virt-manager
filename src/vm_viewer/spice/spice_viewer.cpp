#include "spice_viewer.h"
#include <QApplication>
extern "C" {
#include <spice/vd_agent.h>
}

Spice_Viewer::Spice_Viewer(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr,
        QString          arg1,
        QString          arg2,
        QString          arg3) :
    VM_Viewer(parent, connPtrPtr, arg1, arg2, arg3)
{
    TYPE = "SPICE";
    QStringList _addrData = addrData.split(";");
    if ( _addrData.count()>4 ) {
        user        = _addrData.at(0);
        host        = _addrData.at(1);
        transport   = _addrData.at(2);
        addr        = _addrData.at(3);
        port        = _addrData.at(4).toInt();
    };
    init();
}

/* private slots */
void Spice_Viewer::reconnectToVirtDomain()
{
    if ( nullptr!=spiceWdg ) {
        delete spiceWdg;
        spiceWdg = nullptr;
        // resizing to any,
        // because will need to init new display configuration
        //resize(getWidgetSizeAroundDisplay());
        initGraphicWidget();
        QSize around_size = getWidgetSizeAroundDisplay();
        if ( nullptr!=spiceWdg ) {
            spiceWdg->updateSize(
                        size().width()-around_size.width(),
                        size().height()-around_size.height());
        };
    } else {
        initGraphicWidget();
    };
}
void Spice_Viewer::disconnectFromVirtDomain()
{
    if ( spiceWdg!=nullptr ) {
        spiceWdg->disconnectFromSpiceSource();
    };
}
void Spice_Viewer::sendKeySeqToVirtDomain(Qt::Key key)
{
    if ( nullptr==spiceWdg ) return;
    spiceWdg->sendKeySequience(key);
}
void Spice_Viewer::getScreenshotFromVirtDomain()
{
    if ( nullptr==spiceWdg ) return;
    spiceWdg->getScreenshot();
}
void Spice_Viewer::copyFilesToVirtDomain()
{
    if ( nullptr==spiceWdg ) return;
    QStringList fileNames = QFileDialog::getOpenFileNames(
                this, "Copy files to Guest", "~");
    spiceWdg->fileCopyAsync(fileNames);
}
void Spice_Viewer::cancelCopyFilesToVirtDomain()
{
    if ( nullptr==spiceWdg ) return;
    spiceWdg->cancelFileCopyAsync();
}
void Spice_Viewer::copyToClipboardFromVirtDomain()
{
    if ( nullptr!=spiceWdg )
        spiceWdg->copyClipboardDataFromGuest();
}
void Spice_Viewer::pasteClipboardToVirtDomain()
{
    if ( nullptr!=spiceWdg )
        spiceWdg->pasteClipboardDataToGuest();
}
void Spice_Viewer::fullScreenVirtDomain()
{
    fullScreenTriggered();
}

void Spice_Viewer::initGraphicWidget()
{
    spiceWdg = new QSpiceWidget(this);
    spiceWdg->setAttribute(Qt::WA_OpaquePaintEvent, true);
    scrolled = new QScrollArea(this);
    scrolled->setWidgetResizable(true);
    scrolled->setAlignment(Qt::AlignCenter);
    scrolled->setWidget(spiceWdg);
    setCentralWidget(scrolled);
    spiceWdg->setGuestAttr(domain, connName);
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
    connect(spiceWdg, SIGNAL(errMsg(const QString&)),
            this, SLOT(sendErrMsg(const QString&)));
    connect(spiceWdg, SIGNAL(clipboardsReleased(bool)),
            viewerToolBar, SLOT(setCopyPasteState(bool)));
    connect(spiceWdg, SIGNAL(boarderTouched()),
            this, SLOT(startAnimatedShow()));
    connect(spiceWdg, SIGNAL(mouseClickedInto()),
            this, SLOT(startAnimatedHide()));

    QSize around_size = getWidgetSizeAroundDisplay();
    //QTextStream s(stdout);
    //s<<"address: "<<addr<<":"<<port<<endl;
    QString _uri = QString("spice://%1:%2").arg(addr).arg(port);
    spiceWdg->connectToSpiceSource(_uri);
    spiceWdg->setNewSize(around_size.width(), around_size.height());
}

void Spice_Viewer::resizeViewer(const QSize &_size)
{
    QSize around_size = getWidgetSizeAroundDisplay();
    // will be showed when occured successful connect to VM
    if ( !isVisible() ) {
        this->show();
    };
    if ( _size+around_size==size() ) {
        return;
    };
    resize(_size+around_size);
}

void Spice_Viewer::fullScreenTriggered()
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

void Spice_Viewer::scaleScreenVirtDomain()
{
    spiceWdg->setScaledScreen(true);
}

void Spice_Viewer::resizeEvent(QResizeEvent *ev)
{
    QSize around_size = getWidgetSizeAroundDisplay();
    if ( nullptr!=spiceWdg ) {
        spiceWdg->updateSize(
                    ev->size().width()-around_size.width(),
                    ev->size().height()-around_size.height());
    };
}

QSize Spice_Viewer::getWidgetSizeAroundDisplay()
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
