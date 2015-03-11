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
#include "qspiceinputschannel.h"

#include <QHash>
#include <QVarLengthArray>

// Inputs

void QSpiceHelper::inputs_modifiers(SpiceInputsChannel *spiceinputschannel, gpointer user_data)
{
    Q_UNUSED(spiceinputschannel)

    QSpiceInputsChannel *inputs = static_cast<QSpiceInputsChannel*>(user_data);

    emit inputs->inputsModifiers();
}


void QSpiceInputsChannel::initCallbacks()
{
    g_signal_connect(gobject, "inputs-modifiers", (GCallback) QSpiceHelper::inputs_modifiers, this);
}

void QSpiceInputsChannel::inputsPosition(
        int x,
        int y,
        int display,
        int button_state)
{
    spice_inputs_position((SpiceInputsChannel *) gobject, x, y, display, button_state);
}

void QSpiceInputsChannel::inputsButtonPress(int button, int button_state)
{
    spice_inputs_button_press((SpiceInputsChannel *) gobject, button, button_state);
}

void QSpiceInputsChannel::inputsButtonRelease(int button, int button_state)
{
    spice_inputs_button_release((SpiceInputsChannel *) gobject, button, button_state);
}


// keyboard
void QSpiceInputsChannel::inputsKeyPress(uint scancode)
{
    spice_inputs_key_press((SpiceInputsChannel *) gobject, scancode);
}

void QSpiceInputsChannel::inputsKeyPressAndRelease(uint scancode)
{
    spice_inputs_key_press_and_release((SpiceInputsChannel *) gobject, scancode);
}

void QSpiceInputsChannel::inputsKeyRelease(uint scancode)
{
    spice_inputs_key_release((SpiceInputsChannel *) gobject, scancode);
}

void QSpiceInputsChannel::inputsSetKeyLocks(uint locks)
{
    spice_inputs_set_key_locks((SpiceInputsChannel *) gobject, locks);
}



class QScanCodeArray : public QVarLengthArray<uint>
{
public:
    QScanCodeArray()
    {
    }

    QScanCodeArray(uint code)
    {
        append(code);
    }

    QScanCodeArray(uint code1, uint code2)
    {
        append(code1);
        append(code2);
    }

};


// Scan Code Hash
#define ADD_SCAN1(q, s1)    scanCodeHash.insert(q, s1);
#define ADD_SCAN2(q, s1, s2)    scanCodeHash.insert(q, QScanCodeArray(s1, s2));

typedef QHash<int, QScanCodeArray> ScanCodeHash;

static ScanCodeHash scanCodeHash;

void InitScanCodeMap()
{
    scanCodeHash.clear();
    scanCodeHash.insert(Qt::Key_Space, 0x39);
    scanCodeHash.insert(Qt::Key_A, 0x1E);
    scanCodeHash.insert(Qt::Key_B, 0x30);
    scanCodeHash.insert(Qt::Key_C, 0x2E);
    scanCodeHash.insert(Qt::Key_D, 0x20);
    scanCodeHash.insert(Qt::Key_E, 0x12);
    scanCodeHash.insert(Qt::Key_F, 0x21);
    scanCodeHash.insert(Qt::Key_G, 0x22);
    scanCodeHash.insert(Qt::Key_H, 0x23);
    scanCodeHash.insert(Qt::Key_I, 0x17);
    scanCodeHash.insert(Qt::Key_J, 0x24);
    scanCodeHash.insert(Qt::Key_K, 0x25);
    scanCodeHash.insert(Qt::Key_L, 0x26);
    scanCodeHash.insert(Qt::Key_M, 0x32);
    scanCodeHash.insert(Qt::Key_N, 0x31);
    scanCodeHash.insert(Qt::Key_O, 0x18);
    scanCodeHash.insert(Qt::Key_P, 0x19);
    scanCodeHash.insert(Qt::Key_Q, 0x10);
    scanCodeHash.insert(Qt::Key_R, 0x13);
    scanCodeHash.insert(Qt::Key_S, 0x1F);
    scanCodeHash.insert(Qt::Key_T, 0x14);
    scanCodeHash.insert(Qt::Key_U, 0x16);
    scanCodeHash.insert(Qt::Key_V, 0x2F);
    scanCodeHash.insert(Qt::Key_W, 0x11);
    scanCodeHash.insert(Qt::Key_X, 0x2D);
    scanCodeHash.insert(Qt::Key_Y, 0x15);
    scanCodeHash.insert(Qt::Key_Z, 0x2C);
    scanCodeHash.insert(Qt::Key_0, 0x0B);
    scanCodeHash.insert(Qt::Key_1, 0x02);
    scanCodeHash.insert(Qt::Key_2, 0x03);
    scanCodeHash.insert(Qt::Key_3, 0x04);
    scanCodeHash.insert(Qt::Key_4, 0x05);
    scanCodeHash.insert(Qt::Key_5, 0x06);
    scanCodeHash.insert(Qt::Key_6, 0x07);
    scanCodeHash.insert(Qt::Key_7, 0x08);
    scanCodeHash.insert(Qt::Key_8, 0x09);
    scanCodeHash.insert(Qt::Key_9, 0x0A);

    // Misc
    scanCodeHash.insert(Qt::Key_Escape, 0x01);
    scanCodeHash.insert(Qt::Key_Tab, 0x0F);
    ADD_SCAN2(Qt::Key_Super_L, 0xE0, 0x5B);
    ADD_SCAN2(Qt::Key_Super_R, 0xE0, 0x5C);
    ADD_SCAN2(Qt::Key_Meta, 0xE0, 0x5B);

    // Specials
    scanCodeHash.insert(Qt::Key_Shift, 0x2A);
    scanCodeHash.insert(Qt::Key_CapsLock, 0x3A);
    scanCodeHash.insert(Qt::Key_Enter, 0x1C);
    scanCodeHash.insert(Qt::Key_Return, 0x1C);
    scanCodeHash.insert(Qt::Key_Backspace, 0x0E);
    scanCodeHash.insert(Qt::Key_Control, 0x1D);
    scanCodeHash.insert(Qt::Key_Alt, 0x38);

    // Cursor Keys/Num pad
    ADD_SCAN2(Qt::Key_Insert,    0xE0, 0x52);
    ADD_SCAN2(Qt::Key_Home,      0xE0, 0x47);
    ADD_SCAN2(Qt::Key_PageUp,    0xE0, 0x49);
    ADD_SCAN2(Qt::Key_Delete,    0xE0, 0x53);
    ADD_SCAN2(Qt::Key_End,       0xE0, 0x4F);
    ADD_SCAN2(Qt::Key_PageDown,  0xE0, 0x51);

    ADD_SCAN2(Qt::Key_Up,    0xE0, 0x48);
    ADD_SCAN2(Qt::Key_Left,  0xE0, 0x4B);
    ADD_SCAN2(Qt::Key_Down,  0xE0, 0x50);
    ADD_SCAN2(Qt::Key_Right, 0xE0, 0x4D);
    ADD_SCAN1(Qt::Key_NumLock, 0x45);

}

// Qt Virtual Keys (platform independant)
QScanCodeArray QKeyToScanCode(int key)
{
    if (scanCodeHash.empty())
        InitScanCodeMap();

    ScanCodeHash::iterator it = scanCodeHash.find(key);
    if (it == scanCodeHash.end())
        return QScanCodeArray();
    else
        return it.value();
}

void QSpiceInputsChannel::inputsQKeyPress(int key)
{
    QScanCodeArray scanCode = QKeyToScanCode(key);
    for (int i = 0; i < scanCode.count(); i++)
        inputsKeyPress(scanCode[i]);
}

void QSpiceInputsChannel::inputsQKeyRelease(int key)
{
    QScanCodeArray scanCode = QKeyToScanCode(key);
    for (int i = 0; i < scanCode.count(); i++)
        inputsKeyRelease(scanCode[i]);

}
