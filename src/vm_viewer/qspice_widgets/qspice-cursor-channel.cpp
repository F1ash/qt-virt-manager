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
#include "qspice-cursor-channel.h"

void QSpiceCursorChannel::initCallbacks()
{
    g_signal_connect(gobject, "cursor-set",
                     GCallback(QSpiceHelper::cursor_set), this);
}

void QSpiceHelper::cursor_set(SpiceCursorChannel *cursor,
                       gint                width,
                       gint                height,
                       gint                hot_x,
                       gint                hot_y,
                       gpointer            rgba,
                       gpointer            user_data)
{
    Q_UNUSED(cursor)

    QSpiceCursorChannel *_cursor = static_cast<QSpiceCursorChannel*>(user_data);
    if ( Q_NULLPTR==_cursor ) return;
    emit _cursor->cursorData(width, height, hot_x, hot_y, rgba);
}
