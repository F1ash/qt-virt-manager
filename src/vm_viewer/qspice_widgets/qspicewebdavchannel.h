#ifndef QSPICEWEBDAVCHANNEL_H
#define QSPICEWEBDAVCHANNEL_H

#include "qspicechannel.h"

class QSpiceWebDAVChannel : public QSpiceChannel
{
    Q_OBJECT
public:

signals:

protected:
    inline QSpiceWebDAVChannel(void *channel) : QSpiceChannel(channel) {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();
};

#endif // QSPICEWEBDAVCHANNEL_H
