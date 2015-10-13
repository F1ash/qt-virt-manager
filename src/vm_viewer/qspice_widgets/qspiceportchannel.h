#ifndef QSPICEPORTCHANNEL_H
#define QSPICEPORTCHANNEL_H

#include "qspicechannel.h"

class QSpicePortChannel : public QSpiceChannel
{
    Q_OBJECT
public:
    Q_GPROP_STR(PortName, "port-name")          // Default value: NULL
    Q_GPROP_BOOL(PortOpened, "port-opened")     // Default value: FALSE

signals:
    void portData(void*,size_t);
    void portEvent(qint8);

protected:
    inline QSpicePortChannel(void *channel) : QSpiceChannel(channel) {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();
    void sendEventToPort(qint8);
    void writeBuffToPort(const void*, size_t);
    static void writeFinishToPort(void*, void*, void*);
};

#endif // QSPICEPORTCHANNEL_H
