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
#ifndef QSPICE_CHANNEL_H
#define QSPICE_CHANNEL_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include "qspice-object.h"

struct SPICE_CHANNEL_MSG {
    QString     channel;
    QString     context;
    QString     msg;
};

class QSpiceChannel : public QSpiceObject
{
    Q_OBJECT
public:
    Q_GPROP_INT(ChannelID, "channel-id")
    Q_GPROP_POINTER(SessionPtr, "spice-session")

    bool objectDisposed();

    bool connectToChannel();

    bool isConnected() const;

signals:
    void channelEvent(int);
    void channelMsg(SPICE_CHANNEL_MSG&);
    // unused, conserved
    void channelDestroyed();


public slots:

protected:
    explicit QSpiceChannel(void *channel);
    friend class QSpiceHelper;
    virtual void initCallbacks();

private:
    bool connected;
};





#endif // QSPICE_CHANNEL_H
