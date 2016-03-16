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
#include "qspice-inputs-channel.h"

#include <QHash>
#include <QVarLengthArray>

// Inputs

void QSpiceHelper::inputs_modifiers(SpiceInputsChannel *spiceinputschannel, gpointer user_data)
{
    Q_UNUSED(spiceinputschannel)

    QSpiceInputsChannel *inputs = static_cast<QSpiceInputsChannel*>(user_data);
    if ( nullptr==inputs ) return;
    emit inputs->inputsModifiers();
}


void QSpiceInputsChannel::initCallbacks()
{
    g_signal_connect(gobject, "inputs-modifiers",
                     (GCallback) QSpiceHelper::inputs_modifiers, this);
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

    QScanCodeArray(uint code1, uint code2, uint code3)
    {
        append(code1);
        append(code2);
        append(code3);
    }

    QScanCodeArray(uint code1, uint code2, uint code3, uint code4)
    {
        append(code1);
        append(code2);
        append(code3);
        append(code4);
    }

};


// Scan Code Hash
#define ADD_SCAN1(q, s1)    scanCodeHash.insert(q, s1);
#define ADD_SCAN2(q, s1, s2)    scanCodeHash.insert(q, QScanCodeArray(s1, s2));
#define ADD_SCAN3(q, s1, s2, s3)    scanCodeHash.insert(q, QScanCodeArray(s1, s2, s3));
#define ADD_SCAN4(q, s1, s2, s3, s4)    scanCodeHash.insert(q, QScanCodeArray(s1, s2, s3, s4));

typedef QHash<int, QScanCodeArray> ScanCodeHash;

static ScanCodeHash scanCodeHash;

void InitScanCodeMap()
{
    // SPICE protocol use PC AT scan codes for keyboard
    // See for: http://philipstorr.id.au/pcbook/book3/scancode.htm
    // WARNING: (Prnt, Scrn) <-> (Keypad *)
    scanCodeHash.clear();
    scanCodeHash.insert(Qt::Key_Space,          0x39);
    scanCodeHash.insert(Qt::Key_A,              0x1E);
    scanCodeHash.insert(Qt::Key_B,              0x30);
    scanCodeHash.insert(Qt::Key_C,              0x2E);
    scanCodeHash.insert(Qt::Key_D,              0x20);
    scanCodeHash.insert(Qt::Key_E,              0x12);
    scanCodeHash.insert(Qt::Key_F,              0x21);
    scanCodeHash.insert(Qt::Key_G,              0x22);
    scanCodeHash.insert(Qt::Key_H,              0x23);
    scanCodeHash.insert(Qt::Key_I,              0x17);
    scanCodeHash.insert(Qt::Key_J,              0x24);
    scanCodeHash.insert(Qt::Key_K,              0x25);
    scanCodeHash.insert(Qt::Key_L,              0x26);
    scanCodeHash.insert(Qt::Key_M,              0x32);
    scanCodeHash.insert(Qt::Key_N,              0x31);
    scanCodeHash.insert(Qt::Key_O,              0x18);
    scanCodeHash.insert(Qt::Key_P,              0x19);
    scanCodeHash.insert(Qt::Key_Q,              0x10);
    scanCodeHash.insert(Qt::Key_R,              0x13);
    scanCodeHash.insert(Qt::Key_S,              0x1F);
    scanCodeHash.insert(Qt::Key_T,              0x14);
    scanCodeHash.insert(Qt::Key_U,              0x16);
    scanCodeHash.insert(Qt::Key_V,              0x2F);
    scanCodeHash.insert(Qt::Key_W,              0x11);
    scanCodeHash.insert(Qt::Key_X,              0x2D);
    scanCodeHash.insert(Qt::Key_Y,              0x15);
    scanCodeHash.insert(Qt::Key_Z,              0x2C);
    scanCodeHash.insert(Qt::Key_QuoteLeft,      0x29);
    scanCodeHash.insert(Qt::Key_AsciiTilde,     0x29);
    scanCodeHash.insert(Qt::Key_1,              0x02);
    scanCodeHash.insert(Qt::Key_Exclam,         0x02);
    scanCodeHash.insert(Qt::Key_At,             0x03);
    scanCodeHash.insert(Qt::Key_2,              0x03);
    scanCodeHash.insert(Qt::Key_3,              0x04);
    scanCodeHash.insert(Qt::Key_NumberSign,     0x04);
    scanCodeHash.insert(Qt::Key_4,              0x05);
    scanCodeHash.insert(Qt::Key_Dollar,         0x05);
    scanCodeHash.insert(Qt::Key_5,              0x06);
    scanCodeHash.insert(Qt::Key_Percent,        0x06);
    scanCodeHash.insert(Qt::Key_6,              0x07);
    scanCodeHash.insert(Qt::Key_AsciiCircum,    0x07);
    scanCodeHash.insert(Qt::Key_7,              0x08);
    scanCodeHash.insert(Qt::Key_Ampersand,      0x08);
    scanCodeHash.insert(Qt::Key_8,              0x09);
    ADD_SCAN2          (Qt::Key_Asterisk,       0x2A, 0x37);
    scanCodeHash.insert(Qt::Key_9,              0x0A);
    scanCodeHash.insert(Qt::Key_ParenLeft,      0x0A);
    scanCodeHash.insert(Qt::Key_0,              0x0B);
    scanCodeHash.insert(Qt::Key_ParenRight,     0x0B);
    scanCodeHash.insert(Qt::Key_Comma,          0x33);
    scanCodeHash.insert(Qt::Key_Less,           0x33);
    scanCodeHash.insert(Qt::Key_Period,         0x34);
    scanCodeHash.insert(Qt::Key_Greater,        0x34);
    scanCodeHash.insert(Qt::Key_Minus,          0x0C);
    scanCodeHash.insert(Qt::Key_Underscore,     0x0C);
    scanCodeHash.insert(Qt::Key_Equal,          0x0D);
    scanCodeHash.insert(Qt::Key_Plus,           0x4E);
    scanCodeHash.insert(Qt::Key_Slash,          0x35);
    scanCodeHash.insert(Qt::Key_Question,       0x35);
    scanCodeHash.insert(Qt::Key_Backslash,      0x2B);
    scanCodeHash.insert(Qt::Key_Bar,            0x2B);
    scanCodeHash.insert(Qt::Key_Apostrophe,     0x28);
    scanCodeHash.insert(Qt::Key_QuoteDbl,       0x28);
    scanCodeHash.insert(Qt::Key_Semicolon,      0x27);
    scanCodeHash.insert(Qt::Key_Colon,          0x27);
    scanCodeHash.insert(Qt::Key_BracketLeft,    0x1A);
    scanCodeHash.insert(Qt::Key_BraceLeft,      0x1A);
    scanCodeHash.insert(Qt::Key_BracketRight,   0x1B);
    scanCodeHash.insert(Qt::Key_BraceRight,     0x1B);

    // Misc
    scanCodeHash.insert(Qt::Key_Escape,         0x01);
    scanCodeHash.insert(Qt::Key_Tab,            0x0F);
    scanCodeHash.insert(Qt::Key_Backtab,        0x0F);
    // not applicable in PC AT keyboard
    // ADD_SCAN2(Qt::Key_Super_L,               0xE0, 0x5B);
    // ADD_SCAN2(Qt::Key_Super_R,               0xE0, 0x5C);
    // ADD_SCAN2(Qt::Key_Meta, 0x00,            0x00);

    // Specials
    scanCodeHash.insert(Qt::Key_Shift,          0x2A);
    scanCodeHash.insert(Qt::Key_CapsLock,       0x3A);
    scanCodeHash.insert(Qt::Key_Return,         0x1C);
    ADD_SCAN2          (Qt::Key_Enter,          0xE0, 0x1C);
    scanCodeHash.insert(Qt::Key_Backspace,      0x0E);
    scanCodeHash.insert(Qt::Key_Control,        0x1D);
    scanCodeHash.insert(Qt::Key_Alt,            0x38);
    ADD_SCAN2          (Qt::Key_AltGr,          0xE0, 0x38);
    // not applicable in PC AT keyboard
    // scanCodeHash.insert(Qt::Key_Menu,        0x00);

    // Cursor Keys/Num pad
    ADD_SCAN2          (Qt::Key_Print,          0xE0, 0x37);
    scanCodeHash.insert(Qt::Key_ScrollLock,     0x46);
    ADD_SCAN2          (Qt::Key_Pause,          0xE0, 0x52);

    ADD_SCAN2          (Qt::Key_Insert,         0xE0, 0x52);
    ADD_SCAN2          (Qt::Key_Home,           0xE0, 0x47);
    ADD_SCAN2          (Qt::Key_PageUp,         0xE0, 0x49);
    ADD_SCAN2          (Qt::Key_Delete,         0xE0, 0x53);
    ADD_SCAN2          (Qt::Key_End,            0xE0, 0x4F);
    ADD_SCAN2          (Qt::Key_PageDown,       0xE0, 0x51);

    ADD_SCAN2          (Qt::Key_Up,             0xE0, 0x48);
    ADD_SCAN2          (Qt::Key_Left,           0xE0, 0x4B);
    ADD_SCAN2          (Qt::Key_Down,           0xE0, 0x50);
    ADD_SCAN2          (Qt::Key_Right,          0xE0, 0x4D);
    ADD_SCAN2          (Qt::Key_NumLock,        0x45, 0xC5);

    scanCodeHash.insert(Qt::Key_Clear,          0x39);

    // functional keys
    scanCodeHash.insert(Qt::Key_F1,             0x3B);
    scanCodeHash.insert(Qt::Key_F2,             0x3C);
    scanCodeHash.insert(Qt::Key_F3,             0x3D);
    scanCodeHash.insert(Qt::Key_F4,             0x3E);
    scanCodeHash.insert(Qt::Key_F5,             0x3F);
    scanCodeHash.insert(Qt::Key_F6,             0x40);
    scanCodeHash.insert(Qt::Key_F7,             0x41);
    scanCodeHash.insert(Qt::Key_F8,             0x42);
    scanCodeHash.insert(Qt::Key_F9,             0x43);
    scanCodeHash.insert(Qt::Key_F10,            0x44);
    scanCodeHash.insert(Qt::Key_F11,            0xD9);
    scanCodeHash.insert(Qt::Key_F12,            0xDA);

    // additional non-keyboard called keys :
    // not applicable in PC AT keyboard,
    // therefore used for send key sequence
    ADD_SCAN3          (Qt::Key_Launch1,        0x1D, 0x38, 0x3B);          // CtrlAltF1
    ADD_SCAN3          (Qt::Key_Launch2,        0x1D, 0x38, 0x3C);          // CtrlAltF2
    ADD_SCAN3          (Qt::Key_Launch3,        0x1D, 0x38, 0x3D);          // CtrlAltF3
    ADD_SCAN3          (Qt::Key_Launch4,        0x1D, 0x38, 0x3E);          // CtrlAltF4
    ADD_SCAN3          (Qt::Key_Launch5,        0x1D, 0x38, 0x3F);          // CtrlAltF5
    ADD_SCAN3          (Qt::Key_Launch6,        0x1D, 0x38, 0x40);          // CtrlAltF6
    ADD_SCAN3          (Qt::Key_Launch7,        0x1D, 0x38, 0x41);          // CtrlAltF7
    ADD_SCAN3          (Qt::Key_Launch8,        0x1D, 0x38, 0x42);          // CtrlAltF8
    ADD_SCAN3          (Qt::Key_LaunchB,        0x1D, 0x38, 0x0E);          // CtrlAltBackSpc
    ADD_SCAN4          (Qt::Key_LaunchD,        0x1D, 0x38, 0xE0, 0x53);    // CtrlAltDel
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
