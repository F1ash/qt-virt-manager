#include "vm_viewer.h"

VM_Viewer::VM_Viewer(
        QWidget *parent, virConnect *conn, QString arg1, QString arg2) :
    QMainWindow(parent), jobConnect(conn), connName(arg1), domain(arg2)
{
    setMinimumSize(100, 100);
    setWindowTitle(QString("<%1> Virtual Machine").arg(domain));
    restoreGeometry( settings.value("VMViewerGeometry").toByteArray() );
    VM_State = true;
}
VM_Viewer::~VM_Viewer()
{
    settings.setValue("VMViewerGeometry", saveGeometry());
    if ( jobConnect!=NULL ) {
        virConnectClose(jobConnect);
    };
    VM_State = false;
}

/* public slots */
bool VM_Viewer::isActive() const
{
    return VM_State;
}
void VM_Viewer::stopProcessing()
{
    /*
     * reserved for stop viewer job
     */
    QString msg = QString("'<b>%1</b>' viewer closed.").arg(domain);
    receiveErrMsg(msg);
}
void VM_Viewer::closeEvent(QCloseEvent *ev)
{
    if ( ev->type()==QEvent::Close ) {
        QString msg = QString("'<b>%1</b>' viewer hidden.").arg(domain);
        receiveErrMsg(msg);
        ev->accept();
    }
}
void VM_Viewer::closeViewer()
{
    qDebug()<<domain<<"Job Finished";
    QString msg = QString("'<b>%1</b>' viewer closed.").arg(domain);
    receiveErrMsg(msg);
    //emit finished();
}
void VM_Viewer::receiveErrMsg(QString &msg)
{
    QString time = QTime::currentTime().toString();
    QString title = QString("Connect '%1'").arg(connName);
    QString errMsg = QString("<b>%1 %2:</b><br>%3").arg(time).arg(title).arg(msg);
    emit errorMsg(errMsg);
}

void VM_Viewer::sendConnErrors()
{
    virtErrors = virConnGetLastError(jobConnect);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        QString msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void VM_Viewer::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        QString msg = QString("VirtError(%1) : %2").arg(virtErrors->code)
                .arg(QString().fromUtf8(virtErrors->message));
        emit errorMsg( msg );
    };
    virResetLastError();
}
