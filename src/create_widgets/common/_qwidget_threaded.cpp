#include "_qwidget_threaded.h"

qwdHelpThread::qwdHelpThread(
        QObject         *parent,
        virConnectPtr   *connPtrPtr) :
    _VirtThread(parent, connPtrPtr)
{

}
void qwdHelpThread::run()
{
    if ( nullptr==ptr_ConnPtr || nullptr==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };
    connType = QString::fromUtf8(
                virConnectGetType(*ptr_ConnPtr))
            .toLower();
    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
}

_QWidget_Threaded::_QWidget_Threaded(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr,
        virDomainPtr     domain) :
    _QWidget(parent, connPtrPtr, domain)
{
    hlpThread = new qwdHelpThread(this, connPtrPtr);
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(init_wdg()));
}

void _QWidget_Threaded::init_wdg()
{

}
