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


#ifndef QSPICEHELPER_H
#define QSPICEHELPER_H

#include <glib-object.h>
#include <spice-session.h>
#include <spice-channel.h>
#include <channel-display.h>
#include <channel-main.h>


class QSpiceHelper
{
public:
    QSpiceHelper();

    // Session callbacks
    static void ss_channel_new(SpiceSession *session, SpiceChannel *channel, gpointer user_data);
    static void ss_channel_destroy(SpiceSession *session, SpiceChannel *channel, gpointer user_data);

    // channel
    static void GDisposeEvent(gpointer data, GObject *_channel);

    // Display Channel
    static void display_primary_create(SpiceDisplayChannel *display,
                                gint                 format,
                                gint                 width,
                                gint                 height,
                                gint                 stride,
                                gint                 shmid,
                                gpointer             imgdata,
                                gpointer             user_data);

    static void display_primary_destroy(SpiceDisplayChannel *display,
                                 gpointer             user_data);

    static void display_invalidate (SpiceDisplayChannel *display,
                                    gint                 x,
                                    gint                 y,
                                    gint                 width,
                                    gint                 height,
                                    gpointer             user_data);

    static void display_mark(SpiceDisplayChannel *display,
                             gint                 mark,
                             gpointer             user_data);

    // Inputs Channel
    static void inputs_modifiers(SpiceInputsChannel *spiceinputschannel, gpointer user_data);

    // Cursor Channel
    static void cursor_set(SpiceCursorChannel *cursor,
                           gint                width,
                           gint                height,
                           gint                hot_x,
                           gint                hot_y,
                           gpointer            rgba,
                           gpointer            user_data);

};

#endif // QSPICEHELPER_H
