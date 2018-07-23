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
#include "qspice-display-channel.h"

// Display Channel

void QSpiceHelper::display_primary_create(SpiceDisplayChannel *display,
                            gint                 format,
                            gint                 width,
                            gint                 height,
                            gint                 stride,
                            gint                 shmid,
                            gpointer             imgdata,
                            gpointer             user_data)
{
    Q_UNUSED(display)

    QSpiceDisplayChannel *_display =
            static_cast<QSpiceDisplayChannel*>(user_data);
    if ( Q_NULLPTR==_display ) return;
    emit _display->displayPrimaryCreated(
                format, width, height, stride, shmid, imgdata);
}

void QSpiceHelper::display_primary_destroy(SpiceDisplayChannel *display,
                             gpointer             user_data)
{
    Q_UNUSED(display)
    QSpiceDisplayChannel *_display =
            static_cast<QSpiceDisplayChannel*>(user_data);
    if ( Q_NULLPTR==_display ) return;
    emit _display->displayPrimaryDestroyed();
}

void QSpiceHelper::display_invalidate (SpiceDisplayChannel *display,
                                gint                 x,
                                gint                 y,
                                gint                 width,
                                gint                 height,
                                gpointer             user_data)
{
    Q_UNUSED(display)
    QSpiceDisplayChannel *_display =
            static_cast<QSpiceDisplayChannel*>(user_data);
    if ( Q_NULLPTR==_display || _display->parentWdg==Q_NULLPTR ) return;
    if ( _display->scaled ) {
        emit _display->displayInvalidated(x, y, width, height);
    } else {
        // avoiding additional delay in updating
        // the display of the virtual machine
        // that is related with the receipt of signals
        // to the slot using eventloop.
        _display->parentWdg->repaint(x, y, width, height);
    };
}

void QSpiceHelper::display_mark(SpiceDisplayChannel *display,
                         gint                 mark,
                         gpointer             user_data)
{
    Q_UNUSED(display)
    QSpiceDisplayChannel *_display =
            static_cast<QSpiceDisplayChannel*>(user_data);
    if ( Q_NULLPTR==_display ) return;
    emit _display->displayMarked(mark);
}


void QSpiceDisplayChannel::initCallbacks()
{
    g_signal_connect(gobject, "display-primary-create",
                     GCallback(QSpiceHelper::display_primary_create), this);
    g_signal_connect(gobject, "display-invalidate",
                     GCallback(QSpiceHelper::display_invalidate), this);
    g_signal_connect(gobject, "display-primary-destroy",
                     GCallback(QSpiceHelper::display_primary_destroy), this);
    g_signal_connect(gobject, "display-mark",
                     GCallback(QSpiceHelper::display_mark), this);
}

void QSpiceDisplayChannel::setParentWidget(void *_obj)
{
    parentWdg = static_cast<QWidget*>(_obj);
}

void QSpiceDisplayChannel::setScaled(bool s)
{
    scaled = s;
}
