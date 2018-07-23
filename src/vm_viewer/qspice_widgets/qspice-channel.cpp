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
#include "qspice-channel.h"

void QSpiceHelper::channel_event(
                            SpiceChannel        *channel,
                            SpiceChannelEvent    event,
                            gpointer             user_data)
{
    Q_UNUSED(channel);
    QSpiceChannel *_channel =
            static_cast<QSpiceChannel*>(user_data);
    if ( Q_NULLPTR==_channel ) return;
    int ev = static_cast<int>(event);
    emit _channel->channelEvent(ev);
}

QSpiceChannel::QSpiceChannel(void *channel) :
    QSpiceObject(channel)
{
    connected = false;
    initCallbacks();
}

void QSpiceChannel::initCallbacks()
{
    g_signal_connect(gobject, "channel-event",
                     GCallback(QSpiceHelper::channel_event), this);
}

// Channel
bool QSpiceChannel::objectDisposed()
{
    //emit channelDestroyed();
    return true;
}

bool QSpiceChannel::connectToChannel()
{
    connected = spice_channel_connect(static_cast<SpiceChannel *>(gobject));
    return connected;
}

bool QSpiceChannel::isConnected() const
{
    return connected;
}
