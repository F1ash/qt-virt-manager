#include "_virt_thread.h"

_VirtThread::_VirtThread(
        QObject         *parent,
        virConnectPtr   *connPtrPtr) :
    QThread(parent), ptr_ConnPtr(connPtrPtr)
{
    virtErrors  = Q_NULLPTR;
    keep_alive = false;
    number = 0;
}
_VirtThread::~_VirtThread() {}
QString _VirtThread::sendConnErrors()
{
    QString msg;
    virtErrors = (Q_NULLPTR!=ptr_ConnPtr && *ptr_ConnPtr)?
                virConnGetLastError(*ptr_ConnPtr):Q_NULLPTR;
    if ( virtErrors!=Q_NULLPTR && virtErrors->code>0 ) {
        msg = QString("VirtError(%1) : %2")
                .arg(virtErrors->code)
                .arg(QString::fromUtf8(virtErrors->message));
        //emit errorMsg( msg, number );
        customErrorNotification( msg, virtErrors->code );
        virResetError(virtErrors);
    } else
        msg = sendGlobalErrors();
    return msg;
}
QString _VirtThread::sendGlobalErrors()
{
    QString msg;
    virtErrors = virGetLastError();
    if ( virtErrors!=Q_NULLPTR && virtErrors->code>0 ) {
        msg = QString("VirtError(%1) : %2")
                .arg(virtErrors->code)
                .arg(QString::fromUtf8(virtErrors->message));
        //emit errorMsg( msg, number );
        customErrorNotification( msg, virtErrors->code );
    };
    virResetLastError();
    return msg;
}
void _VirtThread::customErrorNotification(const QString &_msg, const int code)
{
    QString msg;
    switch ( code ) {
    case 38:
        msg = QString(
                tr("%1 : [ <font color='red'>WARNING:</font> %2 ]"))
                .arg(_msg)
                .arg(
    tr("Make sure the required Libvirt or another service is installed and running"));
        break;
    default:
        msg = _msg;
        return;
    };
    emit errorMsg( msg, number );
}

/* public slots */
void _VirtThread::stop()
{
    keep_alive = false;
}
