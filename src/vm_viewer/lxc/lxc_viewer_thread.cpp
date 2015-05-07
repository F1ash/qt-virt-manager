#include "lxc_viewer_thread.h"

#define BLOCK_SIZE  1024*100

LXC_ViewerThread::LXC_ViewerThread(QObject *parent) :
    ControlThread(parent)
{

}
LXC_ViewerThread::~LXC_ViewerThread()
{
    bool closed = wait(60000);
    //qDebug()<<"delete thread:"<<closed;
    if (!closed) {
        setTerminationEnabled(true);
        terminate();
        closed = wait(60000);
        //qDebug()<<"delete thread:"<<closed;
    };
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
    stream = (NULL!=currWorkConnect)? virStreamNew( currWorkConnect, VIR_STREAM_NONBLOCK ):NULL;
    if ( NULL==stream ) {
        sendConnErrors();
        keep_alive = false;
    } else {
        int ret = -1;
        QString msg;
        if ( ret=virDomainOpenConsole( domainPtr, NULL, stream, VIR_DOMAIN_CONSOLE_SAFE)+1 ) {
            msg = QString("In '<b>%1</b>': Console opened in SAFE-mode...").arg(domain);
            emit errorMsg(msg);
        } else if ( ret=virDomainOpenConsole( domainPtr, NULL, stream, VIR_DOMAIN_CONSOLE_FORCE )+1 ) {
            msg = QString("In '<b>%1</b>': Console opened in FORCE-mode...").arg(domain);
            emit errorMsg(msg);
        } else if ( ret=virDomainOpenConsole( domainPtr, NULL, stream, 0 )+1 ) {
            msg = QString("In '<b>%1</b>': Console opened in ZIRO-mode...").arg(domain);
            emit errorMsg(msg);
        } else {
            msg = QString("In '<b>%1</b>': Open console failed...").arg(domain);
            emit errorMsg(msg);
            sendConnErrors();
        };
        if ( ret<0 ) {
            keep_alive = false;
        } else if ( registerStreamEvents()<0 ) {
            keep_alive = false;
        };
    };
    while (keep_alive) {
        msleep(100);
    };
}
void LXC_ViewerThread::stop()
{
    if ( isRunning() ) closeStream();
}

/* private slots */
int  LXC_ViewerThread::registerStreamEvents()
{
    int ret = virStreamEventAddCallback(stream,
                                        VIR_STREAM_EVENT_READABLE |
                                        VIR_STREAM_EVENT_HANGUP |
                                        VIR_STREAM_EVENT_ERROR,
                                        streamEventCallBack, this,
    //  don't register freeCallback, because it remove itself
                                        NULL);
    if (ret<0) sendConnErrors();
    return ret;
}
int  LXC_ViewerThread::unregisterStreamEvents()
{
    int ret = virStreamEventRemoveCallback(stream);
    if (ret<0) sendConnErrors();
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
    LXC_ViewerThread *obj = static_cast<LXC_ViewerThread*>(opaque);
    if ( NULL==obj ) return;
    if ( events & VIR_STREAM_EVENT_ERROR ||
         events & VIR_STREAM_EVENT_HANGUP ) {
        // Received stream ERROR/HANGUP, closing console
        obj->closeStream();
        return;
    };
    if ( events & VIR_STREAM_EVENT_READABLE ) {
        obj->sendDataToDisplay(_stream);
    };
    if ( events & VIR_STREAM_EVENT_WRITABLE ) {
        //obj->sendDataToVMachine(_stream);
    };
}
void LXC_ViewerThread::updateStreamEvents(virStreamPtr _stream, int type)
{
    int ret = virStreamEventUpdateCallback(_stream, type);
    if (ret<0) sendConnErrors();
}
void LXC_ViewerThread::sendDataToDisplay(virStreamPtr _stream)
{
    if ( NULL==_stream ) return;
    QString msg;
    char buff[BLOCK_SIZE];
    memset( buff, '\0', BLOCK_SIZE );
    int got = virStreamRecv(_stream, buff, BLOCK_SIZE);
    switch ( got ) {
    case -2:
        // This is basically EAGAIN
        break;
    case 0:
        // Received EOF from stream, closing
        if ( NULL!=stream ) {
            closeStream();
            write(ptySlaveFd, "\nEOF...", 7);
            close(ptySlaveFd);
            msg = QString("In '<b>%1</b>': EOF.").arg(domain);
            emit errorMsg(msg);
            emit termEOF();
            //qDebug()<<"EOF emited";
        };
        break;
    case -1:
        // Error stream
        if ( NULL!=stream ) {
            virStreamAbort(stream);
            closeStream();
        };
        break;
    default:
        // send to TermEmulator stdout useing ptySlaveFd
        for ( int i=0; i<got; i++ ) {
            int sent = write(ptySlaveFd, &buff[i], 1);
            if ( sent<0 ) {
                virStreamAbort(_stream);
                closeStream();
                break;
            };
        };
        break;
    };
}
void LXC_ViewerThread::sendDataToVMachine(const char *buff, int got)
{
    if ( got<=0 || NULL==stream ) return;
    int saved = virStreamSend(stream, buff, got);
    if ( saved==-2 ) {
        // This is basically EAGAIN
        return;
    } else if ( saved==-1 ) {
        sendConnErrors();
    } else {
        //qDebug()<<saved<<"sent";
    };
    updateStreamEvents(stream,
                       VIR_STREAM_EVENT_READABLE |
                       VIR_STREAM_EVENT_ERROR |
                       VIR_STREAM_EVENT_HANGUP);
}
void LXC_ViewerThread::closeStream()
{
    //qDebug()<<"stream close:";
    if ( NULL!=stream ) {
        int ret = unregisterStreamEvents();
        if ( ret+1 && virStreamFinish(stream)<0 ) {
            sendConnErrors();
        };
        if ( ret+1 && virStreamFree(stream)<0 ) {
            sendConnErrors();
        };
        stream = NULL;
        //qDebug()<<"stream closed";
    };
    //qDebug()<<"stream closed already";
    keep_alive = false;
}
