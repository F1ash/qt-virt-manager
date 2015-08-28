#include "lxc_viewer_thread.h"

#define BLOCK_SIZE  1024*100

LXC_ViewerThread::LXC_ViewerThread(QObject *parent) :
    ControlThread(parent)
{
    ptySlaveFd = -1;
    stream = NULL;
    domainPtr = NULL;
    connect(this, SIGNAL(streamIObroken()),
            this, SLOT(forceCloseDomain()));
}
LXC_ViewerThread::~LXC_ViewerThread()
{
    disconnect(this, SIGNAL(streamIObroken()),
               this, SLOT(forceCloseDomain()));
    stream = NULL;
    if (isRunning()) {
        setTerminationEnabled(true);
        terminate();
        //qDebug()<<"delete thread:"<<closed;
    };
    wait(30000);
}

/* public slots */
void LXC_ViewerThread::setData(QString &_dom, virDomainPtr _domPtr, int fd)
{
    ptySlaveFd = fd;
    domain = _dom;
    domainPtr = _domPtr;
}
void LXC_ViewerThread::run()
{
    stream = (NULL!=currWorkConnection)?
                virStreamNew( currWorkConnection, VIR_STREAM_NONBLOCK ):NULL;
    if ( NULL==stream ) {
        sendConnErrors();
        keep_alive = false;
    } else {
        int ret = -1;
        QString msg;
        if ( ret=virDomainOpenConsole( domainPtr, NULL, stream, VIR_DOMAIN_CONSOLE_SAFE)+1 ) {
            msg = QString("In '<b>%1</b>': Console opened in SAFE-mode...").arg(domain);
            emit errorMsg(msg, number);
        } else if ( ret=virDomainOpenConsole( domainPtr, NULL, stream, VIR_DOMAIN_CONSOLE_FORCE )+1 ) {
            msg = QString("In '<b>%1</b>': Console opened in FORCE-mode...").arg(domain);
            emit errorMsg(msg, number);
        } else if ( ret=virDomainOpenConsole( domainPtr, NULL, stream, 0 )+1 ) {
            msg = QString("In '<b>%1</b>': Console opened in ZERO-mode...").arg(domain);
            emit errorMsg(msg, number);
        } else {
            msg = QString("In '<b>%1</b>': Open console failed...").arg(domain);
            emit errorMsg(msg, number);
            sendConnErrors();
        };
        if ( ret<0 ) {
            keep_alive = false;
        } else if ( registerStreamEvents()<0 ) {
            keep_alive = false;
        };
    };
    qDebug()<<"stream"<<stream<<keep_alive<<ptySlaveFd;
    while (keep_alive) {
        msleep(100);
    };
    closeStream();
    if (ptySlaveFd) {
        close(ptySlaveFd);
        ptySlaveFd = -1;
        qDebug()<<"stream"<<stream<<keep_alive<<ptySlaveFd;
    };
}
void LXC_ViewerThread::stop()
{
    qDebug()<<"stop";
    keep_alive = false;
}

/* private slots */
int  LXC_ViewerThread::registerStreamEvents()
{
    qDebug()<<"registerStreamEvents";
    int ret = virStreamEventAddCallback(
                stream,
                VIR_STREAM_EVENT_READABLE |
                VIR_STREAM_EVENT_HANGUP   |
                VIR_STREAM_EVENT_ERROR,
                streamEventCallBack,
                this,
    //  don't register freeCallback, because it remove itself
                NULL);
    if (ret<0) sendConnErrors();
    return ret;
}
int  LXC_ViewerThread::unregisterStreamEvents(virStreamPtr _stream)
{
    qDebug()<<"unregisterStreamEvents";
    int ret = virStreamEventRemoveCallback(_stream);
    if (ret<0) {
        sendConnErrors();
        emit streamIObroken();
    };
    return ret;
}
void LXC_ViewerThread::freeData(void *opaque)
{
    if ( NULL!=opaque ) {
        void *data = opaque;
        free(data);
    }
}
void LXC_ViewerThread::streamEventCallBack(virStreamPtr _stream, int events, void *opaque)
{
    qDebug()<<"streamEventCallBack";
    LXC_ViewerThread *obj = static_cast<LXC_ViewerThread*>(opaque);
    if ( NULL==obj ) {
        qDebug()<<"streamEventCallBack"<<"static_cast returns NULL";
        return;
    };
    if ( !obj->keep_alive ) {
        qDebug()<<"streamEventCallBack"<<"thread not alive";
        obj->unregisterStreamEvents(_stream);
        return;
    };
    if ( _stream==NULL ) {
        qDebug()<<"streamEventCallBack"<<"callback stream is NULL";
        return;
    };
    if ( events & VIR_STREAM_EVENT_ERROR ||
         events & VIR_STREAM_EVENT_HANGUP ) {
        qDebug()<<"VIR_STREAM_EVENT_HANGUP";
        // Received stream ERROR/HANGUP, closing console
        obj->closeStream();
    } else if ( events & VIR_STREAM_EVENT_READABLE ) {
        qDebug()<<"VIR_STREAM_EVENT_READABLE"<<_stream;
        obj->sendDataToDisplay(_stream);
    } else if ( events & VIR_STREAM_EVENT_WRITABLE ) {
        qDebug()<<"VIR_STREAM_EVENT_WRITABLE";
        //obj->sendDataToVMachine();
    } else {
        qDebug()<<"streamEventCallBack"<<"unused case";
    };
}
void LXC_ViewerThread::updateStreamEvents(virStreamPtr _stream, int type)
{
    qDebug()<<"updateStreamEvents";
    int ret = virStreamEventUpdateCallback(_stream, type);
    if (ret<0) sendConnErrors();
}
void LXC_ViewerThread::sendDataToDisplay(virStreamPtr _stream)
{
    qDebug()<<"sendDataToDisplay"<<"to"<<ptySlaveFd;
    if ( NULL==_stream ) {
        qDebug()<<"sendDataToDisplay"<<"callback stream is NULL";
        unregisterStreamEvents(_stream);
        return;
    };
    qDebug()<<"sendDataToDisplay"<<"stream not NULL";
    if ( !keep_alive ) {
        qDebug()<<"sendDataToDisplay"<<"thread is not alive";
        unregisterStreamEvents(_stream);
        return;
    };
    QString msg;
    char buff[BLOCK_SIZE];
    memset( buff, '\0', BLOCK_SIZE );
    int got = virStreamRecv(_stream, buff, BLOCK_SIZE);
    switch ( got ) {
    case -2:
        // This is basically EAGAIN
        qDebug()<<"sendDataToDisplay"<<"EAGAIN";
        break;
    case 0:
        // Received EOF from stream, closing
        qDebug()<<"sendDataToDisplay"<<"Received EOF";
        if ( NULL!=_stream && keep_alive ) {
            closeStream();
            write(ptySlaveFd, "\nEOF...", 7);
            close(ptySlaveFd);
            ptySlaveFd = -1;
            msg = QString("In '<b>%1</b>': EOF.").arg(domain);
            emit errorMsg(msg, number);
            emit termEOF();
            qDebug()<<"EOF emited";
        };
        break;
    case -1:
        // Error stream
        write(ptySlaveFd, "\nError in callback stream...", 19);
        close(ptySlaveFd);
        ptySlaveFd = -1;
        sendConnErrors();
        qDebug()<<"sendDataToDisplay"<<"Error in callback stream"<<_stream;
        unregisterStreamEvents(_stream);
        if ( virStreamFree(_stream)<0 ) {
            sendConnErrors();
        };
        keep_alive = false;
        emit streamIObroken();
        break;
    default:
        // send to TermEmulator stdout useing ptySlaveFd
        qDebug()<<"sendDataToDisplay"<<"send to TermEmulator stdout"<<ptySlaveFd;
        for ( int i=0; i<got; i++ ) {
            int sent = write(ptySlaveFd, &buff[i], 1);
            if ( sent<0 ) {
                close(ptySlaveFd);
                ptySlaveFd = -1;
                keep_alive = false;
                break;
            };
        };
        break;
    };
}
void LXC_ViewerThread::sendDataToVMachine(const char *buff, int got)
{
    qDebug()<<"sendDataToVMachine"<<"from"<<ptySlaveFd;
    if ( got<=0 || NULL==stream || !keep_alive ) return;
    int saved = virStreamSend(stream, buff, got);
    if ( saved==-2 ) {
        // This is basically EAGAIN
        return;
    } else if ( saved==-1 ) {
        sendConnErrors();
    } else {
        //qDebug()<<saved<<"sent";
    };
    updateStreamEvents(
                stream,
                VIR_STREAM_EVENT_READABLE |
                VIR_STREAM_EVENT_ERROR    |
                VIR_STREAM_EVENT_HANGUP);
}
void LXC_ViewerThread::closeStream()
{
    qDebug()<<"stream close:";
    if ( NULL!=stream ) {
        // don't finish the stream, because it occure
        // callback stream event and loop
        //if ( virStreamFinish(stream)<0 ) {
        //    sendConnErrors();
        //};
        //qDebug()<<"stream close:"<<"finish";
        unregisterStreamEvents(stream);
        qDebug()<<"stream close:"<<"unregistered";
        if ( virStreamFree(stream)<0 ) {
            sendConnErrors();
        };
        stream = NULL;
        qDebug()<<"stream close:"<<"free"<<stream;
    };
    qDebug()<<"stream closed already";
    keep_alive = false;
}
void LXC_ViewerThread::forceCloseDomain()
{
    qDebug()<<"forceCloseDomain";
    if ( domainPtr!=NULL && virDomainIsActive(domainPtr)) {
        if ( virDomainDestroy(domainPtr)<0 ) {
            sendConnErrors();
        };
    };
}
