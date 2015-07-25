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
#include "qspicecursorchannel.h"

void QSpiceCursorChannel::initCallbacks()
{
    g_signal_connect(gobject, "cursor-set", (GCallback) QSpiceHelper::cursor_set, this);
    g_signal_connect(gobject, "cursor-move", (GCallback) QSpiceHelper::cursor_move, this);
    g_signal_connect(gobject, "cursor-hide", (GCallback) QSpiceHelper::cursor_hide, this);
    g_signal_connect(gobject, "cursor-reset", (GCallback) QSpiceHelper::cursor_reset, this);
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

    emit _cursor->cursorSet(width, height, hot_x, hot_y, rgba);
}

void QSpiceHelper::cursor_move(SpiceCursorChannel *cursor,
                       gint                dx,
                       gint                dy,
                       gpointer            user_data)
{
    Q_UNUSED(cursor)

    QSpiceCursorChannel *_cursor = static_cast<QSpiceCursorChannel*>(user_data);

    emit _cursor->cursorMove(dx, dy);
}

void QSpiceHelper::cursor_hide(SpiceCursorChannel *cursor,
                       gpointer            user_data)
{
    Q_UNUSED(cursor)

    QSpiceCursorChannel *_cursor = static_cast<QSpiceCursorChannel*>(user_data);

    emit _cursor->cursorHide();
}

void QSpiceHelper::cursor_reset(SpiceCursorChannel *cursor,
                       gpointer            user_data)
{
    Q_UNUSED(cursor)

    QSpiceCursorChannel *_cursor = static_cast<QSpiceCursorChannel*>(user_data);

    emit _cursor->cursorReset();
}
