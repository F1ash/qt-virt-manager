#include "lxc_viewer.h"

LXC_Viewer::LXC_Viewer(
        QWidget *parent,
        virConnect *conn,
        QString arg1,
        QString arg2,
        const QString& work_dir,
        const QString& command) :
    TermMainWindow(parent, conn, arg1, arg2, work_dir, command)
{
    TYPE = "LXC";
    // unused toolbar
    // viewerToolBar->setVisible(false);
    if ( jobConnect!=NULL ) {
        domainPtr = getDomainPtr();
    };
    QString msg;
    if ( domainPtr!=NULL ) {
        viewerThread = new LXC_ViewerThread(this);
        timerId = startTimer(PERIOD);
    } else {
        msg = QString("In '<b>%1</b>':<br> Connection or Domain is NULL...")
                .arg(domain);
        sendErrMsg(msg);
        showErrorInfo(msg);
        startCloseProcess();
    };
    sendConnErrors();
    //qDebug()<<msg<<"term inits";
}
LXC_Viewer::~LXC_Viewer()
{
    qDebug()<<"LXC_Viewer destroy:";
    QString msg, key;
    msg = QString("In '<b>%1</b>': Display destroyed.")
            .arg(domain);
    sendErrMsg(msg);
    //key = QString("%1_%2").arg(connName).arg(domain);
    //emit finished(key);
    qDebug()<<"LXC_Viewer destroyed";
}

/* public slots */

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
            viewerThread->setData(domain, domainPtr, ptySlaveFd);
            if ( viewerThread->setCurrentWorkConnect(jobConnect) ) {
                setTerminalParameters();
            };
        } else if ( TIMEOUT<counter*PERIOD ) {
            killTimer(timerId);
            timerId = 0;
            counter = 0;
            QString msg = QString("In '<b>%1</b>': Open PTY Error...").arg(domain);
            sendErrMsg(msg);
            getCurrentTerminal()->impl()->sendText(msg);
        }
    } else if ( ev->timerId()==killTimerId ) {
        counter++;
        closeProcess->setValue(counter*PERIOD*6);
        if ( TIMEOUT<counter*PERIOD*6 ) {
            killTimer(killTimerId);
            killTimerId = 0;
            counter = 0;
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
        connect(viewerThread, SIGNAL(termEOF()),
                this, SLOT(startCloseProcess()));
        connect(viewerThread, SIGNAL(errorMsg(QString&, uint)),
                this, SLOT(sendErrMsg(QString&, uint)));
        connect(viewerThread, SIGNAL(finished()),
                this, SLOT(startCloseProcess()));
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
    ev->ignore();
    if ( killTimerId>0 ) {
        killTimer(killTimerId);
        killTimerId = 0;
    };
    if ( NULL!=viewerThread ) {
        viewerThread->blockSignals(true);
        viewerThread->stop();
        //deleteLater();
        QString key = QString("%1_%2").arg(connName).arg(domain);
        emit finished(key);
    };
}
