#ifndef QSPICEPLAYBACKCHANNEL_H
#define QSPICEPLAYBACKCHANNEL_H

#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>
#include <QIODevice>
#include "qspicechannel.h"

class QSpicePlaybackChannel : public QSpiceChannel
{
    Q_OBJECT
public:

protected:
    friend class QSpiceHelper;
    inline QSpicePlaybackChannel(void *channel) : QSpiceChannel(channel)
    {
        audioFormat.setCodec("audio/pcm");
        audioFormat.setByteOrder(QAudioFormat::LittleEndian);
        audioFormat.setSampleType(QAudioFormat::Float);
        audioFormat.setSampleSize(16);
        audioOutput = NULL;
        _dev = NULL;
        initCallbacks();
    }
    ~QSpicePlaybackChannel();

    void initCallbacks();

private:
    QIODevice    *_dev;
    QAudioFormat  audioFormat;
    QAudioOutput *audioOutput;
};

#endif // QSPICEPLAYBACKCHANNEL_H
