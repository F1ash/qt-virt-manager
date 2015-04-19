#include "vm_viewer.h"

VM_Viewer::VM_Viewer(
        QWidget *parent, virConnect *conn, QString arg1, QString arg2) :
    QMainWindow(parent), jobConnect(conn), connName(arg1), domain(arg2)
{
    qRegisterMetaType<QString>("QString&");
    setMinimumSize(100, 100);
    setWindowTitle(QString("<%1> Virtual Machine").arg(domain));
    VM_State = true;
    viewerToolBar = new ViewerToolBar(this);
    viewerToolBar->setEnabled(false);
    addToolBar(Qt::TopToolBarArea, viewerToolBar);
    connect(viewerToolBar, SIGNAL(execMethod(const QStringList&)),
            this, SLOT(resendExecMethod(const QStringList&)));
    closeProcess = new QProgressBar(this);
    closeProcess->setRange(0, TIMEOUT);
    statusBar()->addPermanentWidget(closeProcess);
    statusBar()->hide();
    // for new virConnect usage create the new virConnectRef[erence]
    if ( virConnectRef(jobConnect)<0 ) jobConnect = NULL;
}
VM_Viewer::~VM_Viewer()
{
    //qDebug()<<"VM_Viewer destroy:";
    if ( timerId>0 ) killTimer(timerId);
    if ( killTimerId>0 ) killTimer(killTimerId);
    if ( NULL!=viewerToolBar ) {
        disconnect(viewerToolBar, SIGNAL(execMethod(const QStringList&)),
                   this, SLOT(resendExecMethod(const QStringList&)));
        delete viewerToolBar;
        viewerToolBar = NULL;
    };
    if ( NULL!=closeProcess ) {
        delete closeProcess;
        closeProcess = NULL;
    };
    // release the reference because no longer required
    if ( jobConnect!=NULL ) {
        virConnectClose(jobConnect);
        // for reject the multiple releasing the reference
        jobConnect = NULL;
    };
    VM_State = false;
    //qDebug()<<"VM_Viewer destroyed";
}

/* public slots */
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
        QString msg = QString("'<b>%1</b>' viewer closed.").arg(domain);
        sendErrMsg(msg);
        ev->accept();
    }
}
void VM_Viewer::sendErrMsg(QString &msg)
{
    QString time = QTime::currentTime().toString();
    QString title = QString("Connect '%1'").arg(connName);
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
    if ( true ) {
        args.append(domain);
        if        ( method.first()=="startVirtDomain" ) {
            args.prepend(method.first());
            args.prepend(QString::number(START_ENTITY));
            args.prepend(connName);
            emit addNewTask(jobConnect, args);
        } else if ( method.first()=="pauseVirtDomain" ) {
            args.append( QString::number(VM_State ? 1 : 0) );
            args.prepend(method.first());
            args.prepend(QString::number(PAUSE_ENTITY));
            args.prepend(connName);
            emit addNewTask(jobConnect, args);
        } else if ( method.first()=="destroyVirtDomain" ) {
            args.prepend(method.first());
            args.prepend(QString::number(DESTROY_ENTITY));
            args.prepend(connName);
            emit addNewTask(jobConnect, args);
        } else if ( method.first()=="resetVirtDomain" ) {
            args.prepend(method.first());
            args.prepend(QString::number(RESET_ENTITY));
            args.prepend(connName);
            emit addNewTask(jobConnect, args);
        } else if ( method.first()=="shutdownVirtDomain" ) {
            args.prepend(method.first());
            args.prepend(QString::number(SHUTDOWN_ENTITY));
            args.prepend(connName);
            emit addNewTask(jobConnect, args);
        } else if ( method.first()=="saveVirtDomain" ) {
            QString to = QFileDialog::getSaveFileName(this, "Save to", "~");
            if ( !to.isEmpty() ) {
                args.append(to);
                args.append( QString::number(VM_State ? 1 : 0) );
                args.prepend(method.first());
                args.prepend(QString::number(SAVE_ENTITY));
                args.prepend(connName);
                emit addNewTask(jobConnect, args);
            };
        } else if ( method.first()=="restoreVirtDomain" ) {
            QString from = QFileDialog::getOpenFileName(this, "Restore from", "~");
            if ( !from.isEmpty() ) {
                args.append(from);
                args.prepend(method.first());
                args.prepend(QString::number(RESTORE_ENTITY));
                args.prepend(connName);
                emit addNewTask(jobConnect, args);
            };
        };
    };
}
void VM_Viewer::startCloseProcess()
{
    qDebug()<<"startCloseProcess";
    killTimerId = startTimer(PERIOD);
    statusBar()->show();
    //qDebug()<<killTimerId<<"killTimer";
}
