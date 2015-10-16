
#include "qspicehelper.h"
#include "qspicerecordchannel.h"

void QSpiceHelper::record_start(SpiceRecordChannel *channel,
                                gint                format,
                                gint                channels,
                                gint                rate,
                                gpointer            user_data)
{
    Q_UNUSED(channel);
    QSpiceRecordChannel *_record =
            static_cast<QSpiceRecordChannel*>(user_data);
    if ( NULL==_record ) return;
    emit _record->recordStart(format, channels, rate);
}

void QSpiceHelper::record_stop(SpiceRecordChannel *channel,
                               gpointer            user_data)
{
    Q_UNUSED(channel);
    QSpiceRecordChannel *_record =
            static_cast<QSpiceRecordChannel*>(user_data);
    if ( NULL==_record ) return;
    emit _record->recordStop();
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
    spice_record_send_data((SpiceRecordChannel*)gobject,
                           data,
                           bytes,
                           time);
}
