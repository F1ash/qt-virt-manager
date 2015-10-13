#ifndef QSPICEWEBDAVCHANNEL_H
#define QSPICEWEBDAVCHANNEL_H

#include "qspiceportchannel.h"

class QSpiceWebDAVChannel : public QSpicePortChannel
{
    Q_OBJECT
public:

signals:

protected:
    friend class QSpiceHelper;
    inline QSpiceWebDAVChannel(void *channel) : QSpicePortChannel(channel) {initCallbacks();}

    void initCallbacks();
};

#endif // QSPICEWEBDAVCHANNEL_H
