#ifndef QSPICESMARTCARDCHANNEL_H
#define QSPICESMARTCARDCHANNEL_H

#include "qspicechannel.h"

class QSpiceSmartcardChannel : public QSpiceChannel
{
    Q_OBJECT
public:

signals:

protected:
    inline QSpiceSmartcardChannel(void *channel) : QSpiceChannel(channel) {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();
};

#endif // QSPICESMARTCARDCHANNEL_H
