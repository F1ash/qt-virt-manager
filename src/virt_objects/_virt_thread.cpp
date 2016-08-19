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
    if ( virtErrors!=nullptr && virtErrors->code>0 ) {
        msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        //emit errorMsg( msg, number );
        customErrorNotification( msg, virtErrors->code );
    };
    virResetLastError();
    return msg;
}
void _VirtThread::customErrorNotification(const QString &_msg, const uint code)
{
    QString msg;
    switch ( code ) {
    case 38:
        msg = QString("%1 : [ <font color='red'>WARNING:</font> %2 ]")
                .arg(_msg)
                .arg(
    "Make sure the required Libvirt or another service is installed and running");
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
