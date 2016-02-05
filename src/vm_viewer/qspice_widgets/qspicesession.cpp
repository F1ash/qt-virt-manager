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

#include <glib-object.h>
#include <spice-session.h>
#include <spice-channel.h>

#include "qspicehelper.h"
#include "qspicesession.h"
#include "qspicechannel.h"
#include "qspicedisplaychannel.h"
#include "qspiceinputschannel.h"
#include "qspicemainchannel.h"
#include "qspicecursorchannel.h"
#include "qspicesmartcardchannel.h"
#include "qspiceusbredirchannel.h"
#include "qspicewebdavchannel.h"
#include "qspiceplaybackchannel.h"
#include "qspicerecordchannel.h"

QSpiceSession::QSpiceSession(QObject *parent) :
    QSpiceObject(parent)
{
    gobject = NULL;
    init();
}


void QSpiceHelper::ss_channel_new(SpiceSession *session, SpiceChannel *channel, gpointer user_data)
{
    Q_UNUSED(session);

    gint channelType = 0;
    g_object_get(G_OBJECT (channel),
                 "channel-type", &channelType,
                 NULL);

    QSpiceSession *_session = static_cast<QSpiceSession *>(user_data);
    QSpiceChannel *_channel = NULL;

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


    emit _session->channelNew(_channel);

}

void QSpiceHelper::ss_channel_destroy(SpiceSession *session, SpiceChannel *channel, gpointer user_data)
{
    Q_UNUSED(session);
    Q_UNUSED(user_data);

    gint channelType = 0;

    g_object_get(G_OBJECT (channel),
                 "channel-type", &channelType,
                 NULL);

    //qDebug() << "channel-destroy signal " << channelType  << endl;
}

void QSpiceSession::init()
{
    //spice_util_set_debug(true);
    //qDebug()<<"Spice.ver.:"<<spice_util_get_version_string()
    //       <<"\nDEBUG mode:"<<(bool)spice_util_get_debug();
    gobject = spice_session_new ();
    setEnableAudio(true);
    setEnableSmartcard(true);
    setEnableUsbredir(true);
    //setSharedDir(QString("/home/%1/Public").arg(qgetenv("USER").data()));
    g_signal_connect(gobject, "channel-new",
                     (GCallback) QSpiceHelper::ss_channel_new, this);
    g_signal_connect(gobject, "channel-destroy",
                     (GCallback) QSpiceHelper::ss_channel_destroy, this);
    qDebug()<<getSharedDir()<<"shared";
#if WITH_LIBCACARD
    // http://www.spice-space.org/page/SmartcardUsage#Using_a_software_smartcard
    // for test only
    if ( getSmartcardDB().isEmpty() ) {
        setSmartcardDB("/home/Flash/.netscape");
        setSmartcardCAC(QStringList()<<"cert1"<<"cert2"<<"cert3");
    };
    qDebug()<<getSmartcardDB()<<"DB"<<getSmartcardCAC()<<"SmartcardCAC";
#endif
}


bool QSpiceSession::Connect()
{
    gboolean rc = spice_session_connect ((SpiceSession *) gobject);

    return rc;
}

void QSpiceSession::Disconnect()
{
    spice_session_disconnect((SpiceSession *)  gobject);
}

