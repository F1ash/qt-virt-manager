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


#ifndef QSPICEINPUTSCHANNEL_H
#define QSPICEINPUTSCHANNEL_H

#include "qspicechannel.h"


class QSpiceInputsChannel : public QSpiceChannel
{
    Q_OBJECT
public:
    // Mouse
    void inputsPosition(
            int x,
            int y,
            int display,
            int button_state);

    void inputsButtonPress(int button, int button_state);

    void inputsButtonRelease(int button, int button_state);

    // keyboard
    // XT Scan Codes (not AT)
    void inputsKeyPress(uint scancode);
    void inputsKeyPressAndRelease(uint scancode);
    void inputsKeyRelease(uint scancode);
    void inputsSetKeyLocks(uint locks);

    // Qt Virtual Keys (platform independant)
    void inputsQKeyPress(int key);
    void inputsQKeyRelease(int key);

signals:
    void inputsModifiers();

protected:
    inline QSpiceInputsChannel(void *channel) : QSpiceChannel(channel) {initCallbacks();}
    friend class QSpiceHelper;

    void initCallbacks();

};
#endif // QSPICEINPUTSCHANNEL_H
