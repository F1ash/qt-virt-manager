#ifndef QSPICE_PORT_CHANNEL_H
#define QSPICE_PORT_CHANNEL_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include "qspice-channel.h"

class QSpicePortChannel : public QSpiceChannel
{
    Q_OBJECT
public:
    Q_GPROP_STR(PortName, "port-name")          // Default value: NULL
    Q_GPROP_BOOL(PortOpened, "port-opened")     // Default value: FALSE

signals:
    void portData(void*, size_t);
    void portEvent(int);

protected:
    inline QSpicePortChannel(void *channel) :
        QSpiceChannel(channel)
    {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();
    void sendEventToPort(quint8);
    void writeBuffToPort(const void*, size_t);
    static void writeFinishToPort(void*, void*, void*);
};

#endif // QSPICE_PORT_CHANNEL_H
