#include "virt_event_loop.h"

VirtEventLoop::VirtEventLoop(QObject *parent) :
    _VirtThread(parent)
{
}

void VirtEventLoop::run()
{
    keep_alive = true;
    while ( keep_alive ) {
        if ( virEventRunDefaultImpl() < 0 ) {
            sendGlobalErrors();
        };
    };
}
