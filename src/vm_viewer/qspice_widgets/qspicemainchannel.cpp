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

#include "qspicehelper.h"
#include "qspicemainchannel.h"


void QSpiceHelper::main_agent_update(SpiceMainChannel *spicemainchannel, gpointer user_data)
{
    Q_UNUSED(spicemainchannel)

    QSpiceMainChannel *_mainchannel = static_cast<QSpiceMainChannel*>(user_data);
    if ( NULL==_mainchannel ) return;
    emit _mainchannel->main_AgentUpdate();
}
void QSpiceHelper::main_clipboard_selection(SpiceMainChannel *spicemainchannel,
                                            guint selection,
                                            guint type,
                                            gpointer data,
                                            guint size,
                                            gpointer user_data)
{
    Q_UNUSED(spicemainchannel)
    Q_UNUSED(selection)
    QSpiceMainChannel *_mainchannel = static_cast<QSpiceMainChannel*>(user_data);
    if ( NULL==_mainchannel ) return;
    emit _mainchannel->main_ClipboardSelection(type, (void*)data, size);
}
void QSpiceHelper::main_clipboard_selection_grab(SpiceMainChannel *spicemainchannel,
                                                 guint selection,
                                                 gpointer types,
                                                 guint ntypes,
                                                 gpointer user_data)
{
    Q_UNUSED(spicemainchannel)

    QSpiceMainChannel *_mainchannel = static_cast<QSpiceMainChannel*>(user_data);
    if ( NULL==_mainchannel ) return;
    emit _mainchannel->main_ClipboardSelectionGrab(selection, (void*)types, ntypes);
}
void QSpiceHelper::main_clipboard_selection_release(SpiceMainChannel *spicemainchannel,
                                                    guint selection,
                                                    gpointer user_data)
{
    Q_UNUSED(spicemainchannel)

    QSpiceMainChannel *_mainchannel = static_cast<QSpiceMainChannel*>(user_data);
    if ( NULL==_mainchannel ) return;
    emit _mainchannel->main_ClipboardSelectionRelease(selection);
}
void QSpiceHelper::main_clipboard_selection_request(SpiceMainChannel *spicemainchannel,
                                                    guint selection,
                                                    guint types,
                                                    gpointer user_data)
{
    Q_UNUSED(spicemainchannel)

    QSpiceMainChannel *_mainchannel = static_cast<QSpiceMainChannel*>(user_data);
    if ( NULL==_mainchannel ) return;
    emit _mainchannel->main_ClipboardSelectionRequest(selection, types);
}
void QSpiceHelper::main_mouse_update(SpiceMainChannel *spicemainchannel, gpointer user_data)
{
    Q_UNUSED(spicemainchannel)

    QSpiceMainChannel *_mainchannel = static_cast<QSpiceMainChannel*>(user_data);
    if ( NULL==_mainchannel ) return;
    emit _mainchannel->main_MouseUpdate();
}

void QSpiceMainChannel::initCallbacks()
{
    g_signal_connect(gobject, "main-agent-update",
                     (GCallback) QSpiceHelper::main_agent_update, this);
    g_signal_connect(gobject, "main-clipboard-selection",
                     (GCallback) QSpiceHelper::main_clipboard_selection, this);
    //g_signal_connect(gobject, "main-clipboard-selection-grab",
    //                 (GCallback) QSpiceHelper::main_clipboard_selection_grab, this);
    //g_signal_connect(gobject, "main-clipboard-selection-release",
    //                 (GCallback) QSpiceHelper::main_clipboard_selection_release, this);
    g_signal_connect(gobject, "main-clipboard-selection-request",
                     (GCallback) QSpiceHelper::main_clipboard_selection_request, this);
    g_signal_connect(gobject, "main-mouse-update",
                     (GCallback) QSpiceHelper::main_mouse_update, this);
}

void QSpiceMainChannel::mainSetDisplay(int id, int x, int y, int width, int height)
{
    spice_main_set_display((SpiceMainChannel *) gobject, id, x, y, width, height);
}

void QSpiceMainChannel::mainSetDisplayEnabled(int id, bool enabled)
{
    spice_main_set_display_enabled((SpiceMainChannel *) gobject, id, enabled);
}

bool QSpiceMainChannel::mainSendMonitorConfig()
{
    return spice_main_send_monitor_config((SpiceMainChannel *) gobject);
}

void QSpiceMainChannel::mainClipboardSelectionGrab(uint selection, quint32 *types, int ntypes)
{
    spice_main_clipboard_selection_grab((SpiceMainChannel *) gobject, selection, types, ntypes);
}

void QSpiceMainChannel::mainClipboardSelectionRelease()
{
    spice_main_clipboard_selection_release(
                (SpiceMainChannel *) gobject,
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD);
}

void QSpiceMainChannel::mainClipboardSelectionNotify(uint type, const uchar *data, uint size)
{
    //qDebug()<<"mainClipboardSelectionNotify";
    spice_main_clipboard_selection_notify(
                (SpiceMainChannel *) gobject,
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                type,
                data,
                size);
}

void QSpiceMainChannel::mainClipboardSelectionRequest()
{
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
}

void QSpiceMainChannel::mainFileCopyAsync(QStringList &fileNames)
{
    if ( fileNames.isEmpty() ) return;
    uint count = fileNames.count();
    //qDebug()<<fileNames<<count;
    GFile* sources[count];
    uint i = 0;
    foreach (QString _path, fileNames) {
        sources[i] = g_file_new_for_path(_path.toUtf8().constData());
        i++;
    };
    sources[i] = NULL;
    //qDebug()<<"SpiceMainChannel"<<this->gobject;
    GCancellable *cancellable = g_cancellable_new();
    spice_main_file_copy_async(
                (SpiceMainChannel *) gobject,
                sources,
                G_FILE_COPY_BACKUP,
                cancellable,
                (GFileProgressCallback)progressCallback,
                this,
                (GAsyncReadyCallback)mainFileCopyFinish,
                this);
}

void QSpiceMainChannel::mainFileCopyFinish(void *channel, void *result, void *error)
{
    GAsyncResult *asyncResult = (GAsyncResult *)result;
    GError **errors = (GError **)error;
    spice_main_file_copy_finish(
                (SpiceMainChannel *)channel,
                asyncResult,
                errors);
    QSpiceMainChannel *obj = static_cast<QSpiceMainChannel*>(
                g_async_result_get_user_data(asyncResult));
    size_t count = sizeof(errors)/sizeof(*errors);
    for ( uint i = 0; i<count; i++ ) {
        //qDebug()<<errors[i]->code<< QString::fromUtf8(errors[i]->message);
        if (obj) emit obj->downloaded(0, 100);
    };
}

void QSpiceMainChannel::progressCallback(uint current_num_bytes, uint total_num_bytes, void *user_data)
{
    QSpiceMainChannel *obj = static_cast<QSpiceMainChannel*>(user_data);
    //qDebug()<<current_num_bytes<<total_num_bytes;
    if (obj) emit obj->downloaded(current_num_bytes, total_num_bytes);
}
