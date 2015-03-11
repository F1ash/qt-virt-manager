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

#ifndef QSPICEDISPLAYCHANNEL_H
#define QSPICEDISPLAYCHANNEL_H

#include "qspicechannel.h"

class QSpiceDisplayChannel : public QSpiceChannel
{
    Q_OBJECT
public:

    Q_GPROP_INT(Height, "height")
    Q_GPROP_INT(MaxMonitors, "monitors-max")
    Q_GPROP_INT(Width, "width")

signals:
    void displayPrimaryCreate(
         int                 format,
         int                 width,
         int                 height,
         int                 stride,
         int                 shmid,
         void *              imgdata);

    void displayInvalidate(
        int                 x,
        int                 y,
        int                 width,
        int                 height);

    void displayPrimaryDestroy();

    void displayMark(int mark);


protected:
    inline QSpiceDisplayChannel(void *channel) : QSpiceChannel(channel) {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();
};

#endif // QSPICEDISPLAYCHANNEL_H
