#include "_virt_thread.h"

_VirtThread::_VirtThread(QObject *parent, virConnectPtr *connPtrPtr) :
    QThread(parent), ptr_ConnPtr(connPtrPtr)
{
    virtErrors  = nullptr;
    keep_alive = false;
    number = 0;
}
_VirtThread::~_VirtThread() {}
QString _VirtThread::sendConnErrors()
{
    QString msg;
    virtErrors = (nullptr!=ptr_ConnPtr && *ptr_ConnPtr)?
                virConnGetLastError(*ptr_ConnPtr):nullptr;
    if ( virtErrors!=nullptr && virtErrors->code>0 ) {
        msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg, number );
        virResetError(virtErrors);
    } else
        msg = sendGlobalErrors();
    return msg;
}
QString _VirtThread::sendGlobalErrors()
{
    QString msg;
    virtErrors = virGetLastError();
    if ( virtErrors!=nullptr && virtErrors->code>0 ) {
        msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg, number );
    };
    virResetLastError();
    return msg;
}

/* public slots */
void _VirtThread::stop()
{
    keep_alive = false;
}
