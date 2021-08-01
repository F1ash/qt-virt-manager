/*
   Copyright (C) 2014 Lindsay Mathieson (lindsay.mathieson@gmail.com).

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, see <http://www.gnu.org/licenses/>.
*/

#include "qspice-helper.h"
#include "qspice-main-channel.h"

#include <spice/vd_agent.h>


// from spice-util.c
typedef enum {
    NEWLINE_TYPE_LF,
    NEWLINE_TYPE_CR_LF
} NewlineType;

static gssize get_line(const gchar *str,
                       gssize       len,
                       NewlineType  type,
                       gsize       *nl_len,
                       GError     **error)
{
    Q_UNUSED(error)
    const gchar *p, *endl;
    gsize nl = 0;

    endl = (type == NEWLINE_TYPE_CR_LF) ? "\r\n" : "\n";
    p = g_strstr_len(str, len, endl);
    if (p) {
        len = p - str;
        nl = strlen(endl);
    }

    *nl_len = nl;
    return len;
}

static char* spice_convert_newlines(const gchar *str,
                                     gssize      len,
                                     NewlineType from,
                                     NewlineType to,
                                     GError    **error)
{
    GError *err = Q_NULLPTR;
    gssize length;
    gsize nl;
    GString *output;
    gboolean free_segment = FALSE;
    gint i;

    g_return_val_if_fail(str != Q_NULLPTR, Q_NULLPTR);
    g_return_val_if_fail(len >= -1, Q_NULLPTR);
    g_return_val_if_fail(error == Q_NULLPTR || *error == Q_NULLPTR, Q_NULLPTR);
    /* only 2 supported combinations */
    g_return_val_if_fail((from == NEWLINE_TYPE_LF &&
                          to == NEWLINE_TYPE_CR_LF) ||
                         (from == NEWLINE_TYPE_CR_LF &&
                          to == NEWLINE_TYPE_LF), Q_NULLPTR);

    if (len == -1)
        len = long(strlen(str));
    /* sometime we get \0 terminated strings, skip that, or it fails
       to utf8 validate line with \0 end */
    else if (len > 0 && str[len-1] == 0)
        len -= 1;

    /* allocate worst case, if it's small enough, we don't care much,
     * if it's big, malloc will put us in mmap'd region, and we can
     * over allocate.
     */
    output = g_string_sized_new(ulong(len) * 2 + 1);

    for (i = 0; i < len; i += ulong(length) + nl) {
        length = get_line(str + i, len - i, from, &nl, &err);
        if (length < 0)
            break;

        g_string_append_len(output, str + i, length);

        if (nl) {
            /* let's not double \r if it's already in the line */
            if (to == NEWLINE_TYPE_CR_LF &&
                output->str[output->len - 1] != '\r')
                g_string_append_c(output, '\r');

            g_string_append_c(output, '\n');
        }
    }

    if (err) {
        g_propagate_error(error, err);
        free_segment = TRUE;
    }

    return g_string_free(output, free_segment);
}
//  ^^spice-util.c

void QSpiceHelper::main_agent_update(SpiceMainChannel *mainchannel,
                                     gpointer user_data)
{
    Q_UNUSED(mainchannel);

    QSpiceMainChannel *_mainchannel =
            static_cast<QSpiceMainChannel*>(user_data);
    if ( Q_NULLPTR==_mainchannel ) return;
    emit _mainchannel->agentUpdated();
}
void QSpiceHelper::main_clipboard_selection(SpiceMainChannel *mainchannel,
                                            guint selection,
                                            guint type,
                                            gpointer data,
                                            guint size,
                                            gpointer user_data)
{
    Q_UNUSED(mainchannel);
    Q_UNUSED(selection);
    QSpiceMainChannel *_mainchannel =
            static_cast<QSpiceMainChannel*>(user_data);
    if ( Q_NULLPTR==_mainchannel ) return;
    emit _mainchannel->guestClipboardSelectionReceived(type, data, size);
}
void QSpiceHelper::main_clipboard_selection_grab(SpiceMainChannel *mainchannel,
                                                 guint selection,
                                                 gpointer types,
                                                 guint ntypes,
                                                 gpointer user_data)
{
    Q_UNUSED(mainchannel);
    Q_UNUSED(selection);
    Q_UNUSED(types);
    Q_UNUSED(ntypes);
    //qDebug()<<"main_clipboard_selection_grub";
    QSpiceMainChannel *_mainchannel =
            static_cast<QSpiceMainChannel*>(user_data);
    if ( Q_NULLPTR==_mainchannel ) return;
    //emit _mainchannel->clipboardSelectionGrabbed(
    //            selection, (void*)types, ntypes);
}
void QSpiceHelper::main_clipboard_selection_release(SpiceMainChannel *mainchannel,
                                                    guint selection,
                                                    gpointer user_data)
{
    Q_UNUSED(mainchannel)
    //qDebug()<<"main_clipboard_selection_release";
    QSpiceMainChannel *_mainchannel =
            static_cast<QSpiceMainChannel*>(user_data);
    if ( Q_NULLPTR==_mainchannel ) return;
    emit _mainchannel->guestClipboardSelectionReleased(selection);
}
void QSpiceHelper::main_clipboard_selection_request(SpiceMainChannel *mainchannel,
                                                    guint selection,
                                                    guint type,
                                                    gpointer user_data)
{
    Q_UNUSED(mainchannel);
    //qDebug()<<"main_clipboard_selection_request";
    QSpiceMainChannel *_mainchannel =
            static_cast<QSpiceMainChannel*>(user_data);
    if ( Q_NULLPTR==_mainchannel ) return;
    emit _mainchannel->clipboardSelectionRequestedFromGuest(selection, type);
}
void QSpiceHelper::main_mouse_update(SpiceMainChannel *mainchannel,
                                     gpointer user_data)
{
    Q_UNUSED(mainchannel)

    QSpiceMainChannel *_mainchannel =
            static_cast<QSpiceMainChannel*>(user_data);
    if ( Q_NULLPTR==_mainchannel ) return;
    emit _mainchannel->mouseUpdated();
}
void QSpiceHelper::migration_started(SpiceMainChannel *main,
                                     GObject *session,
                                     gpointer user_data)
{
    Q_UNUSED(main)
    Q_UNUSED(session)

    QSpiceMainChannel *_mainchannel =
            static_cast<QSpiceMainChannel*>(user_data);
    if ( Q_NULLPTR==_mainchannel ) return;
    emit _mainchannel->migrationStarted();
}

void QSpiceHelper::operation_cancelled(GCancellable *cancellable,
                                       gpointer user_data)
{
    Q_UNUSED(cancellable)

    QSpiceMainChannel *_mainchannel =
            static_cast<QSpiceMainChannel*>(user_data);
    if ( Q_NULLPTR==_mainchannel ) return;
    emit _mainchannel->cancelled();
}

#if SPICE_GTK_CHECK_VERSION(0, 31, 0)
void QSpiceHelper::new_file_transfer(SpiceMainChannel *mainchannel,
                                     SpiceFileTransferTask *task,
                                     gpointer user_data)
{
    Q_UNUSED(mainchannel)
    QSpiceMainChannel *_mainchannel =
            static_cast<QSpiceMainChannel*>(user_data);
    if ( Q_NULLPTR==_mainchannel ) return;
    QString _fileName = QString::fromUtf8(
            spice_file_transfer_task_get_filename( task ));
    emit _mainchannel->newFileTransfer(_fileName);
    // TODO: implement for each task displaying progress and cancel
    //qDebug()<<"new file transfer:"<<_fileName;
}
#endif

void QSpiceMainChannel::initCallbacks()
{
    //setAgentConnected(true); // not writable, don't use in CONSTRUCT
    //qDebug()<<getAgentConnected()<<"agent connected"; // crash, at debug mode returns FALSE
    //qDebug()<<getMaxClipboard()<<"max-clipboard";
    cancellable = g_cancellable_new();
    g_signal_connect(
                gobject, "main-agent-update",
                GCallback(QSpiceHelper::main_agent_update),
                this);
    g_signal_connect(
                gobject, "main-clipboard-selection",
                GCallback(QSpiceHelper::main_clipboard_selection),
                this);
    g_signal_connect(
                gobject, "main-clipboard-selection-grab",
                GCallback(QSpiceHelper::main_clipboard_selection_grab),
                this);
    g_signal_connect(
                gobject, "main-clipboard-selection-release",
                GCallback(QSpiceHelper::main_clipboard_selection_release),
                this);
    g_signal_connect(
                gobject,
                "main-clipboard-selection-request",
                GCallback(QSpiceHelper::main_clipboard_selection_request),
                this);
    g_signal_connect(
                gobject, "main-mouse-update",
                GCallback(QSpiceHelper::main_mouse_update),
                this);
    g_signal_connect(
                gobject, "migration-started",
                GCallback(QSpiceHelper::migration_started),
                this);
    g_signal_connect(
                cancellable, "cancelled",
                GCallback(QSpiceHelper::operation_cancelled),
                this);
#if SPICE_GTK_CHECK_VERSION(0, 31, 0)
    g_signal_connect(
                gobject, "new-file-transfer",
                GCallback(QSpiceHelper::new_file_transfer),
                this);
#endif
}

void QSpiceMainChannel::setDisplay(int id, int x, int y, int width, int height)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_main_channel_update_display(
                static_cast<SpiceMainChannel*>(gobject),
                id,
                x,
                y,
                width,
                height,
                true);
#else
    spice_main_set_display(
                (SpiceMainChannel *) gobject,
                id,
                x,
                y,
                width,
                height);
#endif
}

void QSpiceMainChannel::setDisplayEnabled(int id, bool enabled)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_main_channel_update_display_enabled(
                static_cast<SpiceMainChannel*>(gobject),
                id,
                enabled,
                true);
#else
    spice_main_set_display_enabled(
                (SpiceMainChannel *) gobject,
                id,
                enabled);
#endif
}

void QSpiceMainChannel::updateDisplay(int id, int x, int y, int width, int height, bool update)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_main_channel_update_display(
#else
    spice_main_update_display(
#endif
                static_cast<SpiceMainChannel*>(gobject),
                id,
                x,
                y,
                width,
                height,
                update);
}

void QSpiceMainChannel::updateDisplayEnabled(int id, bool enabled, bool update)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_main_channel_update_display_enabled(
                static_cast<SpiceMainChannel*>(gobject),
                id,
                enabled,
                update);
#elif SPICE_GTK_CHECK_VERSION(0, 30, 0)
    spice_main_update_display_enabled(
                (SpiceMainChannel *) gobject,
                id,
                enabled,
                update);
#else
    Q_UNUSED(id)
    Q_UNUSED(enabled)
    Q_UNUSED(update)
#endif
}

bool QSpiceMainChannel::sendMonitorConfig()
{
    return
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
           spice_main_channel_send_monitor_config(
#else
           spice_main_send_monitor_config(
#endif
                static_cast<SpiceMainChannel*>(gobject));
}

void QSpiceMainChannel::clipboardSelectionGrab(uint selection, quint32 *types, int ntypes)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_main_channel_clipboard_selection_grab(
#else
    spice_main_clipboard_selection_grab(
#endif
                static_cast<SpiceMainChannel*>(gobject),
                selection,
                types,
                ntypes);
}

void QSpiceMainChannel::initClipboardSelectionRequestOnGuest()
{
    const int ntypes = 5;
    guint32 types[ntypes] = {
            VD_AGENT_CLIPBOARD_UTF8_TEXT,
            VD_AGENT_CLIPBOARD_IMAGE_BMP,
            VD_AGENT_CLIPBOARD_IMAGE_JPG,
            VD_AGENT_CLIPBOARD_IMAGE_PNG,
            VD_AGENT_CLIPBOARD_IMAGE_TIFF};
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_main_channel_clipboard_selection_grab (
#else
    spice_main_clipboard_selection_grab (
#endif
                static_cast<SpiceMainChannel*>(gobject),
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                types,
                ntypes);
}

void QSpiceMainChannel::initClipboardSelectionRequestOnHost()
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_main_channel_clipboard_selection_request(
                static_cast<SpiceMainChannel*>(gobject),
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                VD_AGENT_CLIPBOARD_UTF8_TEXT);
    spice_main_channel_clipboard_selection_request(
                static_cast<SpiceMainChannel*>(gobject),
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                VD_AGENT_CLIPBOARD_IMAGE_PNG);
    spice_main_channel_clipboard_selection_request(
                static_cast<SpiceMainChannel*>(gobject),
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                VD_AGENT_CLIPBOARD_IMAGE_JPG);
    spice_main_channel_clipboard_selection_request(
                static_cast<SpiceMainChannel*>(gobject),
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                VD_AGENT_CLIPBOARD_IMAGE_BMP);
    spice_main_channel_clipboard_selection_request(
                static_cast<SpiceMainChannel*>(gobject),
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                VD_AGENT_CLIPBOARD_IMAGE_TIFF);
#else
    spice_main_clipboard_selection_request(
                (SpiceMainChannel *) gobject,
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                VD_AGENT_CLIPBOARD_UTF8_TEXT);
    spice_main_clipboard_selection_request(
                (SpiceMainChannel *) gobject,
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                VD_AGENT_CLIPBOARD_IMAGE_PNG);
    spice_main_clipboard_selection_request(
                (SpiceMainChannel *) gobject,
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                VD_AGENT_CLIPBOARD_IMAGE_JPG);
    spice_main_clipboard_selection_request(
                (SpiceMainChannel *) gobject,
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                VD_AGENT_CLIPBOARD_IMAGE_BMP);
    spice_main_clipboard_selection_request(
                (SpiceMainChannel *) gobject,
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                VD_AGENT_CLIPBOARD_IMAGE_TIFF);
#endif
}

void QSpiceMainChannel::clipboardSelectionNotify(uint selection, quint32 type, const uchar *data, long size)
{
    //qDebug()<<"clipboardSelectionNotify";

    gpointer conv = Q_NULLPTR;
    uint len = 0;
    if ( type==VD_AGENT_CLIPBOARD_UTF8_TEXT ) {
        /* gtk+ internal utf8 newline is always LF, even on windows */
        if (
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
             spice_main_channel_agent_test_capability(
#else
             spice_main_agent_test_capability(
#endif
                    static_cast<SpiceMainChannel*>(gobject),
                    VD_AGENT_CAP_GUEST_LINEEND_CRLF) ) {
            GError *err = Q_NULLPTR;
            conv = spice_convert_newlines(
                        reinterpret_cast<const char*>(data),
                        size,
                        NEWLINE_TYPE_LF,
                        NEWLINE_TYPE_CR_LF,
                        &err);
            if (err!=Q_NULLPTR) {
                qWarning("Failed to convert text line ending: %s", err->message);
                // continue for paste empty string
                // and to do paste notify for unblock Copy\Paste toolbar buttons
                //return;
            };
            g_clear_error(&err);
            len = strlen(static_cast<char*>(conv));
        } else {
            /* On Windows, with some versions of gtk+, GtkSelectionData::length
             * will include the final '\0'. When a string with this trailing '\0'
             * is pasted in some linux applications, it will be pasted as <NIL> or
             * as an invisible character, which is unwanted. Ensure the length we
             * send to the agent does not include any trailing '\0'
             * This is gtk+ bug https://bugzilla.gnome.org/show_bug.cgi?id=734670
             */
            len = strlen(reinterpret_cast<const char*>(data));
        };
    };

    //qDebug()<<data<<conv<<len;
    const guchar* _conv = static_cast<const guchar*>(conv);
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_main_channel_clipboard_selection_notify(
#else
    spice_main_clipboard_selection_notify(
#endif
                static_cast<SpiceMainChannel*>(gobject),
                selection,
                type,
                _conv ? _conv : data,
                len? len : uint(size));
    g_free(conv);

    clipboardSelectionRelease(type);
}

void QSpiceMainChannel::clipboardSelectionRelease(quint32 type)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_main_channel_clipboard_selection_release(
#else
    spice_main_clipboard_selection_release(
#endif
                static_cast<SpiceMainChannel*>(gobject),
                type);
}

void QSpiceMainChannel::fileCopyAsync(QStringList &fileNames)
{
    if ( fileNames.isEmpty() ) {
        emit downloadCompleted();
        return;
    };
    const uint count = uint(fileNames.count());
    //qDebug()<<fileNames<<count;
    GFile* sources[count];
    uint i = 0;
    foreach (QString _path, fileNames) {
        sources[i] = g_file_new_for_path(_path.toUtf8().constData());
        ++i;
    };
    sources[i] = Q_NULLPTR;
    //qDebug()<<"SpiceMainChannel"<<this->gobject;
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_main_channel_file_copy_async(
#else
    spice_main_file_copy_async(
#endif
                static_cast<SpiceMainChannel*>(gobject),
                sources,
                G_FILE_COPY_BACKUP,
                static_cast<GCancellable*>(cancellable),
                GFileProgressCallback(progressCallback),
                this,
                GAsyncReadyCallback(fileCopyFinish),
                this);
}

void QSpiceMainChannel::fileCopyFinish(void *channel, void *result, void *error)
{
    GAsyncResult *asyncResult = static_cast<GAsyncResult *>(result);
    GError **errors = static_cast<GError**>(error);
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_main_channel_file_copy_finish(
#else
    spice_main_file_copy_finish(
#endif
                static_cast<SpiceMainChannel*>(channel),
                asyncResult,
                errors);

    QSpiceMainChannel *obj = static_cast<QSpiceMainChannel*>(
                g_async_result_get_user_data(asyncResult));
    if ( obj!=Q_NULLPTR) {
        emit obj->downloaded(0, 100);
        SPICE_CHANNEL_MSG _msg;
        _msg.channel = tr("main_channel");
        _msg.context = tr("file transferring finished");
        if ( errors!=Q_NULLPTR && *errors!=Q_NULLPTR ) {
                _msg.msg = QString(tr("Error(%1): %2"))
                        .arg((*errors)->code)
                        .arg(QString::fromUtf8((*errors)->message));
        };
        emit obj->channelMsg(_msg);
        emit obj->downloadCompleted();
        // reset cancellable for allow the start new task
        g_cancellable_reset(static_cast<GCancellable*>(obj->cancellable));
    };
}

void QSpiceMainChannel::progressCallback(qint64 current_num_bytes, qint64 total_num_bytes, void *user_data)
{
    QSpiceMainChannel *obj = static_cast<QSpiceMainChannel*>(user_data);
    //qDebug()<<current_num_bytes<<total_num_bytes;
    if (obj) emit obj->downloaded(current_num_bytes, total_num_bytes);
}

void QSpiceMainChannel::cancelFileCopyAsync()
{
    GCancellable* c = static_cast<GCancellable*>(cancellable);
    if ( !g_cancellable_is_cancelled(c) )
        g_cancellable_cancel(c);
}
