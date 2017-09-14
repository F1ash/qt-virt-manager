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
Spice_Viewer::~Spice_Viewer()
{
    if ( spiceWdg!=nullptr ) {
        spiceWdg->disconnectFromSpiceSource();
    };
    if ( sshTunnelThread!=nullptr ) {
        sshTunnelThread->deleteLater();
    };
}

/* public slots */
void Spice_Viewer::init()
{
    QString msg;
    if ( addr.isEmpty() || port==0 ) {
        viewerToolBar->setEnabled(false);
        msg = QString("In '<b>%1</b>':<br> Getting the address data is failed.")
                .arg(domain);
        sendErrMsg(msg);
        showErrorInfo(msg);
        startCloseProcess();
//    } else if ( addr.contains("127.0.0") &&
//                !host.contains("localhost") &&
//                !host.contains("localdomain") ) {
//        viewerToolBar->setEnabled(false);
//        msg = QString("In '<b>%1</b>':<br>\n\
//Guest is on a remote host,\n\
//but is only configured to allow\n\
//local file descriptor connections.")
//                .arg(domain);
//        sendErrMsg(msg);
//        showErrorInfo(msg);
//        startCloseProcess();
    } else {
        actFullScreen = new QShortcut(
                    QKeySequence(tr("Shift+F11", "View|Full Screen")),
                    this);
        connect(actFullScreen, SIGNAL(activated()),
                SLOT(fullScreenTriggered()));
        //if ( host.contains("localhost") || host.contains("localdomain") ) {
        //    // local VM, graphic is allow
        //    emit initGraphic();
        //} else {
            // need ssh tunnel
            QVariantMap _data;
            _data.insert("User", user);
            QStringList _remoteAddr = host.split(":");
            _data.insert("RemoteHost", _remoteAddr.first());
            if ( _remoteAddr.count()==2 ) {
                _data.insert("RemotePort", _remoteAddr.last());
            } else {
                _data.insert("RemotePort", "22"); // default SSH service TCP port
            };
            _data.insert("GraphicsAddr", addr);
            _data.insert("GraphicsPort", port);
            sshTunnelThread = new SSH_Tunnel(this);
            connect(sshTunnelThread, SIGNAL(established(quint16)),
                    this, SLOT(useSSHTunnel(quint16)));
            connect(sshTunnelThread, SIGNAL(errMsg(QString)),
                    this, SLOT(sendErrMsg(QString)));
            sshTunnelThread->setData(_data);
            sshTunnelThread->start();
            // crazy solution >>>
            connectBtn = new QPushButton(this);
            connectBtn->setText("Connect to VM");
            setCentralWidget(connectBtn);
            connect(connectBtn, SIGNAL(released()),
                    this, SLOT(initGraphicWidget()));
            // <<< crazy solution
        //};
    };
}
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

/* private slots */
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
    QTextStream s(stdout);
    s<<"address: "<<addr<<":"<<port<<endl;
    QString _uri = QString("spice://%1:%2").arg(addr).arg(port);
    spiceWdg->connectToSpiceSource(_uri);
    spiceWdg->setNewSize(around_size.width(), around_size.height());
}

void Spice_Viewer::useSSHTunnel(quint16 _port)
{
    addr = "127.0.0.1";
    port = _port;
    //initGraphicWidget();
}

void Spice_Viewer::timerEvent(QTimerEvent *ev)
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
    }
}

void Spice_Viewer::resizeViewer(const QSize &_size)
{
    QSize around_size = getWidgetSizeAroundDisplay();
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
