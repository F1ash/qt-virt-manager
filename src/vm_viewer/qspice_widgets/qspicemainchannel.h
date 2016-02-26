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

#ifndef QSPICEMAINCHANNEL_H
#define QSPICEMAINCHANNEL_H

#include "qspicechannel.h"
#include <QStringList>
#include <spice/vd_agent.h>

class QSpiceMainChannel : public QSpiceChannel
{
    Q_OBJECT
public:
    Q_GPROP_BOOL(AgentConnected, "agent-connected")     // doc default FALSE

    void mainSetDisplay(int id, int x, int y, int width, int height);
    void mainSetDisplayEnabled(int id, bool enabled);
    bool mainSendMonitorConfig();

    void mainClipboardSelectionGrab(uint selection, quint32 *types, int ntypes);
    void mainClipboardSelectionRelease();
    void mainClipboardSelectionNotify(quint32, const uchar*, size_t);
    void guestClipboardSelectionRequest();

    void mainFileCopyAsync(QStringList&);

    static void mainFileCopyFinish(void*, void*, void*);
    static void progressCallback(uint, uint, void*);

signals:
    void main_AgentUpdate();
    void main_ClipboardSelection(uint, void*, uint);
    void main_ClipboardSelectionGrab(uint, void*, uint);
    void guest_ClipboardSelectionRelease(uint);
    void main_ClipboardSelectionRequest(uint, uint);
    void main_MouseUpdate();
    void downloaded(int, int);

protected:
    inline QSpiceMainChannel(void *channel) : QSpiceChannel(channel) {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();
};

#endif // QSPICEMAINCHANNEL_H
