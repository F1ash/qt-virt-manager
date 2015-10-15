#ifndef QSPICEPLAYBACKCHANNEL_H
#define QSPICEPLAYBACKCHANNEL_H

#include "qspicechannel.h"

class QSpicePlaybackChannel : public QSpiceChannel
{
    Q_OBJECT
public:

signals:
    void playbackData(void*, int);
    void playbackGetDelay();
    void playbackStart(int, int, int);
    void playbackStop();

protected:
    friend class QSpiceHelper;
    inline QSpicePlaybackChannel(void *channel) : QSpiceChannel(channel) {initCallbacks();}

    void initCallbacks();
};

#endif // QSPICEPLAYBACKCHANNEL_H
