#ifndef QSPICE_PLAYBACK_CHANNEL_H
#define QSPICE_PLAYBACK_CHANNEL_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>
#include "qspice-channel.h"

class QSpicePlaybackChannel : public QSpiceChannel
{
    Q_OBJECT
public:
    ~QSpicePlaybackChannel();

protected:
    friend class QSpiceHelper;
    inline QSpicePlaybackChannel(void *channel) :
        QSpiceChannel(channel)
    {
        audioFormat.setCodec("audio/pcm");
        audioFormat.setByteOrder(QAudioFormat::LittleEndian);
        audioFormat.setSampleType(QAudioFormat::SignedInt);
        audioFormat.setSampleSize(16);
        audioOutput = Q_NULLPTR;
        _dev = Q_NULLPTR;
        initCallbacks();
    }

    void initCallbacks();

private:
    QIODevice    *_dev;
    QAudioFormat  audioFormat;
    QAudioOutput *audioOutput;
};

#endif // QSPICE_PLAYBACK_CHANNEL_H
