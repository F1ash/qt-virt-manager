#ifndef QSPICE_WEBDAV_CHANNEL_H
#define QSPICE_WEBDAV_CHANNEL_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include "qspice-port-channel.h"

class QSpiceWebDAVChannel : public QSpicePortChannel
{
    Q_OBJECT
public:

signals:

protected:
    friend class QSpiceHelper;
    inline QSpiceWebDAVChannel(void *channel) :
        QSpicePortChannel(channel)
    {initCallbacks();}

    void initCallbacks();
};

#endif // QSPICE_WEBDAV_CHANNEL_H
