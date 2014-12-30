#include "control_thread.h"

ControlThread::ControlThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<Result>("Result");
}

/* public slots */
bool ControlThread::setCurrentWorkConnect(virConnectPtr conn, uint i)
{
    keep_alive = true;
    number = i;
    currWorkConnect = conn;
    //qDebug()<<"net_thread"<<currWorkConnect;
}
void ControlThread::execAction(int i, QStringList _str) { }
void ControlThread::stop() { keep_alive = false; }

/* private slots */
void ControlThread::run()
{
    return;
}

void ControlThread::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnect);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void ControlThread::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL && virtErrors->code>0 )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
    virResetLastError();
}
