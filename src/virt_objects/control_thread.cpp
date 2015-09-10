#include "control_thread.h"

ControlThread::ControlThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<Result>("Result");
    qRegisterMetaType<Actions>("Actions");
    number = 0;
    currConnPtr = NULL;
    keep_alive = false;
}
ControlThread::~ControlThread()
{
    if ( currConnPtr!=NULL ) {
        // release the reference because no longer required
        int ret = virConnectClose(*currConnPtr);
        //qDebug()<<"virConnectRef -1"<<"ControlThread"<<currConnName<<(ret+1>0)<<number;
        // for reject the multiple releasing the reference
        currConnPtr = NULL;
    };
    wait(30000);
}

/* public slots */
bool ControlThread::setCurrentWorkConnect(
        virConnectPtr *connPtr, uint i, QString _name)
{
    keep_alive = true;
    number = i;
    currConnName = _name;
    currConnPtr = connPtr;
    // for new virConnect usage create the new virConnectRef[erence]
    int ret = virConnectRef(*currConnPtr);
    if ( ret<0 ) {
        currConnPtr = NULL;
        sendConnErrors();
        keep_alive = false;
    };
    //qDebug()<<"virConnectRef +1"<<"ControlThread"<<currConnName<<(ret+1>0)<<number;
    //qDebug()<<"net_thread"<<currConnPtr;
    return keep_alive;
}
void ControlThread::execAction(uint _number, TASK task)
{
    Q_UNUSED(_number); Q_UNUSED(task);
}
void ControlThread::stop() { keep_alive = false; }
void ControlThread::run()
{
    return;
}

QString ControlThread::sendConnErrors()
{
    QString msg;
    virtErrors = virConnGetLastError(*currConnPtr);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg, number );
        virResetError(virtErrors);
    } else
        msg = sendGlobalErrors();
    return msg;
}
QString ControlThread::sendGlobalErrors()
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
