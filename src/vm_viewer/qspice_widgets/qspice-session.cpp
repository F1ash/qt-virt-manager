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
#include "qspice-session.h"
#include "qspice-display-channel.h"
#include "qspice-inputs-channel.h"
#include "qspice-main-channel.h"
#include "qspice-cursor-channel.h"
#include "qspice-smartcard-channel.h"
#include "qspice-usbredir-channel.h"
#include "qspice-webdav-channel.h"
#include "qspice-playback-channel.h"
#include "qspice-record-channel.h"

QSpiceSession::QSpiceSession(QObject *parent) :
    QSpiceObject(parent)
{
    gobject = Q_NULLPTR;
    init();
}


void QSpiceHelper::ss_channel_new(SpiceSession *session, SpiceChannel *channel, gpointer user_data)
{
    Q_UNUSED(session);
    QSpiceSession *_session = static_cast<QSpiceSession *>(user_data);
    if ( Q_NULLPTR==_session ) return;

    gint channelType = 0;
    g_object_get(
                G_OBJECT (channel),
                "channel-type",
                &channelType,
                Q_NULLPTR);

    QSpiceChannel *_channel = Q_NULLPTR;

    switch(channelType)
    {
    case SPICE_CHANNEL_MAIN:
        _channel = new QSpiceMainChannel(channel);
        break;

    case SPICE_CHANNEL_DISPLAY:
        _channel = new QSpiceDisplayChannel(channel);
        break;

    case SPICE_CHANNEL_INPUTS:
        _channel = new QSpiceInputsChannel(channel);
        break;

    case SPICE_CHANNEL_CURSOR:
        _channel = new QSpiceCursorChannel(channel);
        break;

    case SPICE_CHANNEL_SMARTCARD:
        _channel = new QSpiceSmartcardChannel(channel);
        break;

    case SPICE_CHANNEL_USBREDIR:
        _channel = new QSpiceUSBRedirChannel(channel);
        break;

    case SPICE_CHANNEL_WEBDAV:
        _channel = new QSpiceWebDAVChannel(channel);
        break;

    case SPICE_CHANNEL_PLAYBACK:
        _channel = new QSpicePlaybackChannel(channel);
        break;

    case SPICE_CHANNEL_RECORD:
        _channel = new QSpiceRecordChannel(channel);
        break;

    default:
        _channel = new QSpiceChannel(channel);
    }

    emit _session->channelInited(_channel);

}

void QSpiceHelper::ss_channel_destroy(SpiceSession *session, SpiceChannel *channel, gpointer user_data)
{
    Q_UNUSED(session);
    QSpiceSession *_session = static_cast<QSpiceSession *>(user_data);
    if ( Q_NULLPTR==_session ) return;

    gint channelType = 0;
    g_object_get(
                G_OBJECT (channel),
                "channel-type",
                &channelType,
                Q_NULLPTR);

    //emit _session->channelDestroyed(channelType);
}

void QSpiceSession::init()
{
    //spice_util_set_debug(true);
    //qDebug()<<"Spice.ver.:"<<spice_util_get_version_string()
    //       <<"\nDEBUG mode:"<<(bool)spice_util_get_debug();
    gobject = spice_session_new ();
    if ( Q_NULLPTR==gobject ) {
        //qDebug()<<"session not inited";
        return;
    };
    //setEnableClientSockets(true);
    setEnableAudio(true);
    setEnableSmartcard(true);
    setEnableUsbredir(true);
    setReadOnly(false);
    //setSharedDir(QString("%1/Public").arg(qgetenv("HOME").data()));
    g_signal_connect(gobject, "channel-new",
                     GCallback(QSpiceHelper::ss_channel_new), this);
    g_signal_connect(gobject, "channel-destroy",
                     GCallback(QSpiceHelper::ss_channel_destroy), this);
    //qDebug()<<getSharedDir()<<"shared";
#if WITH_LIBCACARD
    // http://www.spice-space.org/page/SmartcardUsage#Using_a_software_smartcard
    // for test only
    if ( getSmartcardDB().isEmpty() ) {
        setSmartcardDB(QString("%1/.netscape").arg(qgetenv("HOME").data()));
        setSmartcardCAC(QStringList()<<"cert1"<<"cert2"<<"cert3");
    };
    //qDebug()<<getSmartcardDB()<<"DB"<<getSmartcardCAC()<<"SmartcardCAC";
#endif
}


bool QSpiceSession::openSession()
{
    return spice_session_connect (static_cast<SpiceSession*>(gobject));
}

void QSpiceSession::closeSession()
{
    spice_session_disconnect(static_cast<SpiceSession*>(gobject));
}

