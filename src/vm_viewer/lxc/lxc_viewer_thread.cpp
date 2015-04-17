#include "lxc_viewer_thread.h"

#define BLOCK_SIZE  1024*100

LXC_ViewerThread::LXC_ViewerThread(QObject *parent, virConnect *_conn) :
    ControlThread(parent), jobConnect(_conn)
{
    bool ret = setCurrentWorkConnect(jobConnect);
    stream = (ret)? virStreamNew( jobConnect, VIR_STREAM_NONBLOCK ):NULL;
    if ( NULL==stream ) {
        sendConnErrors();
    };
}

LXC_ViewerThread::~LXC_ViewerThread()
{
    closeStream();
}

/* public slots */
void LXC_ViewerThread::run()
{
    while (keep_alive) {
        msleep(100);
    };
}

/* private slots */
int  LXC_ViewerThread::registerStreamEvents(QString &domain, virDomainPtr domainPtr, int fd)
{
    ptySlaveFd = fd;
    /*
     * Older servers did not support either flag,
     * and also did not forbid simultaneous clients on a console,
     * with potentially confusing results.
     * When passing @flags of 0 in order to support a wider range of server versions,
     * it is up to the client to ensure mutual exclusion.
     */
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
        closeStream();
        return ret;
    };
    ret = virStreamEventAddCallback(stream,
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
    if ( opaque!=NULL ) {
        void *data = opaque;
        free(data);
    }
}
void LXC_ViewerThread::streamEventCallBack(virStreamPtr _stream, int events, void *opaque)
{
    LXC_ViewerThread *obj = static_cast<LXC_ViewerThread*>(opaque);
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
    QString msg;
    char buff[BLOCK_SIZE];
    memset( buff, '\0', BLOCK_SIZE );
    int got = virStreamRecv(_stream, buff, BLOCK_SIZE);
    switch ( got ) {
    case -2:
        // This is basically EAGAIN
        return;
    case 0:
        // Received EOF from stream, closing
        closeStream();
        write(ptySlaveFd, "\nEOF...", 7);
        msg = QString("In '<b>%1</b>': EOF.");
        emit errorMsg(msg);
        emit termEOF();
        //qDebug()<<"EOF emited";
        return;
    case -1:
        // Error stream
        return;
    default:
        // send to TermEmulator stdout useing ptySlaveFd
        for ( int i=0; i<got; i++ ) {
            write(ptySlaveFd, &buff[i], 1);
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
    if ( stream!=NULL ) {
        if ( virStreamEventUpdateCallback(
                 stream,
                 VIR_STREAM_EVENT_READABLE |
                 VIR_STREAM_EVENT_WRITABLE |
                 VIR_STREAM_EVENT_ERROR |
                 VIR_STREAM_EVENT_HANGUP)<0 )
            sendConnErrors();
        unregisterStreamEvents();
        if ( virStreamFinish(stream)+1 )
            virStreamFree(stream);
        stream = NULL;
        //qDebug()<<"stream closed";
    };
    //qDebug()<<"stream closed already";
    keep_alive = false;
}
