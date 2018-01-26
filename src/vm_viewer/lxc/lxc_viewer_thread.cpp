#include "lxc_viewer_thread.h"
#include <unistd.h>
//#include <QTextStream>

#define BLOCK_SIZE  1024*100

LXC_ViewerThread::LXC_ViewerThread(QObject *parent) :
    ControlThread(parent)
{
    ptySlaveFd = -1;
    stream = nullptr;
    domainPtr = nullptr;
    streamRegistered = false;
    connRef = false;
    EndOfFile = false;
}
LXC_ViewerThread::~LXC_ViewerThread()
{
    if (ptySlaveFd) {
        close(ptySlaveFd);
        ptySlaveFd = -1;
        //qDebug()<<"stream"<<stream<<keep_alive<<ptySlaveFd<<"in delete";
    };
    unregisterStreamEvents();
    if ( connRef ) virConnectClose(*ptr_ConnPtr);
    keep_alive = false;
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
    if ( ptr_ConnPtr ) {
        connRef = !(virConnectRef(*ptr_ConnPtr)<0);
    } else
        connRef = false;
    stream = (connRef)?
                virStreamNew( *ptr_ConnPtr, VIR_STREAM_NONBLOCK )
              : nullptr;
    if ( nullptr==stream ) {
        sendConnErrors();
        keep_alive = false;
    } else {
        int ret = virDomainOpenConsole(
                    domainPtr, nullptr, stream, 0 );
        QString msg;
        if ( ret+1 ) {
            msg = QString(
                        tr("In '<b>%1</b>': Console opened in ZERO-mode..."))
                    .arg(domain);
            emit errorMsg(msg, number);
        } else {
            msg = QString(
                        tr("In '<b>%1</b>': Open console failed..."))
                    .arg(domain);
            emit errorMsg(msg, number);
            sendConnErrors();
        };
        if ( ret<0 ) {
            keep_alive = false;
        } else if ( !(streamRegistered=(registerStreamEvents()==0)) ) {
            keep_alive = false;
        };
    };
    //qDebug()<<"stream"<<stream<<keep_alive<<ptySlaveFd<<"init in run";
    //QTextStream s(stdout);
    //s<<"stream "<<stream<<" "<<keep_alive<<" "<<ptySlaveFd<<" init in run"<<endl;
    while (keep_alive) {
        msleep(100);
    };
}

/* private slots */
int  LXC_ViewerThread::registerStreamEvents()
{
    //qDebug()<<"registerStreamEvents";
    int ret = virStreamEventAddCallback(
                stream,
                VIR_STREAM_EVENT_READABLE |
                VIR_STREAM_EVENT_HANGUP   |
                VIR_STREAM_EVENT_ERROR,
                streamEventCallBack,
                this,
    //  don't register freeCallback, because it remove itself
                nullptr);
    if (ret<0) sendConnErrors();
    return ret;
}
void LXC_ViewerThread::unregisterStreamEvents()
{
    //qDebug()<<"unregisterStreamEvents";
    if ( nullptr!=stream && streamRegistered ) {
        streamRegistered = !(virStreamEventRemoveCallback(stream)==0);
        if ( streamRegistered ) {
            sendConnErrors();
        } else {
            if ( virStreamAbort(stream)<0 ) {
                sendConnErrors();
            };
            if ( virStreamFree(stream)<0 ) {
                sendConnErrors();
            };
        };
    };
    //qDebug()<<"StreamEvents state:"<<streamRegistered;
}
void LXC_ViewerThread::freeData(void *opaque)
{
    if ( nullptr!=opaque ) {
        void *data = opaque;
        free(data);
    }
}
void LXC_ViewerThread::streamEventCallBack(virStreamPtr _stream, int events, void *opaque)
{
    //Q_UNUSED(_stream);
    //QTextStream s(stdout);
    //qDebug()<<"streamEventCallBack";
    //s<<"streamEventCallBack"<<" "<<_stream<<endl;
    LXC_ViewerThread *obj = static_cast<LXC_ViewerThread*>(opaque);
    if ( nullptr==obj ) {
        //qDebug()<<"streamEventCallBack"<<"static_cast returns nullptr";
        obj->keep_alive = false;
        return;
    };
    if ( obj->EndOfFile ) {
        //qDebug()<<"streamEventCallBack"<<"EndOfFile cause";
        obj->unregisterStreamEvents();
        obj->keep_alive = false;
        return;
    };
    if ( events & VIR_STREAM_EVENT_ERROR ||
         events & VIR_STREAM_EVENT_HANGUP ) {
        //qDebug()<<"VIR_STREAM_EVENT_HANGUP";
        // Received stream ERROR/HANGUP, closing console
        obj->unregisterStreamEvents();
        obj->keep_alive = false;
    } else if ( events & VIR_STREAM_EVENT_READABLE ) {
        //qDebug()<<"VIR_STREAM_EVENT_READABLE"<<obj->stream;
        //s<<"stream readable"<< endl;
        if ( obj->streamRegistered ) obj->sendDataToDisplay();
    } else if ( events & VIR_STREAM_EVENT_WRITABLE ) {
        //qDebug()<<"VIR_STREAM_EVENT_WRITABLE";
        //obj->sendDataToVMachine();
    } else {
        //qDebug()<<"streamEventCallBack"<<"unused case";
        obj->unregisterStreamEvents();
        obj->keep_alive = false;
    };
}
void LXC_ViewerThread::updateStreamEvents(virStreamPtr _stream, int type)
{
    Q_UNUSED(_stream);
    //qDebug()<<"updateStreamEvents";
    //if ( !streamRegistered ) return;
    int ret = virStreamEventUpdateCallback(stream, type);
    if (ret<0) sendConnErrors();
}
void LXC_ViewerThread::sendDataToDisplay()
{
    //qDebug()<<"sendDataToDisplay"<<"to"<<ptySlaveFd;
    //QTextStream s(stdout);
    //s<<"sendDataToDisplay "<<"to "<<ptySlaveFd<<" "<<stream<<endl;
    if ( nullptr==stream || !keep_alive ||
         !streamRegistered || EndOfFile ) {
        //qDebug()<<"sendDataToDisplay"<<"callback stream is nullptr or deregistered or thread is died or stream EOF";
        keep_alive = false;
        return;
    };
    QString msg;
    size_t _size = sizeof(char)*BLOCK_SIZE;
    char *buff = (char*) malloc(_size);
    if (buff==nullptr) return;
    int got = virStreamRecv(stream, buff, _size);
    switch ( got ) {
    case -2:
        // This is basically EAGAIN
        //qDebug()<<"sendDataToDisplay"<<"EAGAIN";
        //s<<"sendDataToDisplay "<<"EAGAIN"<<endl;
        break;
    case 0:
        // Received EOF from stream, closing
        //qDebug()<<"sendDataToDisplay"<<"Received EOF";
        //s<<"sendDataToDisplay "<<"Received EOF"<<endl;
        if ( !EndOfFile ) {
            EndOfFile = true;
            unregisterStreamEvents();
            if (ptySlaveFd) {
                write(ptySlaveFd, "\nEOF...", 7);
                close(ptySlaveFd);
                ptySlaveFd = -1;
                //qDebug()<<"stream"<<stream<<keep_alive<<ptySlaveFd<<"in EOF";
            };
            msg = QString(tr("In '<b>%1</b>': EOF.")).arg(domain);
            emit errorMsg(msg, number);
            //qDebug()<<"emit errMsg";
        };
        keep_alive = false;
        break;
    case -1:
        // Error stream
        if (ptySlaveFd) {
            write(ptySlaveFd, "\nError in callback stream...", 28);
        };
        sendConnErrors();
        //qDebug()<<"sendDataToDisplay"<<"Error in callback stream"<<stream;
        //s<<"sendDataToDisplay "<<"Error in callback stream "<<stream<<endl;
        keep_alive = false;
        break;
    default:
        // send to TermEmulator stdout useing ptySlaveFd
        //qDebug()<<"sendDataToDisplay"<<"send to TermEmulator stdout"<<ptySlaveFd;
        //s<<"sendDataToDisplay "<<"send to TermEmulator stdout "<<ptySlaveFd<<endl;
        for ( int i=0; i<got; i++ ) {
            if (ptySlaveFd) {
                int sent = write(ptySlaveFd, &buff[i], 1);
                if ( sent<0 ) {
                    keep_alive = false;
                    break;
                };
            };
        };
        break;
    };
    free(buff);
}
void LXC_ViewerThread::sendDataToVMachine(const char *buff, int got)
{
    //qDebug()<<"sendDataToVMachine"<<"from"<<ptySlaveFd;
    //QTextStream s(stdout);
    //s<<"sendDataToVMachine "<<"from "<<ptySlaveFd<<" "<<buff<<" "<<got;
    //s<<" stream "<<stream<<" keep_alive "<<keep_alive;
    //s<<" streamRegistered "<<streamRegistered<<endl;
    if ( got<=0 || nullptr==stream ||
         !keep_alive || !streamRegistered || EndOfFile ) return;
    int saved = virStreamSend(stream, buff, got);
    if ( saved==-2 ) {
        // This is basically EAGAIN
        //s<<"sendDataToVMachine EAGAIN"<<endl;
        return;
    } else if ( saved==-1 ) {
        sendConnErrors();
        keep_alive = false;
        //s<<"sendDataToVMachine error"<<endl;
        return;
    } else {
        //qDebug()<<saved<<"sent";
        //s<<"sent "<<saved<<endl;
    };
    updateStreamEvents(
                stream,
                VIR_STREAM_EVENT_READABLE |
                VIR_STREAM_EVENT_ERROR    |
                VIR_STREAM_EVENT_HANGUP);
}
