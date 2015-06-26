#include "control_thread.h"

ControlThread::ControlThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<Result>("Result");
    qRegisterMetaType<Actions>("Actions");
    number = 0;
}
ControlThread::~ControlThread()
{
    if ( currWorkConnection!=NULL ) {
        // release the reference because no longer required
        int ret = virConnectClose(currWorkConnection);
        //qDebug()<<"virConnectRef -1"<<"ControlThread"<<currConnName<<(ret+1>0)<<number;
        // for reject the multiple releasing the reference
        currWorkConnection = NULL;
    };
    wait(30000);
}

/* public slots */
bool ControlThread::setCurrentWorkConnect(virConnectPtr conn, uint i, QString _name)
{
    keep_alive = true;
    number = i;
    currConnName = _name;
    currWorkConnection = conn;
    // for new virConnect usage create the new virConnectRef[erence]
    int ret = virConnectRef(currWorkConnection);
    if ( ret<0 ) {
        currWorkConnection = NULL;
        sendConnErrors();
        keep_alive = false;
    };
    //qDebug()<<"virConnectRef +1"<<"ControlThread"<<currConnName<<(ret+1>0)<<number;
    //qDebug()<<"net_thread"<<currWorkConnection;
    return keep_alive;
}
void ControlThread::execAction(Actions i, QStringList _str)
{
    return;
}
void ControlThread::stop() { keep_alive = false; }
void ControlThread::run()
{
    return;
}

void ControlThread::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnection);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        QString msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg, number );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void ControlThread::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        QString msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg, number );
    };
    virResetLastError();
}
