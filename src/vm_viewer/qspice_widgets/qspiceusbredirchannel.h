#ifndef QSPICEUSBREDIRCHANNEL_H
#define QSPICEUSBREDIRCHANNEL_H

#include "qspicechannel.h"

class QSpiceUSBRedirChannel : public QSpiceChannel
{
    Q_OBJECT
public:

signals:

protected:
    inline QSpiceUSBRedirChannel(void *channel) : QSpiceChannel(channel) {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();
};

#endif // QSPICEUSBREDIRCHANNEL_H
