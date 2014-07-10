#include "lxc_viewer.h"
#define BLOCK_SIZE 1024*100

LXC_Viewer::LXC_Viewer(QWidget *parent, virConnect *conn, QString str) :
    QWidget(parent), jobConnect(conn), domain(str)
{
    dontActive = new QLabel("Domain is shutdown", this);
    commonLayout = new QVBoxLayout();
    if ( jobConnect!=NULL ) {
        domainPtr = getDomainPtr();
    };QString msg;
    if ( domainPtr!=NULL ) {
        stream = virStreamNew( jobConnect, VIR_STREAM_NONBLOCK );
        if ( stream==NULL ) {
            msg = "Create virtual stream failed...";
            emit errorMsg(msg);
        } else {
            /*
             * Older servers did not support either flag,
             * and also did not forbid simultaneous clients on a console,
             * with potentially confusing results.
             * When passing @flags of 0 in order to support a wider range of server versions,
             * it is up to the client to ensure mutual exclusion.
             */
            int ret;
            if ( ret=virDomainOpenConsole( domainPtr, NULL, stream, VIR_DOMAIN_CONSOLE_SAFE)+1 ) {
                msg = "Console opened in SAFE-mode...";
                emit errorMsg(msg);
            } else if ( ret=virDomainOpenConsole( domainPtr, NULL, stream, VIR_DOMAIN_CONSOLE_FORCE )+1 ) {
                msg = "Console opened in FORCE-mode...";
                emit errorMsg(msg);
            } else if ( ret=virDomainOpenConsole( domainPtr, NULL, stream, 0 )+1 ) {
                msg = "Console opened in ZIRO-mode...";
                emit errorMsg(msg);
            } else {
                msg = "Open console failed...";
                emit errorMsg(msg);
            };
            if ( ret ) {
                display = new QTermWidget(0, this);
                //QFont font = QApplication::font();
                //font.setFamily("Mono");
                //font.setPointSize(12);
                //display->setTerminalFont(font);
                display->setTerminalOpacity(0.5);
                display->setScrollBarPosition(QTermWidget::ScrollBarRight);
                //display->setShellProgram("/usr/bin/mc");
                //connect(display, SIGNAL(receivedData(const QString&)),
                //        this, SLOT(sendData(const QString&)));
                connect(display, SIGNAL(finished()), this, SLOT(close()));
                connect(display, SIGNAL(destroyed()), this, SLOT(close()));
                /*
                connect(display->getSession(),
                        SIGNAL(commToVM(const QByteArray&)),
                        this,
                        SLOT(sendDataToVMachine(const QByteArray&)));
                        */
                commonLayout->addWidget(display);
                // don't start (default) shell program,
                // because take the data from VM Stream
                // display->startShellProgram();
                //ptySlaveFd = display->getSession()->getSlaveFd();
                //qDebug()<<ptySlaveFd<<"slFd";
                ret = registerStreamEvents();
                if ( ret<0 ) {
                    msg = QString("Open PTY Error...");
                    emit errorMsg(msg);
                };
                //else updateStreamEvents(stream,
                //                          VIR_STREAM_EVENT_READABLE |
                //                          VIR_STREAM_EVENT_ERROR |
                //                          VIR_STREAM_EVENT_HANGUP);
            } else
                commonLayout->addWidget(dontActive, Qt::AlignCenter);
        };
    } else {
        msg = "Connect or Domain is NULL...";
        emit errorMsg(msg);
    };
    setLayout(commonLayout);
}
LXC_Viewer::~LXC_Viewer()
{
    if ( display!=NULL ) {
        //disconnect(display, SIGNAL(receivedData(const QString&)),
        //           this, SLOT(sendData(const QString&)));
        disconnect(display, SIGNAL(finished()), this, SLOT(close()));
        disconnect(display, SIGNAL(destroyed()), this, SLOT(close()));
        /*
         * disconnect(display->getSession(),
                   SIGNAL(commToVM(const QByteArray&)),
                   this,
                   SLOT(sendDataToVMachine(const QByteArray&)));
                   */
        unregisterStreamEvents();
        delete display;
        display = 0;
        //qDebug()<<domain<< "Display destroyed";
    };
    delete dontActive;
    dontActive = 0;
    delete commonLayout;
    commonLayout = 0;
    //qDebug()<<domain<<"Viewer destroyed";
}

/* public slots */
void LXC_Viewer::close()
{
    //commonLayout->removeWidget(display);
    this->close();
}

/* private slots */
virDomain* LXC_Viewer::getDomainPtr() const
{
    return virDomainLookupByName(jobConnect, domain.toUtf8().data());
}
int LXC_Viewer::registerStreamEvents()
{
    int ret = virStreamEventAddCallback(stream,
                                        VIR_STREAM_EVENT_READABLE |
                                        VIR_STREAM_EVENT_HANGUP |
                                        VIR_STREAM_EVENT_ERROR,
                                        streamEventCallBack, this,
    //  don't register freeCallback, because it remove viewer
                                        NULL);
    if (ret<0) sendConnErrors();
    return ret;
}
int LXC_Viewer::unregisterStreamEvents()
{
    int ret = virStreamEventRemoveCallback(stream);
    if (ret<0) sendConnErrors();
    return ret;
}
void LXC_Viewer::freeData(void *opaque)
{
    if ( opaque!=NULL ) {
        void *data = opaque;
        free(data);
    }
}
void LXC_Viewer::streamEventCallBack(virStreamPtr _stream, int events, void *opaque)
{
    LXC_Viewer *obj = static_cast<LXC_Viewer*>(opaque);
    if ( events & VIR_STREAM_EVENT_ERROR ||
         events & VIR_STREAM_EVENT_HANGUP ) {
        // Received stream ERROR/HANGUP, closing console
        obj->streamClosed();
        return;
    };
    if ( events & VIR_STREAM_EVENT_READABLE ) {
        obj->sendDataToDisplay(_stream);
    };
    if ( events & VIR_STREAM_EVENT_WRITABLE ) {
        obj->sendDataToVMachine(_stream);
    };
}
void LXC_Viewer::updateStreamEvents(virStreamPtr _stream, int type)
{
    int ret = virStreamEventUpdateCallback(_stream, type);
    if (ret<0) sendConnErrors();
}
void LXC_Viewer::sendDataToDisplay(virStreamPtr _stream)
{
    char buf[BLOCK_SIZE];
    int got = virStreamRecv(_stream, buf, BLOCK_SIZE);
    switch ( got ) {
    case -2:
        // This is basically EAGAIN
        return;
    case 0:
        // Received EOF from stream, closing
        streamClosed();
        return;
    case -1:
        // Error stream
        return;
    default:
        // send to TermEmulator stdout useing pty->slaveFd
        ::write(ptySlaveFd, &buf, got);
        //qDebug()<<QString().fromUtf8(buf)<<"toTerm";
        //QString answer = QString().fromUtf8(buf);
        //display->sendText(answer);
        break;
    };
}
void LXC_Viewer::sendDataToVMachine(virStreamPtr _stream)
{
    if ( terminalToStream.isEmpty() ) {
        return;
    };
    int saved = virStreamSend(_stream, terminalToStream.toUtf8().data(), BLOCK_SIZE);
    if ( saved==-2 ) {
        // This is basically EAGAIN
        return;
    } else if ( saved==-1 ) {
        sendConnErrors();
    } else {
        qDebug()<<saved<<"sent";
        terminalToStream.remove(0, saved);
    };
    if ( terminalToStream.isEmpty() ) {
        updateStreamEvents(_stream,
                           VIR_STREAM_EVENT_READABLE |
                           VIR_STREAM_EVENT_ERROR |
                           VIR_STREAM_EVENT_HANGUP);
    };
}
void LXC_Viewer::sendDataToVMachine(const QByteArray &comm)
{
    qDebug()<<terminalToStream.append(comm)<<"recv from Term";
    updateStreamEvents(stream,
                       VIR_STREAM_EVENT_WRITABLE |
                       VIR_STREAM_EVENT_ERROR |
                       VIR_STREAM_EVENT_HANGUP);
}

void LXC_Viewer::streamClosed()
{
    if ( stream!=NULL ) {
        virStreamFinish(stream);
        virStreamFree(stream);
    };
    qDebug()<<"stream closed";
    //close();
}

void LXC_Viewer::sendConnErrors()
{
    virtErrors = virConnGetLastError(jobConnect);
    if ( virtErrors!=NULL ) {
        QString msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void LXC_Viewer::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL ) {
        QString msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg );
    };
    virResetLastError();
}
