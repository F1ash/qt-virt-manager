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

#ifndef QSPICECURSORCHANNEL_H
#define QSPICECURSORCHANNEL_H

#include "qspicechannel.h"

class QSpiceCursorChannel : public QSpiceChannel
{
    Q_OBJECT
public:

signals:

    void cursorSet(
            int                width,
            int                height,
            int                hot_x,
            int                hot_y,
            void *             rgba);

protected:
    inline QSpiceCursorChannel(void *channel) : QSpiceChannel(channel) {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();
};

#endif // QSPICECURSORCHANNEL_H
