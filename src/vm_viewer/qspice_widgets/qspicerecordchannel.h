#ifndef QSPICERECORDCHANNEL_H
#define QSPICERECORDCHANNEL_H

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioInput>
#include <QTemporaryFile>
#include "qspicechannel.h"

class QSpiceRecordChannel : public QSpiceChannel
{
    Q_OBJECT
public:
    ~QSpiceRecordChannel();

protected:
    friend class QSpiceHelper;
    inline QSpiceRecordChannel(void *channel) : QSpiceChannel(channel)
    {
        audioFormat.setCodec("audio/pcm");
        audioFormat.setByteOrder(QAudioFormat::LittleEndian);
        audioFormat.setSampleType(QAudioFormat::Float);
        audioFormat.setSampleSize(16);
        audioInput = nullptr;
        _dev = new QTemporaryFile(this);
        _dev->setAutoRemove(false);
        initCallbacks();
    }

    void initCallbacks();

private:
    QTemporaryFile  *_dev;
    QAudioFormat     audioFormat;
    QAudioInput     *audioInput;
    void             spiceRecord_send_data(void*, size_t, quint32);
};

#endif // QSPICERECORDCHANNEL_H
