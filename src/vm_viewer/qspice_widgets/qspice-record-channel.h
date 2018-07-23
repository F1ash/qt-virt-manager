#ifndef QSPICE_RECORD_CHANNEL_H
#define QSPICE_RECORD_CHANNEL_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioInput>
#include <QTemporaryFile>
#include "qspice-channel.h"

class QSpiceRecordChannel : public QSpiceChannel
{
    Q_OBJECT
public:
    ~QSpiceRecordChannel();

protected:
    friend class QSpiceHelper;
    inline QSpiceRecordChannel(void *channel) :
        QSpiceChannel(channel)
    {
        audioFormat.setCodec("audio/pcm");
        audioFormat.setByteOrder(QAudioFormat::LittleEndian);
        audioFormat.setSampleType(QAudioFormat::Float);
        audioFormat.setSampleSize(16);
        audioInput = Q_NULLPTR;
        _dev = new QTemporaryFile(this);
        _dev->setAutoRemove(false);
        initCallbacks();
    }

    void initCallbacks();

private:
    QTemporaryFile  *_dev;
    QAudioFormat     audioFormat;
    QAudioInput     *audioInput;
    void             spiceRecord_send_data(const char*, qint64, qint64);
};

#endif // QSPICE_RECORD_CHANNEL_H
