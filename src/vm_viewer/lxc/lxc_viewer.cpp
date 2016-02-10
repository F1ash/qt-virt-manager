#include "lxc_viewer.h"

lxcHlpThread::lxcHlpThread(
        QObject *parent, virConnectPtr* connPtrPtr, QString _domain) :
    _VirtThread(parent, connPtrPtr), domain(_domain)
{

}
void lxcHlpThread::run()
{
    if ( NULL==ptr_ConnPtr || NULL==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };
    domainPtr =virDomainLookupByName(
                    *ptr_ConnPtr, domain.toUtf8().data());
    domainIsActive = (virDomainIsActive(domainPtr)>0);
    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
}

LXC_Viewer::LXC_Viewer(
        QWidget *parent, virConnectPtr *connPtrPtr,
        QString arg1, QString arg2,
        const QString& work_dir, const QString& command) :
    TermMainWindow(parent, connPtrPtr, arg1, arg2, work_dir, command)
{
    TYPE = "LXC";
    // unused toolbar & state widget
    viewerToolBar->setVisible(false);
    vm_stateWdg->setVisible(false);
    hlpThread = new lxcHlpThread(this, ptr_ConnPtr, domain);
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(init()));
    connect(hlpThread, SIGNAL(errorMsg(QString&,uint)),
            this, SIGNAL(errorMsg(QString&)));
    hlpThread->start();
}
LXC_Viewer::~LXC_Viewer()
{
    //qDebug()<<"LXC_Viewer destroy:";
    if ( timerId>0 ) {
        killTimer(timerId);
        timerId = 0;
    };
    if ( killTimerId>0 ) {
        killTimer(killTimerId);
        killTimerId = 0;
    };
    VM_State = false;
    QString msg;
    if ( NULL!=viewerThread ) {
        viewerThread->blockSignals(true);
        delete viewerThread;
        viewerThread = NULL;
    };
    msg = QString("In '<b>%1</b>': Display destroyed.")
            .arg(domain);
    sendErrMsg(msg);
    //qDebug()<<"LXC_Viewer destroyed";
}

/* public slots */
void LXC_Viewer::init()
{
    QString msg;
    if ( hlpThread->domainPtr!=NULL && hlpThread->domainIsActive ) {
        viewerThread = new LXC_ViewerThread(this);
        timerId = startTimer(PERIOD);
    } else {
        msg = QString("In '<b>%1</b>':<br> Connection or Domain is NULL or inactive")
                .arg(domain);
        sendErrMsg(msg);
        showErrorInfo(msg);
        startCloseProcess();
    };
    sendConnErrors();
    //qDebug()<<msg<<"term inits";
}

/* private slots */
void LXC_Viewer::timerEvent(QTimerEvent *ev)
{
    if ( ev->timerId()==timerId ) {
        int ptySlaveFd = this->getPtySlaveFd();
        counter++;
        //qDebug()<<counter<<ptySlaveFd;
        if ( ptySlaveFd>0 && NULL!=viewerThread ) {
            killTimer(timerId);
            timerId = 0;
            counter = 0;
            viewerThread->setData(
                        domain, hlpThread->domainPtr, ptySlaveFd);
            if ( viewerThread->setCurrentWorkConnect(ptr_ConnPtr) ) {
                setTerminalParameters();
            };
        } else if ( TIMEOUT<counter*PERIOD ) {
            killTimer(timerId);
            timerId = 0;
            counter = 0;
            QString msg = QString("In '<b>%1</b>': Open PTY Error...").arg(domain);
            sendErrMsg(msg);
            getCurrentTerminal()->impl()->sendText(msg);
            startCloseProcess();
        }
    } else if ( ev->timerId()==killTimerId ) {
        counter++;
        vm_stateWdg->setCloseProcessValue(counter*PERIOD*6);
        if ( TIMEOUT<counter*PERIOD*6 ) {
            counter = 0;
            killTimer(killTimerId);
            killTimerId = 0;
            close();
        };
    }
}
void LXC_Viewer::setTerminalParameters()
{
    TermWidget *t = getCurrentTerminal();
    if ( NULL!=t ) {
        connect(t->impl(), SIGNAL(sendData(const char*,int)),
                viewerThread, SLOT(sendDataToVMachine(const char*,int)));
        connect(viewerThread, SIGNAL(errorMsg(QString&, uint)),
                this, SLOT(sendErrMsg(QString&, uint)));
        viewerThread->start();
        if ( viewerThread->keep_alive ) {
            QString msg = QString("In '<b>%1</b>': Stream Registation success. \
PTY opened. Terminal is active.").arg(domain);
            sendErrMsg(msg);
        };
        /*
         * As usually a xterm terminals don't support
         * the Window manipulation through CSI,
         * then zoomOut it to standard 80x24.
         */
        bool recognized = false;
        do {
            int l = t->impl()->screenLinesCount();
            int c = t->impl()->screenColumnsCount();
            if (l<24 || c<80) t->impl()->zoomOut();
            else recognized = true;
        } while ( !recognized );
        t->impl()->startTerminalTeletype();
    };
}
void LXC_Viewer::closeEvent(QCloseEvent *ev)
{
    if ( ev->type()==QEvent::Close ) {
        ev->ignore();
        QString key = QString("%1_%2").arg(connName).arg(domain);
        if (VM_State) emit finished(key);
    };
}
