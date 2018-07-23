
#include "qspice-helper.h"
#include "qspice-port-channel.h"

void QSpiceHelper::port_data(SpicePortChannel *spiceportchannel,
                             gpointer          arg1,
                             gint              arg2,
                             gpointer          user_data)
{
    // emitted when data is available for receiving from port
    Q_UNUSED(spiceportchannel)
    QSpicePortChannel *_port = static_cast<QSpicePortChannel*>(user_data);
    if ( Q_NULLPTR==_port ) return;
    emit _port->portData(arg1, size_t(arg2));
}

void QSpiceHelper::port_event(SpicePortChannel *spiceportchannel,
                              gint              arg1,
                              gpointer          user_data)
{
    Q_UNUSED(spiceportchannel)
    QSpicePortChannel *_port = static_cast<QSpicePortChannel*>(user_data);
    if ( Q_NULLPTR==_port ) return;
    emit _port->portEvent(arg1);
}

void QSpicePortChannel::initCallbacks()
{
    g_signal_connect(gobject, "port-data",
                     GCallback(QSpiceHelper::port_data), this);
    g_signal_connect(gobject, "port-event",
                     GCallback(QSpiceHelper::port_event), this);
}

void QSpicePortChannel::sendEventToPort(quint8 event)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_port_channel_event(
#else
    spice_port_event(
#endif
                static_cast<SpicePortChannel*>(gobject),
                guint8(event));
}

void QSpicePortChannel::writeBuffToPort(const void *buff, size_t _size)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_port_channel_write_async(
#else
    spice_port_write_async(
#endif
                static_cast<SpicePortChannel*>(gobject),
                buff,
                _size,
                Q_NULLPTR,
                GAsyncReadyCallback(writeFinishToPort),
                this);
}

void QSpicePortChannel::writeFinishToPort(void *_port, void *_res, void *_errs)
{
    GAsyncResult *result = static_cast<GAsyncResult*>(_res);
    GError **errors = static_cast<GError**>(_errs);
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    long _size = spice_port_channel_write_finish(
#else
    long _size = spice_port_write_finish(
#endif
                static_cast<SpicePortChannel*>(_port),
                result,
                errors);

    QSpicePortChannel *obj = static_cast<QSpicePortChannel*>(
                g_async_result_get_user_data(result));
    if ( obj!=Q_NULLPTR) {
        SPICE_CHANNEL_MSG _msg;
        _msg.channel = tr("port");
        _msg.context = tr("writing data finished");
        if ( errors!=Q_NULLPTR && *errors!=Q_NULLPTR ) {
                _msg.msg = QString(tr("Error(%1): %2"))
                        .arg((*errors)->code)
                        .arg(QString::fromUtf8((*errors)->message));
        } else {
            _msg.msg = QString(tr("Sent %1 bytes")).arg(_size);
        };
        emit obj->channelMsg(_msg);
    };
}
