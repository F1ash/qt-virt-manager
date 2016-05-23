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
    if ( nullptr==_display ) return;
    emit _display->displayPrimaryCreated(
                format, width, height, stride, shmid, imgdata);
}

void QSpiceHelper::display_primary_destroy(SpiceDisplayChannel *display,
                             gpointer             user_data)
{
    Q_UNUSED(display)
    QSpiceDisplayChannel *_display =
            static_cast<QSpiceDisplayChannel*>(user_data);
    if ( nullptr==_display ) return;
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
    if ( nullptr==_display || _display->imageObj==nullptr ) return;
    //emit _display->displayInvalidated(x, y, width, height);
    // avoiding additional delay in updating
    // the display of the virtual machine
    // that is related with the receipt of signals
    // to the slot using eventloop.
    //if ( nullptr!=_display && _display->imageObj!=nullptr )
    //    _display->imageObj->update(x, y, width, height);
    if ( !_display->deltaIsZeroes() ) {
        // increment between scrollArea and pixbuffer
        // x=5, y=6; then for fullscreen the correction used.
        _display->imageObj->update(
                    x + _display->d_X-5,
                    y + _display->d_Y-6,
                    width + 15,
                    height + 18);
    } else {
        _display->imageObj->update(x, y, width, height);
    }
}

void QSpiceHelper::display_mark(SpiceDisplayChannel *display,
                         gint                 mark,
                         gpointer             user_data)
{
    Q_UNUSED(display)
    QSpiceDisplayChannel *_display =
            static_cast<QSpiceDisplayChannel*>(user_data);
    if ( nullptr==_display ) return;
    emit _display->displayMarked(mark);
}


void QSpiceDisplayChannel::initCallbacks()
{
    d_Y = d_X = 0;
    g_signal_connect(gobject, "display-primary-create",
                     (GCallback) QSpiceHelper::display_primary_create, this);
    g_signal_connect(gobject, "display-invalidate",
                     (GCallback) QSpiceHelper::display_invalidate, this);
    g_signal_connect(gobject, "display-primary-destroy",
                     (GCallback) QSpiceHelper::display_primary_destroy, this);
    g_signal_connect(gobject, "display-mark",
                     (GCallback) QSpiceHelper::display_mark, this);
}

void QSpiceDisplayChannel::setImageObject(void *_obj)
{
    imageObj = static_cast<QLabel*>(_obj);
}

void QSpiceDisplayChannel::setPositionDelta(int _dX, int _dY)
{
    d_X = _dX; d_Y = _dY;
}

bool QSpiceDisplayChannel::deltaIsZeroes()
{
    return ( d_X==0 && d_Y==0 );
}
