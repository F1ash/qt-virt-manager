#include "lxc_viewer.h"
#define BLOCK_SIZE 1024

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
            /*
             * TODO: implement virStreamEventCallback for read/write stream control
             * for display info into terminal emulator
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
                //ret = ::openpty(&master_fd, &slave_fd, &namePTY, NULL, NULL);
                registerStreamEvents();
                display = new QTermWidget();
                QFont font = QApplication::font();
                font.setFamily("Mono");
                font.setPointSize(12);
                display->setTerminalFont(font);
                //display->setColorScheme(COLOR_SCHEME_BLACK_ON_LIGHT_YELLOW);
                display->setScrollBarPosition(QTermWidget::ScrollBarRight);
                connect(display, SIGNAL(finished()), this, SLOT(close()));
                commonLayout->addWidget(display);
                if ( ret<0 ) {
                    msg = QString("Open PTY Error...");
                    emit errorMsg(msg);
                };
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
    unregisterStreamEvents();
    if ( display!=NULL ) {
        delete display;
        display = 0;
    };
    delete dontActive;
    dontActive = 0;
    if ( stream!=NULL ) {
        virStreamFinish(stream);
        virStreamFree(stream);
    };
    delete commonLayout;
    commonLayout = 0;
}

/* public slots */
void LXC_Viewer::close()
{
    this->close();
}

/* private slots */
virDomain* LXC_Viewer::getDomainPtr() const
{
    return virDomainLookupByName(jobConnect, domain.toUtf8().data());
}
void LXC_Viewer::registerStreamEvents()
{
    int ret = virStreamEventAddCallback(stream,
                                        VIR_STREAM_EVENT_ERROR |
                                        VIR_STREAM_EVENT_HANGUP |
                                        VIR_STREAM_EVENT_READABLE |
                                        VIR_STREAM_EVENT_WRITABLE,
                                        streamCallBack, NULL, freeData);
    if (ret<0) sendConnErrors();
}
void LXC_Viewer::unregisterStreamEvents()
{
    int ret = virStreamEventRemoveCallback(stream);
    if (ret<0) sendConnErrors();
}
void LXC_Viewer::freeData(void * opaque)
{
    void *data = opaque;
    free(data);
}
void LXC_Viewer::streamCallBack(virStreamPtr _stream, int events, void *opaque)
{
    uint length = 0;
    int got, saved, step, recvd;
    char buf[BLOCK_SIZE];
    QString msg;
    switch (events) {
    case VIR_STREAM_EVENT_ERROR:
        break;
    case VIR_STREAM_EVENT_HANGUP:
        break;
    case VIR_STREAM_EVENT_READABLE:
        step = 0;
        while ( 1 ) {
            //got = read(buf, BLOCK_SIZE);
            if (got == 0) break;
            if ( got<0 ) {
                //msg = QString("ReadError after (%2): %1 bytes")
                //        .arg(length).arg(step);
                //emit errorMsg( msg );
            } else {
                saved = virStreamSend(_stream, buf, got);
                if (saved < 0) {
                    break;
                };
                step++;
                length += saved;
                //qDebug()<<"got<>saved:length"<<got<<saved<<step<<length;
            };
        };
        break;
    case VIR_STREAM_EVENT_WRITABLE:
        recvd = 0;
        while ( 1 ) {
            got = virStreamRecv(_stream, buf, BLOCK_SIZE);
            if (got < 0) {
                //msg = QString("WriteError after : %1 bytes").arg(recvd);
                //emit errorMsg( msg );
                break;
            };
            if (got == 0) break;
            recvd += got;
            //msg = QString().fromUtf8(buf);
            //emit errorMsg(msg);
        };
        break;
    default:
        break;
    }
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
