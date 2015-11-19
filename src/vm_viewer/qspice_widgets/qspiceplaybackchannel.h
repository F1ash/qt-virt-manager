#ifndef QSPICEPLAYBACKCHANNEL_H
#define QSPICEPLAYBACKCHANNEL_H

#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>
#include "qspicechannel.h"

class QSpicePlaybackChannel : public QSpiceChannel
{
    Q_OBJECT
public:

signals:
    /*
    void playbackData(void*, int);
    void playbackGetDelay();
    void playbackStart(int, int, int);
    void playbackStop();
    */

protected:
    friend class QSpiceHelper;
    inline QSpicePlaybackChannel(void *channel) : QSpiceChannel(channel)
    {
        audioFormat.setCodec("audio/pcm");
        audioOutput = NULL;
        initCallbacks();
    }

    void initCallbacks();

private:
    QAudioFormat  audioFormat;
    QAudioOutput *audioOutput;
};

#endif // QSPICEPLAYBACKCHANNEL_H
