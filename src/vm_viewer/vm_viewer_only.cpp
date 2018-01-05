#include "vm_viewer_only.h"

VM_Viewer_Only::VM_Viewer_Only(
        QWidget          *parent,
        const QString     _url) :
    QMainWindow(parent), url(_url)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(100, 100);
    setContentsMargins(0,0,0,0);
    setWindowTitle("Qt Remote Viewer");
    QIcon::setThemeName("QtRemoteViewer");
    setWindowIcon(QIcon::fromTheme("remote-desktop-viewer"));

    viewerToolBar = new ViewerToolBar(this);
    viewerToolBar->hide();
    addToolBar(Qt::TopToolBarArea, viewerToolBar);
    connect(viewerToolBar, SIGNAL(execMethod(const Act_Param&)),
            this, SLOT(resendExecMethod(const Act_Param&)));

    toolBarPoint = QPoint(0,0);
    animatedShowToolBar = new QPropertyAnimation(
                viewerToolBar, "pos");
    animatedShowToolBar->setDuration(333);
    animatedHideToolBar = new QPropertyAnimation(
                viewerToolBar, "pos");
    animatedHideToolBar->setDuration(333);
    connect(animatedShowToolBar, SIGNAL(finished()),
            this, SLOT(toolBarShowed()));
    connect(animatedHideToolBar, SIGNAL(finished()),
            this, SLOT(hideToolBar()));
    connect(viewerToolBar, SIGNAL(positionChanged(const QPoint&)),
            this, SLOT(setNewPosition(const QPoint&)));
    connect(this, SIGNAL(initGraphic()),
            this, SLOT(initGraphicWidget()));

    viewerToolBar->removeAction(viewerToolBar->pause_Action);
    viewerToolBar->removeAction(viewerToolBar->destroy_Action);
    viewerToolBar->removeAction(viewerToolBar->snapshot_Action);
    viewerToolBar->removeAction(viewerToolBar->sep1);
    viewerToolBar->removeAction(viewerToolBar->sep2);
    parseURL();
}
VM_Viewer_Only::~VM_Viewer_Only()
{
    //qDebug()<<"VM_Viewer_Only destroy:";
    if ( timerId>0 ) {
        killTimer(timerId);
        timerId = 0;
    };
    if ( killTimerId>0 ) {
        killTimer(killTimerId);
        killTimerId = 0;
    };
    if ( reinitTimerId>0 ) {
        killTimer(reinitTimerId);
        reinitTimerId = 0;
    };
    disconnectFromVirtDomain();
    if ( sshTunnelThread!=nullptr ) {
        sshTunnelThread->stop();
    };
    //qDebug()<<"VM_Viewer_Only destroyed";
}
void VM_Viewer_Only::init()
{
    QString msg;
    if ( !socket.isEmpty() ) {
        actFullScreen = new QShortcut(
                    QKeySequence(tr("Shift+F11", "View|Full Screen")),
                    this);
        connect(actFullScreen, SIGNAL(activated()),
                SLOT(fullScreenTriggered()));
        // uses socket for domain graphics;
        // dirty (for local VMs) and lazy hack with ssh tunnel
        QVariantMap _data;
        _data.insert("User", "root");
        QStringList _remoteAddr = host.split(":", QString::SkipEmptyParts);
        if ( _remoteAddr.count()>1 ) {
            _data.insert("RemotePort", _remoteAddr.last());
            _remoteAddr.removeLast();
        } else {
            _data.insert("RemotePort", "22"); // default SSH service TCP port
        };
        if ( _remoteAddr.count()>1 ) {
            _data.insert("RemoteHost", _remoteAddr.join(":"));
        } else {
            _data.insert("RemoteHost", _remoteAddr.first());
        };
        _data.insert("GraphicsAddr", addr);
        _data.insert("GraphicsSock", socket);
        sshTunnelThread = new SSH_Tunnel();
        connect(sshTunnelThread, SIGNAL(established(quint16)),
                this, SLOT(useSSHTunnel(quint16)));
        connect(sshTunnelThread, SIGNAL(finished()),
                this, SLOT(sshThreadFinished()));
        sshTunnelThread->setData(_data);
        sshTunnelThread->start();
    } else if ( addr.isEmpty() || port==0 ) {
        viewerToolBar->setEnabled(false);
        msg = QString("In '<b>%1</b>':<br> Getting the address data is failed.")
                .arg(url);
        showErrorInfo(msg);
        startCloseProcess();
        //s << "failed" << endl;
    } else {
        actFullScreen = new QShortcut(
                    QKeySequence(tr("Shift+F11", "View|Full Screen")),
                    this);
        connect(actFullScreen, SIGNAL(activated()),
                SLOT(fullScreenTriggered()));
        if ( !transport.contains("ssh", Qt::CaseInsensitive) ) {
            emit initGraphic();
        } else {
            // need ssh tunnel
            QVariantMap _data;
            _data.insert("User", user);
            QStringList _remoteAddr = host.split(":", QString::SkipEmptyParts);
            if ( _remoteAddr.count()>1 ) {
                _data.insert("RemotePort", _remoteAddr.last());
                _remoteAddr.removeLast();
            } else {
                _data.insert("RemotePort", "22"); // default SSH service TCP port
            };
            if ( _remoteAddr.count()>1 ) {
                _data.insert("RemoteHost", _remoteAddr.join(":"));
            } else {
                _data.insert("RemoteHost", _remoteAddr.first());
            };
            _data.insert("GraphicsAddr", addr);
            _data.insert("GraphicsPort", port);
            sshTunnelThread = new SSH_Tunnel();
            connect(sshTunnelThread, SIGNAL(established(quint16)),
                    this, SLOT(useSSHTunnel(quint16)));
            connect(sshTunnelThread, SIGNAL(finished()),
                    this, SLOT(sshThreadFinished()));
            sshTunnelThread->setData(_data);
            sshTunnelThread->start();
        };
    };
}
void VM_Viewer_Only::parseURL()
{
    if ( url.split("://", QString::SkipEmptyParts).count()>1 ) {
        QStringList parts1 = url.split("://").at(1)
                .split("/");
        host    = parts1.first();
        if ( host.isEmpty() ) {
            host.append("localhost.localdomain");
        };
        QStringList _parts = url.split("/?");
        if ( _parts.count()>1 ) {
            // address has extra parameters
            QVariantMap _data;
            foreach (QString _str, _parts.at(1).split("&", QString::SkipEmptyParts)) {
                QStringList _params = _str.split("=");
                _data.insert(_params.first(), _params.last());
            };
            transport   = _data.value("transport").toString();
            user        = _data.value("user").toString();
            addr        = _data.value("addr").toString();
            port        = _data.value("port").toInt();
            socket      = _data.value("socket").toString();
        } else {
            // address has usual parameters only
            QStringList parts2 = host.split(":", QString::SkipEmptyParts);
            if ( parts2.count()>1 ) {
                port = parts2.last().toInt();
                parts2.removeLast();
                if ( parts2.count()>1 ) {
                    addr = parts2.join(":");
                } else {
                    addr = parts2.first();
                };
            } else {
                // Getting the address data is failed
            };
        };
    } else {
        // Getting the address data is failed
    };
}

/* public slots */
void VM_Viewer_Only::initGraphicWidget()
{

}
void VM_Viewer_Only::timerEvent(QTimerEvent *ev)
{
    if ( ev->timerId()==killTimerId ) {
        killCounter++;
        viewerToolBar->vm_stateWdg->setCloseProcessValue(killCounter*PERIOD*6);
        if ( TIMEOUT<killCounter*PERIOD*6 ) {
            killTimer(killTimerId);
            killTimerId = 0;
            killCounter = 0;
            close();
        };
    } else if ( ev->timerId()==toolBarTimerId ) {
        startAnimatedHide();
    } else if ( ev->timerId()==reinitTimerId ) {
        reinitCounter++;
        if ( !isVisible() && reinitCounter<30 ) {
            reconnectToVirtDomain();
        } else {
            killTimer(reinitTimerId);
            reinitTimerId = 0;
            if ( reinitCounter>=30 ) {
                viewerToolBar->setEnabled(false);
                QString msg =
                        QString("In '<b>%1</b>':<br> Open the address is failed.")
                        .arg(url);
                showErrorInfo(msg);
                startCloseProcess();
            };
            reinitCounter = 0;
        };
    };
}
void VM_Viewer_Only::useSSHTunnel(quint16 _port)
{
    addr = "127.0.0.1";
    port = _port;
    // start timer for [re]connect to VM,
    // when connection is successful,
    // then will be resizeing occured and
    // viewer will be showed.
    // See for resizeViewer method.
    reinitTimerId = startTimer(1000);
}
void VM_Viewer_Only::resendExecMethod(const Act_Param &params)
{
    if ( params.method==Methods::reconnectToVirtDomain ) {
        reconnectToVirtDomain();
    } else if ( params.method==Methods::sendKeySeqToVirtDomain ) {
        sendKeySeqToVirtDomain((Qt::Key)params.path.toInt());
    } else if ( params.method==Methods::getScreenshotFromVirtDomain ) {
        getScreenshotFromVirtDomain();
    } else if ( params.method==Methods::copyFilesToVirtDomain ) {
        copyFilesToVirtDomain();
    } else if ( params.method==Methods::cancelCopyFilesToVirtDomain ) {
        cancelCopyFilesToVirtDomain();
    } else if ( params.method==Methods::copyToClipboardFromVirtDomain ) {
        copyToClipboardFromVirtDomain();
    } else if ( params.method==Methods::pasteClipboardToVirtDomain ) {
        pasteClipboardToVirtDomain();
    } else if ( params.method==Methods::fullScreenVirtDomain ) {
        fullScreenVirtDomain();
    } else if ( params.method==Methods::scaleScreenVirtDomain ) {
        scaleScreenVirtDomain();
    };

}
void VM_Viewer_Only::startCloseProcess()
{
    //qDebug()<<"startCloseProcess";
    if ( killTimerId==0 ) {
        killTimerId = startTimer(PERIOD);
        if ( nullptr!=statusBar() ) statusBar()->show();
    };
    //qDebug()<<killTimerId<<"killTimer";
}
void VM_Viewer_Only::reconnectToVirtDomain()
{

}
void VM_Viewer_Only::disconnectFromVirtDomain()
{

}
void VM_Viewer_Only::sendKeySeqToVirtDomain(Qt::Key key)
{
    Q_UNUSED(key);
}
void VM_Viewer_Only::getScreenshotFromVirtDomain()
{

}
void VM_Viewer_Only::copyFilesToVirtDomain()
{

}
void VM_Viewer_Only::cancelCopyFilesToVirtDomain()
{

}
void VM_Viewer_Only::copyToClipboardFromVirtDomain()
{

}
void VM_Viewer_Only::pasteClipboardToVirtDomain()
{

}
void VM_Viewer_Only::fullScreenVirtDomain()
{

}
void VM_Viewer_Only::scaleScreenVirtDomain()
{

}
void VM_Viewer_Only::showErrorInfo(const QString &_msg)
{
    QIcon _icon = QIcon::fromTheme("face-sad");
    icon = new QLabel(this);
    icon->setPixmap(_icon.pixmap(256));
    msg = new QLabel(this);
    if ( _msg.isEmpty() ) {
        msg->setText(QString("Can't connect to host:\n%1").arg(url));
    } else {
        msg->setText(_msg);
    };
    infoLayout = new QVBoxLayout();
    infoLayout->addWidget(icon, 0, Qt::AlignHCenter);
    infoLayout->addWidget(msg, 0, Qt::AlignHCenter);
    infoLayout->addStretch(-1);
    info = new QWidget(this);
    info->setLayout(infoLayout);
    setCentralWidget(info);
    if ( !isVisible() ) {
        this->show();
    };
}
void VM_Viewer_Only::startAnimatedShow()
{
    if ( viewerToolBar->isVisible() ) return;
    viewerToolBar->blockSignals(true);
    viewerToolBar->setWindowFlags(
                Qt::Window | Qt::FramelessWindowHint);
    viewerToolBar->show();
    if ( toolBarPoint.x()==0 && toolBarPoint.y()==0 ) {
        toolBarPoint =
                QPoint(
                    (this->size().width()
                     -viewerToolBar->size().width()
                     )/2, 0);
    };
    animatedShowToolBar->setStartValue(
                mapToGlobal(QPoint(0,0)));
    animatedShowToolBar->setEndValue(
                mapToGlobal(toolBarPoint));
    animatedShowToolBar->start();
    if ( toolBarTimerId==0 )
        toolBarTimerId = startTimer(15000);
}
void VM_Viewer_Only::startAnimatedHide()
{
    if ( toolBarTimerId>0 ) {
        animatedHideToolBar->setStartValue(
                    viewerToolBar->pos());
        animatedHideToolBar->setEndValue(
                    mapToGlobal(QPoint(0,0)));
        viewerToolBar->blockSignals(true);
        animatedHideToolBar->start();
    };
}

/* private slots */
void VM_Viewer_Only::toolBarShowed()
{
    viewerToolBar->blockSignals(false);
}
void VM_Viewer_Only::hideToolBar()
{
    killTimer(toolBarTimerId);
    toolBarTimerId = 0;
    viewerToolBar->hide();
    viewerToolBar->blockSignals(false);
}
void VM_Viewer_Only::setNewPosition(const QPoint &_pos)
{
    toolBarPoint = mapFromParent(_pos);
}
void VM_Viewer_Only::sshThreadFinished()
{
    showErrorInfo("SSH tunnel is destroyed.");
}
