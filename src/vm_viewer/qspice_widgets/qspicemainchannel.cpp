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
#include <spice/vd_agent.h>


void QSpiceHelper::main_agent_update(SpiceMainChannel *spicemainchannel, gpointer user_data)
{
    Q_UNUSED(spicemainchannel)

    QSpiceMainChannel *_mainchannel = static_cast<QSpiceMainChannel*>(user_data);

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

    QSpiceMainChannel *_mainchannel = static_cast<QSpiceMainChannel*>(user_data);
    QString cp = QString::fromUtf8((char*)data);
    emit _mainchannel->main_ClipboardSelection(cp);
}
void QSpiceHelper::main_clipboard_selection_grab(SpiceMainChannel *spicemainchannel,
                                                 guint selection,
                                                 gpointer types,
                                                 guint ntypes,
                                                 gpointer user_data)
{
    Q_UNUSED(spicemainchannel)

    QSpiceMainChannel *_mainchannel = static_cast<QSpiceMainChannel*>(user_data);
    emit _mainchannel->main_ClipboardSelectionGrab(selection, (void*)types, ntypes);
}
void QSpiceHelper::main_clipboard_selection_release(SpiceMainChannel *spicemainchannel,
                                                    guint selection,
                                                    gpointer user_data)
{
    Q_UNUSED(spicemainchannel)

    QSpiceMainChannel *_mainchannel = static_cast<QSpiceMainChannel*>(user_data);

    emit _mainchannel->main_ClipboardSelectionRelease(selection);
}
void QSpiceHelper::main_clipboard_selection_request(SpiceMainChannel *spicemainchannel,
                                                    guint selection,
                                                    guint types,
                                                    gpointer user_data)
{
    Q_UNUSED(spicemainchannel)

    QSpiceMainChannel *_mainchannel = static_cast<QSpiceMainChannel*>(user_data);

    emit _mainchannel->main_ClipboardSelectionRequest(selection, types);
}
void QSpiceHelper::main_mouse_update(SpiceMainChannel *spicemainchannel, gpointer user_data)
{
    Q_UNUSED(spicemainchannel)

    QSpiceMainChannel *_mainchannel = static_cast<QSpiceMainChannel*>(user_data);

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

void QSpiceMainChannel::mainSendMonitorConfig()
{
    spice_main_send_monitor_config((SpiceMainChannel *) gobject);
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

void QSpiceMainChannel::mainClipboardSelectionNotify(QString &_data)
{
    qDebug()<<"mainClipboardSelectionNotify"<<_data.toUtf8().data();
    QByteArray data = _data.toUtf8();
    size_t size = data.size()*sizeof(char);
    uchar *buff = (uchar*) malloc(size);
    for (int i=0; i<size; i++) {
        buff[i] = uchar(data.data()[i]);
        qDebug()<<"mainClipboardSelectionNotify"<<QString(buff[i]);
    };
    spice_main_clipboard_selection_notify(
                (SpiceMainChannel *) gobject,
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                VD_AGENT_CLIPBOARD_UTF8_TEXT,
                (const guchar*)buff,
                size);
    free(buff);
}

void QSpiceMainChannel::mainClipboardSelectionRequest()
{
    //spice_main_clipboard_selection_request((SpiceMainChannel *) gobject, selection, type);
    spice_main_clipboard_selection_request(
                (SpiceMainChannel *) gobject,
                VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD,
                VD_AGENT_CLIPBOARD_UTF8_TEXT);
}

void QSpiceMainChannel::mainFileCopyAsync(QStringList &fileNames)
{
    if ( fileNames.isEmpty() ) return;
    uint count = fileNames.count();
    qDebug()<<fileNames<<count;
    GFile* sources[count];
    uint i = 0;
    foreach (QString _path, fileNames) {
        GFile *file = g_file_new_for_path(_path.toUtf8().constData());
        sources[i] = file;
        i++;
    };
    qDebug()<<"SpiceMainChannel"<<this->gobject;
    spice_main_file_copy_async(
                (SpiceMainChannel *) gobject,
                sources,
                G_FILE_COPY_BACKUP,
                NULL,
                NULL,
                NULL,
                (GAsyncReadyCallback)mainFileCopyFinish,
                this);
}

void QSpiceMainChannel::mainFileCopyFinish(void *obj, void *result, void *error)
{
    bool res = spice_main_file_copy_finish(
                (SpiceMainChannel *)obj,
                (GAsyncResult *)result,
                (GError **)error);
}
