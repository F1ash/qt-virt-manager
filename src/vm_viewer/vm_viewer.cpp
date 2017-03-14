#include "vm_viewer.h"

VM_Viewer::VM_Viewer(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr,
        QString          arg1,
        QString          arg2) :
    QMainWindow(parent),
    ptr_ConnPtr(connPtrPtr),
    connName(arg1),
    domain(arg2)
{
    qRegisterMetaType<QString>("QString&");
    setWindowTitle(QString("<%1> Virtual Machine in [ %2 ] connection")
                   .arg(domain).arg(connName));
    setWindowIcon(QIcon::fromTheme("virtual-engineering"));
    setMinimumSize(100, 100);
    setContentsMargins(0,0,0,0);
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
}
VM_Viewer::~VM_Viewer()
{
    //qDebug()<<"VM_Viewer destroy:";
    if ( timerId>0 ) {
        killTimer(timerId);
        timerId = 0;
    };
    if ( killTimerId>0 ) {
        killTimer(killTimerId);
        killTimerId = 0;
    };
    //qDebug()<<"VM_Viewer destroyed";
}

/* public slots */
void VM_Viewer::init()
{

}
void VM_Viewer::closeEvent(QCloseEvent *ev)
{
    if ( ev->type()==QEvent::Close ) {
        QString key = objectName();
        QString msg = QString("'<b>%1</b>' VM viewer closed.")
                .arg(domain);
        sendErrMsg(msg);
        emit finished(key);
    }
}
void VM_Viewer::sendErrMsg(const QString &msg)
{
    sendErrMsg(msg, 0);
}
void VM_Viewer::sendErrMsg(const QString &msg, const uint _number)
{
    Q_UNUSED(_number);
    QString time = QTime::currentTime().toString();
    QString title = QString("Connection '%1'").arg(connName);
    QString errMsg = QString(
    "<b>%1 %2:</b><br><font color='blue'><b>EVENT</b></font>: %3")
            .arg(time).arg(title).arg(msg);
    emit errorMsg(errMsg);
}

void VM_Viewer::sendConnErrors()
{
    virtErrors = (nullptr!=ptr_ConnPtr && *ptr_ConnPtr)?
                virConnGetLastError(*ptr_ConnPtr):nullptr;
    if ( virtErrors!=nullptr && virtErrors->code>0 ) {
        QString msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void VM_Viewer::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=nullptr && virtErrors->code>0 ) {
        QString msg = QString("VirtError(%1) : %2")
                .arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg );
    };
    virResetLastError();
}
void VM_Viewer::resendExecMethod(const Act_Param &params)
{
    QStringList args;
    args.append(domain);
    TASK task;
    task.type       = VIRT_ENTITY::VIRT_DOMAIN;
    task.srcConnPtr = ptr_ConnPtr;
    task.srcConName = connName;
    task.object     = domain;
    task.method     = params.method;
    if        ( params.method==Methods::startEntity ) {
        task.action     = Actions::START_ENTITY;
        emit addNewTask(&task);
    } else if ( params.method==Methods::pauseEntity ) {
        task.action     = Actions::PAUSE_ENTITY;
        emit addNewTask(&task);
    } else if ( params.method==Methods::destroyEntity ) {
        task.action     = Actions::DESTROY_ENTITY;
        emit addNewTask(&task);
    } else if ( params.method==Methods::resetVirtDomain ) {
        task.action     = Actions::RESET_ENTITY;
        emit addNewTask(&task);
    } else if ( params.method==Methods::shutdownVirtDomain ) {
        task.action     = Actions::SHUTDOWN_ENTITY;
        emit addNewTask(&task);
    } else if ( params.method==Methods::saveVirtDomain ) {
        QString to = QFileDialog::getSaveFileName(this, "Save to", "~");
        if ( !to.isEmpty() ) {
            task.action     = Actions::SAVE_ENTITY;
            task.args.path  = to;
            emit addNewTask(&task);
        };
    } else if ( params.method==Methods::restoreVirtDomain ) {
        QString from =
                QFileDialog::getOpenFileName(
                    this, "Restore from", "~");
        if ( !from.isEmpty() ) {
            task.action     = Actions::RESTORE_ENTITY;
            task.args.path  = from;
            emit addNewTask(&task);
        };
    } else if ( params.method==Methods::createVirtDomainSnapshot ) {
        //qDebug()<<"Methods::createVirtDomainSnapshot";
        CreateSnapshotDialog *_dialog =
                new CreateSnapshotDialog(
                    this, domain, connName, true, ptr_ConnPtr);
        connect(_dialog, SIGNAL(errMsg(const QString&)),
                this, SLOT(sendErrMsg(const QString&)));
        int exitCode = _dialog->exec();
        disconnect(_dialog, SIGNAL(errMsg(const QString&)),
                   this, SLOT(sendErrMsg(const QString&)));
        _dialog->deleteLater();
        if ( exitCode ) {
            task.action      = Actions::CREATE_DOMAIN_SNAPSHOT;
            task.args.object = _dialog->getSnapshotXMLDesc();
            task.args.sign   = _dialog->getSnapshotFlags();
            emit addNewTask(&task);
        };
    } else if ( params.method==Methods::moreSnapshotActions ) {
        //qDebug()<<"Methods::moreSnapshotActions";
        SnapshotActionDialog *_dialog =
               new SnapshotActionDialog(
                    this, ptr_ConnPtr, domain, connName);
        int exitCode = _dialog->exec();
        _dialog->deleteLater();
        if ( exitCode ) {
            Act_Param params = _dialog->getParameters();
            task.action      = params.act;
            Methods method;
            if ( params.act==Actions::REVERT_TO_DOMAIN_SNAPSHOT ) {
                method = Methods::revertVirtDomainSnapshot;
            } else if ( params.act==Actions::DELETE_DOMAIN_SNAPSHOT ) {
                method = Methods::deleteVirtDomainSnapshot;
            };
            task.method      = method;
            task.args.object = params.path;
            task.args.sign   = _dialog->getSnapshotFlags();
            emit addNewTask(&task);
        };
    } else if ( params.method==Methods::reconnectToVirtDomain ) {
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
void VM_Viewer::startCloseProcess()
{
    //qDebug()<<"startCloseProcess";
    if ( killTimerId==0 ) {
        killTimerId = startTimer(PERIOD);
        if ( nullptr!=statusBar() ) statusBar()->show();
    };
    //qDebug()<<killTimerId<<"killTimer";
}
void VM_Viewer::reconnectToVirtDomain()
{

}
void VM_Viewer::sendKeySeqToVirtDomain(Qt::Key key)
{
    Q_UNUSED(key);
}
void VM_Viewer::getScreenshotFromVirtDomain()
{

}
void VM_Viewer::copyFilesToVirtDomain()
{

}
void VM_Viewer::cancelCopyFilesToVirtDomain()
{

}
void VM_Viewer::copyToClipboardFromVirtDomain()
{

}
void VM_Viewer::pasteClipboardToVirtDomain()
{

}
void VM_Viewer::fullScreenVirtDomain()
{

}
void VM_Viewer::scaleScreenVirtDomain()
{

}
void VM_Viewer::showErrorInfo(const QString &_msg)
{
    QIcon _icon = QIcon::fromTheme("face-sad");
    icon = new QLabel(this);
    icon->setPixmap(_icon.pixmap(256));
    msg = new QLabel(this);
    msg->setText(_msg);
    infoLayout = new QVBoxLayout();
    infoLayout->addWidget(icon, 0, Qt::AlignHCenter);
    infoLayout->addWidget(msg);
    infoLayout->addStretch(-1);
    info = new QWidget(this);
    info->setLayout(infoLayout);
    setCentralWidget(info);
}
void VM_Viewer::startAnimatedShow()
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
void VM_Viewer::startAnimatedHide()
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
void VM_Viewer::toolBarShowed()
{
    viewerToolBar->blockSignals(false);
}
void VM_Viewer::hideToolBar()
{
    killTimer(toolBarTimerId);
    toolBarTimerId = 0;
    viewerToolBar->hide();
    viewerToolBar->blockSignals(false);
}
void VM_Viewer::setNewPosition(const QPoint &_pos)
{
    toolBarPoint = mapFromParent(_pos);
}
