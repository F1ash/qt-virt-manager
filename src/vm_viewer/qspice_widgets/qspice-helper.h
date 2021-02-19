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


#ifndef QSPICE_HELPER_H
#define QSPICE_HELPER_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif


#include <spice-client.h>

//#include <QDebug>

class QSpiceHelper
{
public:
    explicit QSpiceHelper();

    // Session callbacks
    static void ss_channel_new(
            SpiceSession        *session,
            SpiceChannel        *channel,
            gpointer             user_data);

    static void ss_channel_destroy(
            SpiceSession        *session,
            SpiceChannel        *channel,
            gpointer             user_data);

    // Spice client Channels
    static void GDisposeEvent(
            gpointer             data,
            GObject             *_channel);

    static void channel_event(
            SpiceChannel        *channel,
            SpiceChannelEvent    event,
            gpointer             user_data);

    // Display Channel
    static void display_primary_create(
            SpiceDisplayChannel *display,
            gint                 format,
            gint                 width,
            gint                 height,
            gint                 stride,
            gint                 shmid,
            gpointer             imgdata,
            gpointer             user_data);

    static void display_primary_destroy(
            SpiceDisplayChannel *display,
            gpointer             user_data);

    static void display_invalidate(
            SpiceDisplayChannel *display,
            gint                 x,
            gint                 y,
            gint                 width,
            gint                 height,
            gpointer             user_data);

    static void display_mark(
            SpiceDisplayChannel *display,
            gint                 mark,
            gpointer             user_data);

    // Inputs Channel
    static void inputs_modifiers(
            SpiceInputsChannel  *spiceinputschannel,
            gpointer             user_data);

    // Cursor Channel
    static void cursor_set(
            SpiceCursorChannel  *cursor,
            gint                 width,
            gint                 height,
            gint                 hot_x,
            gint                 hot_y,
            gpointer             rgba,
            gpointer             user_data);

    // Main Channel
    static void main_agent_update(
            SpiceMainChannel    *mainchannel,
            gpointer             user_data);

    static void main_clipboard_selection(
            SpiceMainChannel    *mainchannel,
            guint                selection,
            guint                type,
            gpointer             data,
            guint                size,
            gpointer             user_data);

    static void main_clipboard_selection_grab(
            SpiceMainChannel    *mainchannel,
            guint                selection,
            gpointer             types,
            guint                ntypes,
            gpointer             user_data);

    static void main_clipboard_selection_release(
            SpiceMainChannel    *mainchannel,
            guint                selection,
            gpointer             user_data);

    static void main_clipboard_selection_request(
            SpiceMainChannel    *mainchannel,
            guint                selection,
            guint                type,
            gpointer             user_data);

    static void main_mouse_update(
            SpiceMainChannel    *mainchannel,
            gpointer             user_data);

    static void migration_started(
            SpiceMainChannel    *mainchannel,
            GObject             *session,
            gpointer             user_data);

    static void operation_cancelled(
            GCancellable        *cancellable,
            gpointer             user_data);

#if SPICE_GTK_CHECK_VERSION(0, 31, 0)
    static void new_file_transfer(
            SpiceMainChannel      *mainchannel,
            SpiceFileTransferTask *task,
            gpointer               user_data);
#endif

    // USB Device Manager
    static void auto_connect_failed(
            SpiceUsbDeviceManager *manager,
            SpiceUsbDevice        *device,
            GError                *error,
            gpointer               user_data);

    static void device_added(
            SpiceUsbDeviceManager *manager,
            SpiceUsbDevice        *device,
            gpointer               user_data);

    static void device_error(
            SpiceUsbDeviceManager *manager,
            SpiceUsbDevice        *device,
            GError                *error,
            gpointer               user_data);

    static void device_removed(
            SpiceUsbDeviceManager *manager,
            SpiceUsbDevice        *device,
            gpointer               user_data);

    // Smartcard Manager
    static void card_inserted(
            SpiceSmartcardManager *manager,
            SpiceSmartcardReader  *reader,
            gpointer               user_data);

    static void card_removed(
            SpiceSmartcardManager *manager,
            SpiceSmartcardReader  *reader,
            gpointer               user_data);

    static void reader_added(
            SpiceSmartcardManager *manager,
            SpiceSmartcardReader  *reader,
            gpointer               user_data);

    static void reader_removed(
            SpiceSmartcardManager *manager,
            SpiceSmartcardReader  *reader,
            gpointer               user_data);

    // Port Channel
    static void port_data(
            SpicePortChannel    *spiceportchannel,
            gpointer             arg1,
            gint                 arg2,
            gpointer             user_data);

    static void port_event(
            SpicePortChannel    *spiceportchannel,
            gint                 arg1,
            gpointer             user_data);

    // Playback Channel
    static void playback_data(
            SpicePlaybackChannel *channel,
            gpointer              data,
            gint                  data_size,
            gpointer              user_data);

    static void playback_get_delay(
            SpicePlaybackChannel *channel,
            gpointer              user_data);

    static void playback_start(
            SpicePlaybackChannel *channel,
            gint                  format,
            gint                  channels,
            gint                  rate,
            gpointer              user_data);

    static void playback_stop(
            SpicePlaybackChannel *channel,
            gpointer              user_data);

    // Record Channel
    static void record_start(
            SpiceRecordChannel  *channel,
            gint                 format,
            gint                 channels,
            gint                 rate,
            gpointer             user_data);

    static void record_stop(
            SpiceRecordChannel  *channel,
            gpointer             user_data);

};

#endif // QSPICE_HELPER_H
