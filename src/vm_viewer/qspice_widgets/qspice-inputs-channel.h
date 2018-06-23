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


#ifndef QSPICE_INPUTS_CHANNEL_H
#define QSPICE_INPUTS_CHANNEL_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include "qspice-channel.h"

#include <QHash>
#include <QVarLengthArray>

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

typedef QHash<int, QScanCodeArray> ScanCodeHash;

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
    // SPICE protocol use PC AT scan codes
    void inputsKeyPress(uint scancode);
    void inputsKeyPressAndRelease(uint scancode);
    void inputsKeyRelease(uint scancode);
    void inputsSetKeyLocks(uint locks);

    // Qt Virtual Keys (platform independant)
    void inputsQKeyPress(int key);
    void inputsQKeyRelease(int key);
    void inputsQKeypadKeyPress(int key);
    void inputsQKeypadKeyRelease(int key);
    void inputsQSequenceKeyPress(int key);
    void inputsQSequenceKeyRelease(int key);

signals:
    void inputsModifiers();

protected:
    inline QSpiceInputsChannel(void *channel) :
        QSpiceChannel(channel)
    {
        initCallbacks();
        initScanCodeMap();
        initKeypadScanCodeMap();
        initSequenceScanCodeMap();
    }
    friend class QSpiceHelper;

    void initCallbacks();

private:
    QHash<int, uint>    scanCodeHash, keypadScanCodeHash;
    ScanCodeHash        sequenceCodeHash;
    void                initScanCodeMap();
    void                initKeypadScanCodeMap();
    void                initSequenceScanCodeMap();
    QScanCodeArray      QSequenceKeyToScanCode(int key);
};
#endif // QSPICE_INPUTS_CHANNEL_H
