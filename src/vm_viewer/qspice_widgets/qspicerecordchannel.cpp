
#include "qspicehelper.h"
#include "qspicerecordchannel.h"

QSpiceRecordChannel::~QSpiceRecordChannel()
{
    if ( audioInput!=NULL ) {
        qDebug()<<"~QSpiceRecordChannel";
        // close the buffer before audioInput stopped
        if ( _dev && _dev->isOpen() ) {
            _dev->close();
            _dev->remove();
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
    if ( NULL==_record ) return;
    _record->audioFormat.setChannelCount(channels);
    _record->audioFormat.setSampleRate(rate);
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(_record->audioFormat)) {
        qWarning()<<"default format not supported, try to use nearest";
        _record->audioFormat = info.nearestFormat(_record->audioFormat);
    };
    if ( _record->audioInput==NULL ) {
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
    if ( NULL==_record ) return;
    if ( _record->audioInput!=NULL ) {
        _record->audioInput->stop();
        if ( _record->_dev->isOpen() ) {
            _record->_dev->flush();
            _record->spiceRecord_send_data(
                        (void*)_record->_dev->readAll().constData(),
                        _record->_dev->size(),
                        _record->audioInput->processedUSecs());
            _record->_dev->close();
        };
    };
}

void QSpiceRecordChannel::initCallbacks()
{
    g_signal_connect(gobject, "record-start",
                     (GCallback) QSpiceHelper::record_start, this);
    g_signal_connect(gobject, "record-stop",
                     (GCallback) QSpiceHelper::record_stop, this);
}

void QSpiceRecordChannel::spiceRecord_send_data(void *data, size_t bytes, quint32 time)
{
    spice_record_send_data(
                (SpiceRecordChannel*)gobject,
                data,
                bytes,
                time);
}
