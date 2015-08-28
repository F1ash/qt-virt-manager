#include "vm_viewer.h"

HlpThread::HlpThread(
        QObject *parent, virConnect *_conn) :
    QThread(parent), currWorkConnection(_conn)
{

}
void HlpThread::run()
{

}

VM_Viewer::VM_Viewer(
        QWidget *parent, virConnect *conn, QString arg1, QString arg2) :
    QMainWindow(parent), jobConnect(conn), connName(arg1), domain(arg2)
{
    qRegisterMetaType<QString>("QString&");
    setMinimumSize(100, 100);
    setWindowTitle(QString("<%1> Virtual Machine in [ %2 ] connection")
                   .arg(domain).arg(connName));
    VM_State = true;
    viewerToolBar = new ViewerToolBar(this);
    viewerToolBar->setVisible(false);
    addToolBar(Qt::TopToolBarArea, viewerToolBar);
    connect(viewerToolBar, SIGNAL(execMethod(const QStringList&)),
            this, SLOT(resendExecMethod(const QStringList&)));
    closeProcess = new QProgressBar(this);
    closeProcess->setRange(0, TIMEOUT);
    statusBar()->addPermanentWidget(closeProcess);
    statusBar()->hide();
    hlpThread = new HlpThread(this, jobConnect);
    connect(hlpThread, SIGNAL(result(QString&)),
            this, SLOT(init(QString&)));
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
    VM_State = false;
    //qDebug()<<"VM_Viewer destroyed";
}

/* public slots */
void VM_Viewer::init(QString&)
{

}
bool VM_Viewer::isActive() const
{
    return VM_State;
}
virDomain* VM_Viewer::getDomainPtr() const
{
    virDomainPtr ret = NULL;
    if ( NULL!=jobConnect )
        ret = virDomainLookupByName(jobConnect, domain.toUtf8().data());
    return ret;
}

void VM_Viewer::closeEvent(QCloseEvent *ev)
{
    if ( ev->type()==QEvent::Close ) {
        VM_State = false;
        QString key = objectName();
        QString msg = QString("'<b>%1</b>' VM viewer closed.")
                .arg(domain);
        sendErrMsg(msg);
        ev->accept();
        emit finished(key);
    }
}
void VM_Viewer::sendErrMsg(QString &msg)
{
    sendErrMsg(msg, 0);
}
void VM_Viewer::sendErrMsg(QString &msg, uint _number)
{
    Q_UNUSED(_number);
    QString time = QTime::currentTime().toString();
    QString title = QString("Connection '%1'").arg(connName);
    QString errMsg = QString("<b>%1 %2:</b><br><font color='blue'><b>EVENT</b></font>: %3")
            .arg(time).arg(title).arg(msg);
    emit errorMsg(errMsg);
}

void VM_Viewer::sendConnErrors()
{
    virtErrors = virConnGetLastError(jobConnect);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        QString msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void VM_Viewer::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        QString msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg );
    };
    virResetLastError();
}
void VM_Viewer::resendExecMethod(const QStringList &method)
{
    QStringList args;
    args.append(domain);
    TASK task;
    task.type = "domain";
    task.sourceConn = jobConnect;
    task.srcConName = connName;
    task.object     = domain;
    if        ( method.first()=="startVirtDomain" ) {
        task.method     = method.first();
        task.action     = START_ENTITY;
        emit addNewTask(task);
    } else if ( method.first()=="pauseVirtDomain" ) {
        task.method     = method.first();
        task.action     = PAUSE_ENTITY;
        task.args.state = VM_State ? 1 : 0;
        emit addNewTask(task);
    } else if ( method.first()=="destroyVirtDomain" ) {
        task.method     = method.first();
        task.action     = DESTROY_ENTITY;
        emit addNewTask(task);
    } else if ( method.first()=="resetVirtDomain" ) {
        task.method     = method.first();
        task.action     = RESET_ENTITY;
        emit addNewTask(task);
    } else if ( method.first()=="shutdownVirtDomain" ) {
        task.method     = method.first();
        task.action     = SHUTDOWN_ENTITY;
        emit addNewTask(task);
    } else if ( method.first()=="saveVirtDomain" ) {
        QString to = QFileDialog::getSaveFileName(this, "Save to", "~");
        if ( !to.isEmpty() ) {
            task.method     = method.first();
            task.action     = SAVE_ENTITY;
            task.args.path  = to;
            task.args.state = VM_State ? 1 : 0;
            emit addNewTask(task);
        };
    } else if ( method.first()=="restoreVirtDomain" ) {
        QString from = QFileDialog::getOpenFileName(this, "Restore from", "~");
        if ( !from.isEmpty() ) {
            task.method     = method.first();
            task.action     = RESTORE_ENTITY;
            task.args.path  = from;
            emit addNewTask(task);
        };
    } else if ( method.first()=="createVirtDomainSnapshot" ) {
        //qDebug()<<"createVirtDomainSnapshot";
        CreateSnapshotDialog *_dialog =
                new CreateSnapshotDialog(
                    this, domain, true, jobConnect);
        connect(_dialog, SIGNAL(errMsg(QString&)),
                this, SLOT(sendErrMsg(QString&)));
        int exitCode = _dialog->exec();
        if ( exitCode ) {
            task.action      = CREATE_DOMAIN_SNAPSHOT;
            task.method      = "createVirtDomainSnapshot";
            task.args.object = _dialog->getSnapshotXMLDesc();
            task.args.sign   = _dialog->getSnapshotFlags();
            emit addNewTask(task);
        };
        disconnect(_dialog, SIGNAL(errMsg(QString&)),
                   this, SLOT(sendErrMsg(QString&)));
        _dialog->deleteLater();
    } else if ( method.first()=="moreSnapshotActions" ) {
        //qDebug()<<"moreSnapshotActions";
        SnapshotActionDialog *_dialog =
               new SnapshotActionDialog(this, jobConnect, domain);
        int exitCode = _dialog->exec();
        if ( exitCode ) {
            QStringList params = _dialog->getParameters();
            task.action      = static_cast<Actions>(exitCode);
            task.method      = params.first();
            params.removeFirst();
            task.args.object = params.first();
            task.args.sign   = _dialog->getSnapshotFlags();
            emit addNewTask(task);
        };
        _dialog->deleteLater();
    } else if ( method.first()=="reconnectToVirtDomain" ) {
        reconnectToDomain();
    } else if ( method.first()=="sendKeySeqToVirtDomain" ) {
        sendKeySeqToDomain((Qt::Key)method.last().toInt());
    };

}
void VM_Viewer::startCloseProcess()
{
    //qDebug()<<"startCloseProcess";
    killTimerId = startTimer(PERIOD);
    statusBar()->show();
    //qDebug()<<killTimerId<<"killTimer";
}
void VM_Viewer::reconnectToDomain()
{

}
void VM_Viewer::sendKeySeqToDomain(Qt::Key key)
{
    Q_UNUSED(key);
}
void VM_Viewer::showErrorInfo(QString &_msg)
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
