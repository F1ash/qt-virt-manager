#include "_virt_thread.h"

_VirtThread::_VirtThread(QObject *parent) :
    QThread(parent)
{
    ptr_ConnPtr = NULL;
    virtErrors  = NULL;
    keep_alive = false;
    number = 0;
}
_VirtThread::~_VirtThread() {}
QString _VirtThread::sendConnErrors()
{
    QString msg;
    virtErrors = (*ptr_ConnPtr)? virConnGetLastError(*ptr_ConnPtr):NULL;
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
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
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
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
