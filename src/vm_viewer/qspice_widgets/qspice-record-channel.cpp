
#include "qspice-helper.h"
#include "qspice-record-channel.h"

QSpiceRecordChannel::~QSpiceRecordChannel()
{
    if ( audioInput!=nullptr ) {
        //qDebug()<<"~QSpiceRecordChannel";
        // close the buffer before audioInput stopped
        if ( _dev ) {
            if ( _dev->isOpen() ) _dev->close();
            bool removed = _dev->remove();
            //qDebug()<<"record_dev_removed"<<removed;
        };
        audioInput->stop();
    };
}

void QSpiceHelper::record_start(SpiceRecordChannel *channel,
                                gint                format,
                                gint                channels,
                                gint                rate,
                                gpointer            user_data)
{
    Q_UNUSED(channel);
    if ( SPICE_AUDIO_FMT_S16!=format ) return;
    QSpiceRecordChannel *_record =
            static_cast<QSpiceRecordChannel*>(user_data);
    if ( nullptr==_record ) return;
    _record->audioFormat.setChannelCount(channels);
    _record->audioFormat.setSampleRate(rate);
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(_record->audioFormat)) {
        qWarning()<<"default format not supported, try to use nearest";
        _record->audioFormat = info.nearestFormat(_record->audioFormat);
    };
    if ( _record->audioInput==nullptr ) {
        _record->audioInput = new QAudioInput(
                    _record->audioFormat, _record);
        qDebug()<<"record_start"<<_record->audioInput;
        if ( _record->_dev && !_record->_dev->isOpen() ) {
            bool opened = _record->_dev->open();
            qDebug()<<"record_dev_opened"<<opened;
        };
        _record->audioInput->start(_record->_dev);
    };
}

void QSpiceHelper::record_stop(SpiceRecordChannel *channel,
                               gpointer            user_data)
{
    Q_UNUSED(channel);
    QSpiceRecordChannel *_record =
            static_cast<QSpiceRecordChannel*>(user_data);
    if ( nullptr==_record ) return;
    if ( _record->audioInput!=nullptr ) {
        _record->audioInput->stop();
        if ( _record->_dev->isOpen() ) {
            _record->_dev->flush();
            _record->spiceRecord_send_data(
                        (void*)_record->_dev->readAll().constData(),
                        _record->_dev->size(),
                        _record->audioInput->processedUSecs());
            _record->_dev->close();
            //qDebug()<<"record_dev_closed";
        };
    };
}

void QSpiceRecordChannel::initCallbacks()
{
#if USE_SPICE_AUDIO
    g_signal_connect(gobject, "record-start",
                     (GCallback) QSpiceHelper::record_start, this);
    g_signal_connect(gobject, "record-stop",
                     (GCallback) QSpiceHelper::record_stop, this);
#endif
}

void QSpiceRecordChannel::spiceRecord_send_data(void *data, size_t bytes, quint32 time)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_record_channel_send_data(
                static_cast<SpiceRecordChannel*>(gobject),
                data,
                bytes,
                time);
#else
    spice_record_send_data(
                (SpiceRecordChannel*)gobject,
                data,
                bytes,
                time);
#endif
}
