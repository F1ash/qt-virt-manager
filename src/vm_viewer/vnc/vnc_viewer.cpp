#include "vnc_viewer.h"
#include <QApplication>
#include <QClipboard>
#include <QTextStream>

VNC_Viewer::VNC_Viewer(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr,
        QString          arg1,
        QString          arg2,
        QString          arg3) :
    VM_Viewer(parent, connPtrPtr, arg1, arg2, arg3)
{
    TYPE = "VNC";
    viewerToolBar->removeAction(viewerToolBar->copyFiles_Action);
    viewerToolBar->removeAction(viewerToolBar->copyToClipboard);
    viewerToolBar->removeAction(viewerToolBar->pasteClipboard);
    viewerToolBar->removeAction(viewerToolBar->stateWdg_Action);
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

/* public slots */
void VNC_Viewer::init()
{
    //QTextStream s(stdout);
    QString msg;
    if ( addr.isEmpty() || port==0 ) {
        viewerToolBar->setEnabled(false);
        msg = QString("In '<b>%1</b>':<br> Getting the address data is failed.")
                .arg(domain);
        sendErrMsg(msg);
        showErrorInfo(msg);
        startCloseProcess();
        //s << "failed" << endl;
    } else if ( addr.contains("127.0.0") &&
                !host.contains("localhost") &&
                !host.contains("localdomain") ) {
        viewerToolBar->setEnabled(false);
        msg = QString("In '<b>%1</b>':<br>\n\
Guest is on a remote host,\n\
but is only configured to allow\n\
local file descriptor connections.")
                .arg(domain);
        sendErrMsg(msg);
        showErrorInfo(msg);
        startCloseProcess();
        //s << "local graphics only on remote host" << endl;
    } else {
        actFullScreen = new QShortcut(
                    QKeySequence(tr("Shift+F11", "View|Full Screen")),
                    this);
        connect(actFullScreen, SIGNAL(activated()),
                SLOT(fullScreenTriggered()));
        //s << "remote or local with allow to graphics stream" << endl;
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
            connect(sshTunnelThread, SIGNAL(established(uint)),
                    this, SLOT(useSSHTunnel(uint)));
            connect(sshTunnelThread, SIGNAL(errMsg(QString)),
                    this, SLOT(sendErrMsg(QString)));
            sshTunnelThread->setData(_data);
            sshTunnelThread->start();
        //};
    };
}
void VNC_Viewer::reconnectToVirtDomain()
{
    if ( nullptr!=vncWdg ) {
        delete vncWdg;
        vncWdg = nullptr;
        // resizing to any,
        // because will need to init new display configuration
        //resize(getWidgetSizeAroundDisplay());
        initGraphicWidget();
        QSize around_size = getWidgetSizeAroundDisplay();
        //resize(around_size);
        if ( nullptr!=vncWdg ) {
            vncWdg->newViewSize(
                        size().width()-around_size.width(),
                        size().height()-around_size.height());
        };
    };
}
void VNC_Viewer::sendKeySeqToVirtDomain(Qt::Key key)
{
    if ( nullptr==vncWdg ) return;
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
void VNC_Viewer::getScreenshotFromVirtDomain()
{
    if ( nullptr==vncWdg ) return;
    QImage img = vncWdg->getScreenCapture();
    // WARNING: used %1%2%3[%4].snapshot template,
    // because filter will added to tail the template
    // after last dot.
    QString fileName = QFileDialog::getSaveFileName(
                this,
                "Save Image to",
                QString("%1%2%3[%4]_%5_%6.snapshot")
                    .arg(QDir::homePath())
                    .arg(QDir::separator())
                    .arg(domain).arg(connName)
                    .arg(QDate::currentDate().toString("dd.MM.yyyy"))
                    .arg(QTime::currentTime().toString()),
                "Images (*.png)");
    if ( !fileName.isNull() ) {
        img.save(fileName, "png");
    };
}
void VNC_Viewer::copyFilesToVirtDomain()
{
    if ( nullptr==vncWdg ) return;
    QStringList fileNames = QFileDialog::getOpenFileNames(
                this, "Copy files to Guest", "~");
    //vncWdg->fileCopyAsync(fileNames);
}
void VNC_Viewer::copyToClipboardFromVirtDomain()
{
    if ( nullptr==vncWdg ) return;
    //vncWdg->copyClipboardDataFromGuest();
}
void VNC_Viewer::pasteClipboardToVirtDomain()
{
    if ( nullptr==vncWdg ) return;
    const QString _text = QApplication::clipboard()->text(QClipboard::Clipboard);
    const QImage _image = QApplication::clipboard()->image(QClipboard::Clipboard);
    qDebug()<<"copy:"<<_text<<_image.isNull()<<";";
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
        } else if ( _format.toLower()=="png" ) {
            qDebug()<<"png";
            _frmt = VD_AGENT_CLIPBOARD_IMAGE_PNG;
        } else if ( _format.toLower()=="bmp" ) {
            _frmt = VD_AGENT_CLIPBOARD_IMAGE_BMP;
        } else if ( _format.toLower()=="jpg" ) {
            _frmt = VD_AGENT_CLIPBOARD_IMAGE_JPG;
        } else if ( _format.toLower()=="tiff" ) {
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
void VNC_Viewer::fullScreenVirtDomain()
{
    fullScreenTriggered();
}

/* private slots */
void VNC_Viewer::initGraphicWidget()
{
    vncWdg = new MachineView(this);
    setCentralWidget(vncWdg);
    connect(vncWdg, SIGNAL(Full_Size(int,int)),
            SLOT(resizeViewer(const int, const int)));
    //connect(vncWdg, SIGNAL(errMsg(const QString&)),
    //        this, SLOT(sendErrMsg(const QString&)));
    //connect(vncWdg, SIGNAL(clipboardsReleased(bool)),
    //        viewerToolBar, SLOT(setCopyPasteState(bool)));
    connect(vncWdg, SIGNAL(boarderTouched()),
            this, SLOT(startAnimatedShow()));
    connect(vncWdg, SIGNAL(mouseClickedInto()),
            this, SLOT(startAnimatedHide()));

    QSize around_size = getWidgetSizeAroundDisplay();
    QTextStream s(stdout);
    s<<"address: "<<addr<<":"<<port<<endl;
    vncWdg->Set_VNC_URL(addr, port);
    vncWdg->Set_Scaling(true);
    vncWdg->initView();
    vncWdg->newViewSize(around_size.width(), around_size.height());
}

void VNC_Viewer::useSSHTunnel(uint _port)
{
    port = _port;
    initGraphicWidget();
}

void VNC_Viewer::timerEvent(QTimerEvent *ev)
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

void VNC_Viewer::resizeViewer(const int h, const int w)
{
    QSize around_size = getWidgetSizeAroundDisplay();
    resize(QSize(h,w)+around_size);
}

void VNC_Viewer::fullScreenTriggered()
{
    if ( nullptr==vncWdg ) return;
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

void VNC_Viewer::scaleScreenVirtDomain()
{
    if ( nullptr==vncWdg ) return;
    vncWdg->Set_Scaling(true);
}

void VNC_Viewer::resizeEvent(QResizeEvent *ev)
{
    QSize around_size = getWidgetSizeAroundDisplay();
    if ( nullptr!=vncWdg ) {
        vncWdg->newViewSize(
                    ev->size().width()-around_size.width(),
                    ev->size().height()-around_size.height());
    };
}

QSize VNC_Viewer::getWidgetSizeAroundDisplay()
{
    int left, top, right, bottom, _width, _height;
    viewerToolBar->getContentsMargins(&left, &top, &right, &bottom);
    _width = left+right;
    _height = top +bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    _width += left+right;
    _height += top +bottom;
    QSize _size(_width, _height);
    return _size;
}
