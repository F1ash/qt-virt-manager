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

#ifndef QSPICE_MAIN_CHANNEL_H
#define QSPICE_MAIN_CHANNEL_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include "qspice-channel.h"

class QSpiceMainChannel : public QSpiceChannel
{
    Q_OBJECT
public:
    Q_GPROP_BOOL(AgentConnected, "agent-connected")  // doc default FALSE
    Q_GPROP_INT(MaxClipboard, "max-clipboard")       // doc default 104857600

    void setDisplay(int, int, int, int, int);
    void setDisplayEnabled(int, bool);
    void updateDisplay(int, int, int, int, int, bool);
    void updateDisplayEnabled(int, bool, bool);
    bool sendMonitorConfig();

    void clipboardSelectionGrab(uint, quint32*, int);
    void clipboardSelectionRelease();
    void clipboardSelectionNotify(quint32, const uchar*, size_t);
    void guestClipboardSelectionRequest();

    void fileCopyAsync(QStringList&);
    void cancelFileCopyAsync();
    static void fileCopyFinish(void*, void*, void*);
    static void progressCallback(uint, uint, void*);

signals:
    void agentUpdated();
    void clipboardSelection(uint, void*, uint);
    void clipboardSelectionGrabbed(uint, void*, uint);
    void clipboardSelectionReleased(uint);
    void clipboardSelectionRequested(uint, uint);
    void mouseUpdated();
    void migrationStarted();
    void downloaded(int, int);
    void cancelled();
    void downloadCompleted();

protected:
    inline QSpiceMainChannel(void *channel) :
        QSpiceChannel(channel)
    {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();

private:
    void *cancellable;
};

#endif // QSPICE_MAIN_CHANNEL_H
