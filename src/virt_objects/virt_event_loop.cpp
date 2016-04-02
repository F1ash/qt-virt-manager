#include "virt_event_loop.h"

VirtEventLoop::VirtEventLoop(QObject *parent) :
    _VirtThread(parent)
{
}

void VirtEventLoop::run()
{
    int registered = -1;
    if ( virInitialize()+1 ) {
        registered = virEventRegisterDefaultImpl();
    };
    ulong libVer;
    int ret = virConnectGetLibVersion(
                virConnectOpenReadOnly(nullptr),
                &libVer);
    if (ret<0) {
        libVersion.append("?.??.???");
    } else {
        uint maj, min, rel;
        maj = libVer/1000000;
        min = (libVer-maj*1000000)/1000;
        rel = (libVer-maj*1000000-min*1000);
        libVersion = QString("%1.%2.%3")
                .arg(maj).arg(min).arg(rel);
    };
    keep_alive = (registered==0);
    emit result(keep_alive);
    while ( keep_alive ) {
        if ( virEventRunDefaultImpl() < 0 ) {
            sendGlobalErrors();
        };
    };
}
