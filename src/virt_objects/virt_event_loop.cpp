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
    keep_alive = (registered==0);
    emit result(keep_alive);
    while ( keep_alive ) {
        if ( virEventRunDefaultImpl() < 0 ) {
            sendGlobalErrors();
        };
    };
}
