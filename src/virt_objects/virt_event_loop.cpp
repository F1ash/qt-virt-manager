#include "virt_event_loop.h"

VirtEventLoop::VirtEventLoop(QObject *parent) :
    _VirtThread(parent)
{
    success = false;
}

bool VirtEventLoop::isSuccess() const
{
    return success;
}

void VirtEventLoop::run()
{
    if ( virInitialize()==0 ) {
        ulong libVer;
        int ret = virGetVersion(&libVer, nullptr, nullptr);
        if (ret<0) {
            libVersion.append("?.??.???");
        } else {
            success = true;
            ulong maj, min, rel;
            maj = libVer/1000000;
            min = (libVer-maj*1000000)/1000;
            rel = (libVer-maj*1000000-min*1000);
            libVersion = QString("%1.%2.%3")
                    .arg(maj).arg(min).arg(rel);
        };
        int registered = virEventRegisterDefaultImpl();
        keep_alive = (registered==0);
        emit result(keep_alive);
        if ( keep_alive ) {
            while ( keep_alive ) {
                if ( virEventRunDefaultImpl()<0 ) {
                    sendGlobalErrors();
                };
            };
        } else {
            sendGlobalErrors();
        };
    } else {
        sendGlobalErrors();
        libVersion.append("?.??.???");
        emit result(false);
    };
}
