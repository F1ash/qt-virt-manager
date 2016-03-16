#ifndef QSPICE_USBREDIR_CHANNEL_H
#define QSPICE_USBREDIR_CHANNEL_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include "qspice-channel.h"

class QSpiceUSBRedirChannel : public QSpiceChannel
{
    Q_OBJECT
public:

signals:

protected:
    inline QSpiceUSBRedirChannel(void *channel) :
        QSpiceChannel(channel)
    {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();
};

#endif // QSPICE_USBREDIR_CHANNEL_H
