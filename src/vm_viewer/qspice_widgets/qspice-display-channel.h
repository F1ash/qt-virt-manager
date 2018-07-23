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

#ifndef QSPICE_DISPLAY_CHANNEL_H
#define QSPICE_DISPLAY_CHANNEL_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include "qspice-channel.h"
#include <QWidget>

class QSpiceDisplayChannel : public QSpiceChannel
{
    Q_OBJECT
public:

    Q_GPROP_INT(Height, "height")
    Q_GPROP_INT(MaxMonitors, "monitors-max")
    Q_GPROP_INT(Width, "width")
    void setParentWidget(void*);
    void setScaled(bool);

signals:
    void displayPrimaryCreated(
        int                 format,
        int                 width,
        int                 height,
        int                 stride,
        int                 shmid,
        void*               imgdata);

    void displayInvalidated(
        int                 x,
        int                 y,
        int                 width,
        int                 height);

    void displayPrimaryDestroyed();

    void displayMarked(int);


protected:
    inline QSpiceDisplayChannel(void *channel) :
        QSpiceChannel(channel)
    {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();

private:
    // pointer to QSpiceWidget:
    QWidget *parentWdg = Q_NULLPTR;
    bool     scaled    = false;
};

#endif // QSPICE_DISPLAY_CHANNEL_H
