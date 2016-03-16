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

#ifndef QSPICE_CURSOR_CHANNEL_H
#define QSPICE_CURSOR_CHANNEL_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include "qspice-channel.h"

class QSpiceCursorChannel : public QSpiceChannel
{
    Q_OBJECT
public:

signals:

    void cursorData(
            int                width,
            int                height,
            int                hot_x,
            int                hot_y,
            void *             rgba);

protected:
    inline QSpiceCursorChannel(void *channel) :
        QSpiceChannel(channel)
    {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();
};

#endif // QSPICE_CURSOR_CHANNEL_H
