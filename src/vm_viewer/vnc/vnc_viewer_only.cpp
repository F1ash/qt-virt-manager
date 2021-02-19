#include "vnc_viewer_only.h"
#include <QApplication>
#include <QClipboard>

VNC_Viewer_Only::VNC_Viewer_Only(
        QWidget         *parent,
        const QString    _url) :
    VM_Viewer_Only(parent, _url)
{
    viewerToolBar->removeAction(viewerToolBar->copyFiles_Action);
    viewerToolBar->removeAction(viewerToolBar->copyToClipboard);
    viewerToolBar->removeAction(viewerToolBar->pasteClipboard);
    viewerToolBar->removeAction(viewerToolBar->stateWdg_Action);
    init();
}

/* private slots */
void VNC_Viewer_Only::reconnectToVirtDomain()
{
    if ( Q_NULLPTR!=vncWdg ) {
        delete vncWdg;
        vncWdg = Q_NULLPTR;
        // resizing to any,
        // because will need to init new display configuration
        //resize(getWidgetSizeAroundDisplay());
        initGraphicWidget();
        QSize around_size = getWidgetSizeAroundDisplay();
        //resize(around_size);
        if ( Q_NULLPTR!=vncWdg ) {
            vncWdg->newViewSize(
                        size().width()-around_size.width(),
                        size().height()-around_size.height());
        };
    } else {
        initGraphicWidget();
    };
}
void VNC_Viewer_Only::disconnectFromVirtDomain()
{
    if ( vncWdg!=Q_NULLPTR ) {
        vncWdg->disconnectVNC();
    };
}
void VNC_Viewer_Only::sendKeySeqToVirtDomain(Qt::Key key)
{
    if ( Q_NULLPTR==vncWdg ) return;
    QKeyEvent *ev;
    switch (key) {
    case Qt::Key_Launch1:
        ev = new QKeyEvent(
                    QEvent::KeyPress,
                    XK_F1,
                    Qt::ControlModifier|Qt::AltModifier);
        break;
    case Qt::Key_Launch2:
        ev = new QKeyEvent(
                    QEvent::KeyPress,
                    XK_F2,
                    Qt::ControlModifier|Qt::AltModifier);
        break;
    case Qt::Key_Launch3:
        ev = new QKeyEvent(
                    QEvent::KeyPress,
                    XK_F3,
                    Qt::ControlModifier|Qt::AltModifier);
        break;
    case Qt::Key_Launch4:
        ev = new QKeyEvent(
                    QEvent::KeyPress,
                    XK_F4,
                    Qt::ControlModifier|Qt::AltModifier);
        break;
    case Qt::Key_Launch5:
        ev = new QKeyEvent(
                    QEvent::KeyPress,
                    XK_F5,
                    Qt::ControlModifier|Qt::AltModifier);
        break;
    case Qt::Key_Launch6:
        ev = new QKeyEvent(
                    QEvent::KeyPress,
                    XK_F6,
                    Qt::ControlModifier|Qt::AltModifier);
        break;
    case Qt::Key_Launch7:
        ev = new QKeyEvent(
                    QEvent::KeyPress,
                    XK_F7,
                    Qt::ControlModifier|Qt::AltModifier);
        break;
    case Qt::Key_Launch8:
        ev = new QKeyEvent(
                    QEvent::KeyPress,
                    XK_F8,
                    Qt::ControlModifier|Qt::AltModifier);
        break;
    case Qt::Key_LaunchB:
        ev = new QKeyEvent(
                    QEvent::KeyPress,
                    XK_BackSpace,
                    Qt::ControlModifier|Qt::AltModifier);
        break;
    case Qt::Key_LaunchD:
        ev = new QKeyEvent(
                    QEvent::KeyPress,
                    XK_Delete,
                    Qt::ControlModifier|Qt::AltModifier);
        break;
    default:
        return;
    };
    vncWdg->sendKey(ev);
}
void VNC_Viewer_Only::getScreenshotFromVirtDomain()
{
    if ( Q_NULLPTR==vncWdg ) return;
    QImage img = vncWdg->getScreenCapture();
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save Image to"),
                QString("%1%2_%3_%4.snapshot")
                    .arg(QDir::homePath())
                    .arg(QDir::separator())
                    .arg(QDate::currentDate().toString("dd.MM.yyyy"))
                    .arg(QTime::currentTime().toString()),
                "Images (*.png)");
    if ( !fileName.isNull() ) {
        img.save(fileName, "png");
    };
}
void VNC_Viewer_Only::copyFilesToVirtDomain()
{
    if ( Q_NULLPTR==vncWdg ) return;
    //QStringList fileNames = QFileDialog::getOpenFileNames(
    //            this, "Copy files to Guest", "~");
    //vncWdg->fileCopyAsync(fileNames);
}
void VNC_Viewer_Only::copyToClipboardFromVirtDomain()
{
    if ( Q_NULLPTR==vncWdg ) return;
    //vncWdg->copyToClipboardFromGuest();
}
void VNC_Viewer_Only::pasteClipboardToVirtDomain()
{
    if ( Q_NULLPTR==vncWdg ) return;
    const QString _text = QApplication::clipboard()->text(QClipboard::Clipboard);
    const QImage _image = QApplication::clipboard()->image(QClipboard::Clipboard);
    //qDebug()<<"copy:"<<_text<<_image.isNull()<<";";
    if ( !_text.isEmpty() ) {
        //vncWdg->sendClipboardDataToGuest(
        //            VD_AGENT_CLIPBOARD_UTF8_TEXT,
        //            (const uchar*)_text.toUtf8().data(),
        //            _text.size());
        vncWdg->pasteClipboardTextToGuest(_text);
    };
    if ( !_image.isNull() ) {
        /*
        QString _format = _text.split(".").last();
        qint32 _frmt;
        if ( _format.isEmpty() ) {
            QMessageBox::information(
                        this,
                        "INFO",
                        QString("Unknown image format:\n'%1'")
                        .arg(_text));
            return;
        } else if ( _format.toLower().compare("png")==0 ) {
            //qDebug()<<"png";
            _frmt = VD_AGENT_CLIPBOARD_IMAGE_PNG;
        } else if ( _format.toLower().compare("bmp")==0 ) {
            _frmt = VD_AGENT_CLIPBOARD_IMAGE_BMP;
        } else if ( _format.toLower().compare("jpg")==0 ) {
            _frmt = VD_AGENT_CLIPBOARD_IMAGE_JPG;
        } else if ( _format.toLower().compare("tiff")==0 ) {
            _frmt = VD_AGENT_CLIPBOARD_IMAGE_TIFF;
        } else {
            QMessageBox::information(
                        this,
                        "INFO",
                        "Unknown image format.");
            return;
        };
        */
        //vncWdg->sendClipboardDataToGuest(
        //            VD_AGENT_CLIPBOARD_IMAGE_PNG,
        //            _image.constBits(),
        //            _image.byteCount());
    };
}
void VNC_Viewer_Only::fullScreenVirtDomain()
{
    fullScreenTriggered();
}

void VNC_Viewer_Only::initGraphicWidget()
{
    vncWdg = new MachineView(this);
    setCentralWidget(vncWdg);
    connect(vncWdg, SIGNAL(Full_Size(int,int)),
            SLOT(resizeViewer(const int, const int)));
    //connect(vncWdg, SIGNAL(errMsg(const QString&)),
    //        this, SLOT(sendErrMsg(const QString&)));
    //connect(vncWdg, SIGNAL(copyPasteStateChanged(bool)),
    //        viewerToolBar, SLOT(setCopyPasteState(bool)));
    connect(vncWdg, SIGNAL(boarderTouched()),
            this, SLOT(startAnimatedShow()));
    connect(vncWdg, SIGNAL(mouseClickedInto()),
            this, SLOT(startAnimatedHide()));
    connect(vncWdg, SIGNAL(Connected()),
            this, SLOT(connectedToHost()));
    connect(vncWdg, SIGNAL(CantConnect()),
            this, SLOT(cantConnect()));

    //QString _url = url.split("://").last();
    //QStringList _urlParams = _url.split(":");
    //addr = _urlParams.first();
    //port = _urlParams.last().toInt();
    QSize around_size = getWidgetSizeAroundDisplay();
    //qDebug()<<"address:"<<addr<<port;
    vncWdg->Set_VNC_URL(addr, port.toInt());
    vncWdg->Set_Scaling(true);
    vncWdg->initView();
    vncWdg->newViewSize(around_size.width(), around_size.height());
}

void VNC_Viewer_Only::resizeViewer(const int h, const int w)
{
    QSize around_size = getWidgetSizeAroundDisplay();
    // will be showed when occured successful connection to VM
    if ( !isVisible() ) {
        this->show();
    };
    resize(QSize(h,w)+around_size);
}

void VNC_Viewer_Only::fullScreenTriggered()
{
    if (isFullScreen()) {
        setWindowState(Qt::WindowNoState);
        vncWdg->Set_Fullscreen(false);
    } else {
        setWindowState(Qt::WindowFullScreen);
        QPalette p;
        p.setColor( QPalette::Background, QColor(22,22,22) );
        setPalette( p );
        vncWdg->Set_Fullscreen(true);
    };
    //vncWdg->Set_Scaling(vncWdg->isFullScreen());
    startAnimatedHide();
}

void VNC_Viewer_Only::scaleScreenVirtDomain()
{
    vncWdg->Set_Scaling(true);
}

void VNC_Viewer_Only::resizeEvent(QResizeEvent *ev)
{
    QSize around_size = getWidgetSizeAroundDisplay();
    if ( Q_NULLPTR!=vncWdg ) {
        vncWdg->newViewSize(
                    ev->size().width()-around_size.width(),
                    ev->size().height()-around_size.height());
    };
}

QSize VNC_Viewer_Only::getWidgetSizeAroundDisplay()
{
    int left, top, right, bottom, _width, _height;
    QMargins _m = viewerToolBar->contentsMargins();
    left    = _m.left();
    top     = _m.top();
    right   = _m.right();
    bottom  = _m.bottom();
    _width = left+right;
    _height = top +bottom;
    _m = contentsMargins();
    left    = _m.left();
    top     = _m.top();
    right   = _m.right();
    bottom  = _m.bottom();
    _width += left+right;
    _height += top +bottom;
    QSize _size(_width, _height);
    return _size;
}

void VNC_Viewer_Only::connectedToHost()
{
    QSize around_size = getWidgetSizeAroundDisplay();
    vncWdg->newViewSize(around_size.width(), around_size.height());
    setWindowTitle(QString(tr("Qt Remote Viewer -- %1")).arg(url));
}

void VNC_Viewer_Only::cantConnect()
{
    showErrorInfo("");
}
