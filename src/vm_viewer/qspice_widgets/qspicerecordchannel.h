#ifndef QSPICERECORDCHANNEL_H
#define QSPICERECORDCHANNEL_H

#include "qspicechannel.h"

class QSpiceRecordChannel : public QSpiceChannel
{
    Q_OBJECT
public:
    void spiceRecord_send_data(void*, size_t, quint32);

signals:
    void recordStart(int, int, int);
    void recordStop();

protected:
    friend class QSpiceHelper;
    inline QSpiceRecordChannel(void *channel) : QSpiceChannel(channel) {initCallbacks();}

    void initCallbacks();
};

#endif // QSPICERECORDCHANNEL_H
