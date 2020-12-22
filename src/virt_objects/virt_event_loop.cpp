#include "virt_event_loop.h"
//#include <QDebug>

VirtEventLoop::VirtEventLoop(QObject *parent) :
    _VirtThread(parent)
{
    success = false;
    timer_id = 0;
}

bool VirtEventLoop::isSuccess() const
{
    return success;
}

void VirtEventLoop::run()
{
    if ( virInitialize()==0 ) {
        ulong libVer;
        int ret = virGetVersion(&libVer, Q_NULLPTR, Q_NULLPTR);
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
        timer_id = virEventAddTimeout(1000, callback, this, freeCallback);
        keep_alive = (timer_id>0) && (registered==0);
        emit result(keep_alive);
        //qDebug() << "keep_alive_0 "<< keep_alive;
        if ( keep_alive ) {
            while ( keep_alive ) {
                //qDebug() << "keep_alive_1 "<< keep_alive;
                if ( virEventRunDefaultImpl()<0 ) {
                    sendGlobalErrors();
                };
                //qDebug() << "next_event_wait";
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

void VirtEventLoop::callback(int _t, void *op)
{
    Q_UNUSED(op);
    Q_UNUSED(_t);
    //VirtEventLoop *obj = static_cast<VirtEventLoop*>(op);
    //qDebug() << "next_timeout_event: keep_alive_3 = "
    //         << obj->keep_alive;
}

void VirtEventLoop::freeCallback(void *op)
{
    VirtEventLoop *obj = static_cast<VirtEventLoop*>(op);
    obj->timer_id = 0;
    obj->keep_alive = false;
    //qDebug() << "keep_alive_4 = " << obj->keep_alive;
    emit obj->timerStopped();
}

void VirtEventLoop::stop()
{
    //qDebug()<<"stop virt Event loop";
    if ( timer_id ) {
        virEventUpdateTimeout(timer_id, -1);
        //qDebug()<<"stopped virt Event timer";
        virEventRemoveTimeout(timer_id);
        //qDebug()<<"removed virt Event timer";
    };
}
