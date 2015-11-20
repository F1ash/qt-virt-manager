
#include "qspicehelper.h"
#include "qspiceplaybackchannel.h"

QSpicePlaybackChannel::~QSpicePlaybackChannel()
{
    if ( audioOutput!=NULL ) {
        //qDebug()<<"~QSpicePlaybackChannel";
        // close the buffer before audioOutput stopped
        if ( _dev && _dev->isOpen() ) _dev->close();
        audioOutput->stop();
    };
}

void QSpiceHelper::playback_data(SpicePlaybackChannel *channel,
                                 gpointer data,
                                 gint data_size,
                                 gpointer user_data)
{
    Q_UNUSED(channel);
    QSpicePlaybackChannel *_playback =
            static_cast<QSpicePlaybackChannel*>(user_data);
    if ( NULL==_playback ) return;
    if ( _playback->audioOutput!=NULL ) {
        if ( _playback->_dev ) {
            _playback->_dev->seek(0);
            qint64 written = _playback->_dev->write((const char*)data, data_size);
            //qDebug()<<data_size<<"playback_data_written"<<written;
        };
    };
}

void QSpiceHelper::playback_get_delay(SpicePlaybackChannel *channel,
                                      gpointer user_data)
{
    Q_UNUSED(channel);
    QSpicePlaybackChannel *_playback =
            static_cast<QSpicePlaybackChannel*>(user_data);
    if ( NULL==_playback ) return;
    if ( _playback->audioOutput!=NULL ) {
        //qDebug()<<"playback_get_delay";
        /*
        if ( _playback->audioOutput->state()==QAudio::ActiveState ) {
            _playback->audioOutput->suspend();
        } else if ( _playback->audioOutput->state()==QAudio::SuspendedState ) {
            _playback->audioOutput->resume();
        };
        */
    };
}
void QSpiceHelper::playback_start(SpicePlaybackChannel *channel,
                                  gint format,
                                  gint channels,
                                  gint rate,
                                  gpointer user_data)
{
    Q_UNUSED(channel);
    if ( SPICE_AUDIO_FMT_S16!=format ) return;
    QSpicePlaybackChannel *_playback =
            static_cast<QSpicePlaybackChannel*>(user_data);
    if ( NULL==_playback ) return;
    _playback->audioFormat.setChannelCount(channels);
    _playback->audioFormat.setSampleRate(rate);
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(_playback->audioFormat)) {
        qWarning()<<"default format not supported, try to use nearest";
        _playback->audioFormat = info.nearestFormat(_playback->audioFormat);
    };
    if ( _playback->audioOutput==NULL ) {
        _playback->audioOutput = new QAudioOutput(
                    _playback->audioFormat, _playback);
        //qDebug()<<"playback_start"<<_playback->audioOutput;
        _playback->_dev = _playback->audioOutput->start();
        if ( _playback->_dev && !_playback->_dev->isOpen() ) {
            bool opened = _playback->_dev->open(QIODevice::WriteOnly);
            //qDebug()<<"playback_dev_opened"<<opened;
        };
    };
}

void QSpiceHelper::playback_stop(SpicePlaybackChannel *channel,
                                 gpointer user_data)
{
    Q_UNUSED(channel);
    QSpicePlaybackChannel *_playback =
            static_cast<QSpicePlaybackChannel*>(user_data);
    if ( NULL==_playback ) return;
    //qDebug()<<"playback_stop";
}

void QSpicePlaybackChannel::initCallbacks()
{
    g_signal_connect(gobject, "playback-data",
                     (GCallback) QSpiceHelper::playback_data, this);
    g_signal_connect(gobject, "playback-get-delay",
                     (GCallback) QSpiceHelper::playback_get_delay, this);
    g_signal_connect(gobject, "playback-start",
                     (GCallback) QSpiceHelper::playback_start, this);
    g_signal_connect(gobject, "playback-stop",
                     (GCallback) QSpiceHelper::playback_stop, this);
}

