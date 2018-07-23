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

// Inputs
void QSpiceHelper::inputs_modifiers(SpiceInputsChannel *spiceinputschannel, gpointer user_data)
{
    Q_UNUSED(spiceinputschannel)

    QSpiceInputsChannel *inputs = static_cast<QSpiceInputsChannel*>(user_data);
    if ( Q_NULLPTR==inputs ) return;
    emit inputs->inputsModifiers();
}


void QSpiceInputsChannel::initCallbacks()
{
    g_signal_connect(gobject, "inputs-modifiers",
                     GCallback(QSpiceHelper::inputs_modifiers), this);
}

void QSpiceInputsChannel::inputsPosition(
        int x,
        int y,
        int display,
        int button_state)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_inputs_channel_position(
#else
    spice_inputs_position(
#endif
                static_cast<SpiceInputsChannel*>(gobject), x, y, display, button_state);
}

void QSpiceInputsChannel::inputsButtonPress(int button, int button_state)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_inputs_channel_button_press(
#else
    spice_inputs_button_press(
#endif
                static_cast<SpiceInputsChannel*>(gobject),
                button,
                button_state);
}

void QSpiceInputsChannel::inputsButtonRelease(int button, int button_state)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_inputs_channel_button_release(
#else
    spice_inputs_button_release(
#endif
                static_cast<SpiceInputsChannel*>(gobject),
                button,
                button_state);
}

void QSpiceInputsChannel::inputsKeyPress(uint scancode)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_inputs_channel_key_press(
#else
    spice_inputs_key_press(
#endif
                static_cast<SpiceInputsChannel*>(gobject),
                scancode);
}

void QSpiceInputsChannel::inputsKeyPressAndRelease(uint scancode)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_inputs_channel_key_press_and_release(
#else
    spice_inputs_key_press_and_release(
#endif
                static_cast<SpiceInputsChannel*>(gobject),
                scancode);
}

void QSpiceInputsChannel::inputsKeyRelease(uint scancode)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_inputs_channel_key_release(
#else
    spice_inputs_key_release(
#endif
                static_cast<SpiceInputsChannel*>(gobject),
                scancode);
}

void QSpiceInputsChannel::inputsSetKeyLocks(uint locks)
{
#if SPICE_GTK_CHECK_VERSION(0, 35, 0)
    spice_inputs_channel_set_key_locks(
#else
    spice_inputs_set_key_locks(
#endif
                static_cast<SpiceInputsChannel*>(gobject),
                locks);
}

// Qt Keys map of scancodes (platform independent)
void QSpiceInputsChannel::initScanCodeMap()
{
    // SPICE protocol use PC AT scan codes for keyboard
    // See for: https://github.com/CendioOssman/keycodemapdb/blob/master/data/keymaps.csv

    // A PC XT (set 1) key scancode. For scancodes with an 0xe0 prefix,
    // drop the prefix and OR the scancode with 0x100 (see for example:
    // https://www.spice-space.org/api/spice-gtk/SpiceInputsChannel.html#spice-inputs-key-press).

    // Undetected keys: KEY_RIGHTSHIFT, KEY_102ND, KEY_RO, KEY_RIGHTCTRL,
    // KEY_KPJPCOMMA, KEY_RIGHTALT, KEY_LINEFEED, KEY_MACRO,
    // KEY_KPPLUSMINUS, KEY_SCALE-KEY_HANGEUL, KEY_RIGHTMETA,
    // KEY_AGAIN, KEY_PROPS, KEY_FRONT, KEY_MENU, KEY_SETUP, KEY_FILE-KEY_DELETEFILE,
    // KEY_PROG[1,2], KEY_CYCLEWINDOWS, KEY_CLOSECD-KEY_EJECTCD, KEY_CONFIG,
    // KEY_MOVE-KEY_SCROLLDOWN, KEY_PROG3-KEY_DASHBOARD, KEY_FASTFORWARD,
    // KEY_HP, KEY_SOUND, KEY_QUESTION, KEY_EMAIL, KEY_CONNECT, KEY_SPORT, KEY_ALTERASE,
    //

    scanCodeHash.clear();
    scanCodeHash.insert(Qt::Key_Escape, 0x01);
    scanCodeHash.insert(Qt::Key_Tab, 0x0f);
    scanCodeHash.insert(Qt::Key_Backtab, 0x0f);
    scanCodeHash.insert(Qt::Key_Backspace, 0x0e);
    scanCodeHash.insert(Qt::Key_Return, 0x1c);  // Qt::Key_Enter
    scanCodeHash.insert(Qt::Key_Enter, 0x1c);
    scanCodeHash.insert(Qt::Key_Insert, 0x0152);
    scanCodeHash.insert(Qt::Key_Delete, 0x0153);
    scanCodeHash.insert(Qt::Key_Pause, 0x0146);
    scanCodeHash.insert(Qt::Key_Print, 0x0139);
    scanCodeHash.insert(Qt::Key_SysReq, 0x54);
    //scanCodeHash.insert(Qt::Key_Clear, 0x00);
    scanCodeHash.insert(Qt::Key_Home, 0x0147);
    scanCodeHash.insert(Qt::Key_End, 0x014f);
    scanCodeHash.insert(Qt::Key_Left, 0x014b);
    scanCodeHash.insert(Qt::Key_Up, 0x0148);
    scanCodeHash.insert(Qt::Key_Right, 0x014d);
    scanCodeHash.insert(Qt::Key_Down, 0x0150);
    scanCodeHash.insert(Qt::Key_PageUp, 0x0149);
    scanCodeHash.insert(Qt::Key_PageDown, 0x0151);
    scanCodeHash.insert(Qt::Key_Shift, 0x2a);
    scanCodeHash.insert(Qt::Key_Control, 0x1d);
    scanCodeHash.insert(Qt::Key_Meta, 0x015b);  // KEY_LEFTMETA
    scanCodeHash.insert(Qt::Key_Alt, 0x38);
    //scanCodeHash.insert(Qt::Key_AltGr, 0x00);
    scanCodeHash.insert(Qt::Key_CapsLock, 0x3a);
    scanCodeHash.insert(Qt::Key_NumLock, 0x45);
    scanCodeHash.insert(Qt::Key_ScrollLock, 0x46);
    scanCodeHash.insert(Qt::Key_F1, 0x3b);
    scanCodeHash.insert(Qt::Key_F2, 0x3c);
    scanCodeHash.insert(Qt::Key_F3, 0x3d);
    scanCodeHash.insert(Qt::Key_F4, 0x3e);
    scanCodeHash.insert(Qt::Key_F5, 0x3f);
    scanCodeHash.insert(Qt::Key_F6, 0x40);
    scanCodeHash.insert(Qt::Key_F7, 0x41);
    scanCodeHash.insert(Qt::Key_F8, 0x42);
    scanCodeHash.insert(Qt::Key_F9, 0x43);
    scanCodeHash.insert(Qt::Key_F10, 0x44);
    scanCodeHash.insert(Qt::Key_F11, 0x57);
    scanCodeHash.insert(Qt::Key_F12, 0x58);
    scanCodeHash.insert(Qt::Key_F13, 0x5d);
    scanCodeHash.insert(Qt::Key_F14, 0x5e);
    scanCodeHash.insert(Qt::Key_F15, 0x5f);
    scanCodeHash.insert(Qt::Key_F16, 0x55);
    scanCodeHash.insert(Qt::Key_F17, 0x0103);
    scanCodeHash.insert(Qt::Key_F18, 0x0177);
    scanCodeHash.insert(Qt::Key_F19, 0x0104);
    scanCodeHash.insert(Qt::Key_F20, 0x5a);
    scanCodeHash.insert(Qt::Key_F21, 0x74);
    scanCodeHash.insert(Qt::Key_F22, 0x0179);
    scanCodeHash.insert(Qt::Key_F23, 0x6d);
    scanCodeHash.insert(Qt::Key_F24, 0x6f);
    //scanCodeHash.insert(Qt::Key_F25, 0x00);
    //scanCodeHash.insert(Qt::Key_F26, 0x00);
    //scanCodeHash.insert(Qt::Key_F27, 0x00);
    //scanCodeHash.insert(Qt::Key_F28, 0x00);
    //scanCodeHash.insert(Qt::Key_F29, 0x00);
    //scanCodeHash.insert(Qt::Key_F30, 0x00);
    //scanCodeHash.insert(Qt::Key_F31, 0x00);
    //scanCodeHash.insert(Qt::Key_F32, 0x00);
    //scanCodeHash.insert(Qt::Key_F33, 0x00);
    //scanCodeHash.insert(Qt::Key_F34, 0x00);
    //scanCodeHash.insert(Qt::Key_F35, 0x00);
    //scanCodeHash.insert(Qt::Key_Super_L, 0x00);
    //scanCodeHash.insert(Qt::Key_Super_R, 0x00);
    scanCodeHash.insert(Qt::Key_Menu, 0x015d);  // KEY_COMPOSE
    //scanCodeHash.insert(Qt::Key_Hyper_L, 0x00);
    //scanCodeHash.insert(Qt::Key_Hyper_R, 0x00);
    scanCodeHash.insert(Qt::Key_Help, 0x0175);
    scanCodeHash.insert(Qt::Key_Direction_L, 0x6b); // KEY_DIRECTION
    scanCodeHash.insert(Qt::Key_Direction_R, 0x6b); // KEY_DIRECTION
    scanCodeHash.insert(Qt::Key_Space, 0x39);
    scanCodeHash.insert(Qt::Key_Any, 0x39);     // Qt::Key_Space
    scanCodeHash.insert(Qt::Key_Exclam, 0x02);
    scanCodeHash.insert(Qt::Key_QuoteDbl, 0x28);
    scanCodeHash.insert(Qt::Key_NumberSign, 0x04);
    scanCodeHash.insert(Qt::Key_Dollar, 0x05);
    scanCodeHash.insert(Qt::Key_Percent, 0x06);
    scanCodeHash.insert(Qt::Key_Ampersand, 0x08);
    scanCodeHash.insert(Qt::Key_Apostrophe, 0x28);
    scanCodeHash.insert(Qt::Key_ParenLeft, 0x0a);
    scanCodeHash.insert(Qt::Key_ParenRight, 0x0b);
    scanCodeHash.insert(Qt::Key_Asterisk, 0x09);
    scanCodeHash.insert(Qt::Key_Plus, 0x0d);
    scanCodeHash.insert(Qt::Key_Comma, 0x33);
    scanCodeHash.insert(Qt::Key_Minus, 0x0c);
    scanCodeHash.insert(Qt::Key_Period, 0x34);
    scanCodeHash.insert(Qt::Key_Slash, 0x35);
    scanCodeHash.insert(Qt::Key_0, 0x0b);
    scanCodeHash.insert(Qt::Key_1, 0x02);
    scanCodeHash.insert(Qt::Key_2, 0x03);
    scanCodeHash.insert(Qt::Key_3, 0x04);
    scanCodeHash.insert(Qt::Key_4, 0x05);
    scanCodeHash.insert(Qt::Key_5, 0x06);
    scanCodeHash.insert(Qt::Key_6, 0x07);
    scanCodeHash.insert(Qt::Key_7, 0x08);
    scanCodeHash.insert(Qt::Key_8, 0x09);
    scanCodeHash.insert(Qt::Key_9, 0x0a);
    scanCodeHash.insert(Qt::Key_Colon, 0x27);
    scanCodeHash.insert(Qt::Key_Semicolon, 0x27);
    scanCodeHash.insert(Qt::Key_Less, 0x33);
    scanCodeHash.insert(Qt::Key_Equal, 0x0d);
    scanCodeHash.insert(Qt::Key_Greater, 0x34);
    scanCodeHash.insert(Qt::Key_Question, 0x35);
    scanCodeHash.insert(Qt::Key_At, 0x03);
    scanCodeHash.insert(Qt::Key_A, 0x1e);
    scanCodeHash.insert(Qt::Key_B, 0x30);
    scanCodeHash.insert(Qt::Key_C, 0x2e);
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
    scanCodeHash.insert(Qt::Key_S, 0x1f);
    scanCodeHash.insert(Qt::Key_T, 0x14);
    scanCodeHash.insert(Qt::Key_U, 0x16);
    scanCodeHash.insert(Qt::Key_V, 0x2f);
    scanCodeHash.insert(Qt::Key_W, 0x11);
    scanCodeHash.insert(Qt::Key_X, 0x2d);
    scanCodeHash.insert(Qt::Key_Y, 0x15);
    scanCodeHash.insert(Qt::Key_Z, 0x2c);
    scanCodeHash.insert(Qt::Key_BracketLeft, 0x1a);
    scanCodeHash.insert(Qt::Key_Backslash, 0x2b);
    scanCodeHash.insert(Qt::Key_BracketRight, 0x1b);
    scanCodeHash.insert(Qt::Key_AsciiCircum, 0x07);
    scanCodeHash.insert(Qt::Key_Underscore, 0x0c);
    scanCodeHash.insert(Qt::Key_QuoteLeft, 0x29);
    scanCodeHash.insert(Qt::Key_BraceLeft, 0x1a);
    scanCodeHash.insert(Qt::Key_Bar, 0x2b);
    scanCodeHash.insert(Qt::Key_BraceRight, 0x1b);
    scanCodeHash.insert(Qt::Key_AsciiTilde, 0x29);
    //scanCodeHash.insert(Qt::Key_nobreakspace, 0x00);
    //scanCodeHash.insert(Qt::Key_exclamdown, 0x00);
    //scanCodeHash.insert(Qt::Key_cent, 0x00);
    //scanCodeHash.insert(Qt::Key_sterling, 0x00);
    //scanCodeHash.insert(Qt::Key_currency, 0x00);
    scanCodeHash.insert(Qt::Key_yen, 0x7d);
    //scanCodeHash.insert(Qt::Key_brokenbar, 0x00);
    //scanCodeHash.insert(Qt::Key_section, 0x00);
    //scanCodeHash.insert(Qt::Key_diaeresis, 0x00);
    //scanCodeHash.insert(Qt::Key_copyright, 0x00);
    //scanCodeHash.insert(Qt::Key_ordfeminine, 0x00);
    //scanCodeHash.insert(Qt::Key_guillemotleft, 0x00);
    //scanCodeHash.insert(Qt::Key_notsign, 0x00);
    //scanCodeHash.insert(Qt::Key_hyphen, 0x00);
    //scanCodeHash.insert(Qt::Key_registered, 0x00);
    //scanCodeHash.insert(Qt::Key_macron, 0x00);
    //scanCodeHash.insert(Qt::Key_degree, 0x00);
    //scanCodeHash.insert(Qt::Key_plusminus, 0x00);
    //scanCodeHash.insert(Qt::Key_twosuperior, 0x00);
    //scanCodeHash.insert(Qt::Key_threesuperior, 0x00);
    //scanCodeHash.insert(Qt::Key_acute, 0x00);
    //scanCodeHash.insert(Qt::Key_mu, 0x00);
    //scanCodeHash.insert(Qt::Key_paragraph, 0x00);
    //scanCodeHash.insert(Qt::Key_periodcentered, 0x00);
    //scanCodeHash.insert(Qt::Key_cedilla, 0x00);
    //scanCodeHash.insert(Qt::Key_onesuperior, 0x00);
    //scanCodeHash.insert(Qt::Key_masculine, 0x00);
    //scanCodeHash.insert(Qt::Key_guillemotright, 0x00);
    //scanCodeHash.insert(Qt::Key_onequarter, 0x00);
    //scanCodeHash.insert(Qt::Key_onehalf, 0x00);
    //scanCodeHash.insert(Qt::Key_threequarters, 0x00);
    //scanCodeHash.insert(Qt::Key_questiondown, 0x00);
    //scanCodeHash.insert(Qt::Key_Agrave, 0x00);
    //scanCodeHash.insert(Qt::Key_Aacute, 0x00);
    //scanCodeHash.insert(Qt::Key_Acircumflex, 0x00);
    //scanCodeHash.insert(Qt::Key_Atilde, 0x00);
    //scanCodeHash.insert(Qt::Key_Adiaeresis, 0x00);
    //scanCodeHash.insert(Qt::Key_Aring, 0x00);
    //scanCodeHash.insert(Qt::Key_AE, 0x00);
    //scanCodeHash.insert(Qt::Key_Ccedilla, 0x00);
    //scanCodeHash.insert(Qt::Key_Egrave, 0x00);
    //scanCodeHash.insert(Qt::Key_Eacute, 0x00);
    //scanCodeHash.insert(Qt::Key_Ecircumflex, 0x00);
    //scanCodeHash.insert(Qt::Key_Ediaeresis, 0x00);
    //scanCodeHash.insert(Qt::Key_Igrave, 0x00);
    //scanCodeHash.insert(Qt::Key_Iacute, 0x00);
    //scanCodeHash.insert(Qt::Key_Icircumflex, 0x00);
    //scanCodeHash.insert(Qt::Key_Idiaeresis, 0x00);
    //scanCodeHash.insert(Qt::Key_ETH, 0x00);
    //scanCodeHash.insert(Qt::Key_Ntilde, 0x00);
    //scanCodeHash.insert(Qt::Key_Ograve, 0x00);
    //scanCodeHash.insert(Qt::Key_Oacute, 0x00);
    //scanCodeHash.insert(Qt::Key_Ocircumflex, 0x00);
    //scanCodeHash.insert(Qt::Key_Otilde, 0x00);
    //scanCodeHash.insert(Qt::Key_Odiaeresis, 0x00);
    scanCodeHash.insert(Qt::Key_multiply, 0x0137);  // KeyPad multiply
    //scanCodeHash.insert(Qt::Key_Ooblique, 0x00);
    //scanCodeHash.insert(Qt::Key_Ugrave, 0x00);
    //scanCodeHash.insert(Qt::Key_Uacute, 0x00);
    //scanCodeHash.insert(Qt::Key_Ucircumflex, 0x00);
    //scanCodeHash.insert(Qt::Key_Udiaeresis, 0x00);
    //scanCodeHash.insert(Qt::Key_Yacute, 0x00);
    //scanCodeHash.insert(Qt::Key_THORN, 0x00);
    //scanCodeHash.insert(Qt::Key_ssharp, 0x00);
    //scanCodeHash.insert(Qt::Key_division, 0x00);
    //scanCodeHash.insert(Qt::Key_ydiaeresis, 0x00);
    //scanCodeHash.insert(Qt::Key_Multi_key, 0x00);
    //scanCodeHash.insert(Qt::Key_Codeinput, 0x00);
    //scanCodeHash.insert(Qt::Key_SingleCandidate, 0x00);
    //scanCodeHash.insert(Qt::Key_MultipleCandidate, 0x00);
    //scanCodeHash.insert(Qt::Key_PreviousCandidate, 0x00);
    scanCodeHash.insert(Qt::Key_Mode_switch, 0x0156);   // KEY_SWITCHVIDEOMODE
    //scanCodeHash.insert(Qt::Key_Kanji, 0x00);
    scanCodeHash.insert(Qt::Key_Muhenkan, 0x7b);
    scanCodeHash.insert(Qt::Key_Henkan, 0x79);
    scanCodeHash.insert(Qt::Key_Romaji, 0x00);
    scanCodeHash.insert(Qt::Key_Hiragana, 0x77);
    scanCodeHash.insert(Qt::Key_Katakana, 0x78);
    scanCodeHash.insert(Qt::Key_Hiragana_Katakana, 0x70);
    //scanCodeHash.insert(Qt::Key_Zenkaku, 0x00);
    //scanCodeHash.insert(Qt::Key_Hankaku, 0x00);
    scanCodeHash.insert(Qt::Key_Zenkaku_Hankaku, 0x76);
    //scanCodeHash.insert(Qt::Key_Touroku, 0x00);
    //scanCodeHash.insert(Qt::Key_Massyo, 0x00);
    //scanCodeHash.insert(Qt::Key_Kana_Lock, 0x00);
    //scanCodeHash.insert(Qt::Key_Kana_Shift, 0x00);
    //scanCodeHash.insert(Qt::Key_Eisu_Shift, 0x00);
    //scanCodeHash.insert(Qt::Key_Eisu_toggle, 0x00);
    //scanCodeHash.insert(Qt::Key_Hangul, 0x00);
    //scanCodeHash.insert(Qt::Key_Hangul_Start, 0x00);
    //scanCodeHash.insert(Qt::Key_Hangul_End, 0x00);
    scanCodeHash.insert(Qt::Key_Hangul_Hanja, 0x010d);  // KEY_HANJA
    //scanCodeHash.insert(Qt::Key_Hangul_Jamo, 0x00);
    //scanCodeHash.insert(Qt::Key_Hangul_Romaja, 0x00);
    //scanCodeHash.insert(Qt::Key_Hangul_Jeonja, 0x00);
    //scanCodeHash.insert(Qt::Key_Hangul_Banja, 0x00);
    //scanCodeHash.insert(Qt::Key_Hangul_PreHanja, 0x00);
    //scanCodeHash.insert(Qt::Key_Hangul_PostHanja, 0x00);
    //scanCodeHash.insert(Qt::Key_Hangul_Special, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Grave, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Acute, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Circumflex, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Tilde, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Macron, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Breve, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Abovedot, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Diaeresis, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Abovering, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Doubleacute, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Caron, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Cedilla, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Ogonek, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Iota, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Voiced_Sound, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Semivoiced_Sound, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Belowdot, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Hook, 0x00);
    //scanCodeHash.insert(Qt::Key_Dead_Horn, 0x00);
    scanCodeHash.insert(Qt::Key_Back, 0x016a);
    scanCodeHash.insert(Qt::Key_Forward, 0x0169);
    scanCodeHash.insert(Qt::Key_Stop, 0x0168);
    scanCodeHash.insert(Qt::Key_Refresh, 0x0167);   // KEY_REFRESH
    scanCodeHash.insert(Qt::Key_VolumeDown, 0x012e);
    scanCodeHash.insert(Qt::Key_VolumeMute, 0x0120);
    scanCodeHash.insert(Qt::Key_VolumeUp, 0x0130);
    scanCodeHash.insert(Qt::Key_BassBoost, 0x0136);
    //scanCodeHash.insert(Qt::Key_BassUp, 0x00);
    //scanCodeHash.insert(Qt::Key_BassDown, 0x00);
    //scanCodeHash.insert(Qt::Key_TrebleUp, 0x00);
    //scanCodeHash.insert(Qt::Key_TrebleDown, 0x00);
    scanCodeHash.insert(Qt::Key_MediaPlay, 0x0128); // KEY_PLAYCD
    scanCodeHash.insert(Qt::Key_MediaStop, 0x0124); // KEY_STOPCD
    scanCodeHash.insert(Qt::Key_MediaPrevious, 0x0110); // KEY_PREVIOUSSONG
    scanCodeHash.insert(Qt::Key_MediaNext, 0x0119); // KEY_NEXTSONG
    scanCodeHash.insert(Qt::Key_MediaRecord, 0x0131);   // KEY_RECORD
    scanCodeHash.insert(Qt::Key_MediaPause, 0x0129);  // KEY_PAUSECD
    scanCodeHash.insert(Qt::Key_MediaTogglePlayPause, 0x0122);    // KEY_PLAYPAUSE
    scanCodeHash.insert(Qt::Key_HomePage, 0x0132);
    scanCodeHash.insert(Qt::Key_Favorites, 0x0166); // KEY_BOOKMARKS
    scanCodeHash.insert(Qt::Key_Search, 0x0165);
    //scanCodeHash.insert(Qt::Key_Standby, 0x00);
    //scanCodeHash.insert(Qt::Key_OpenUrl, 0x00);
    scanCodeHash.insert(Qt::Key_LaunchMail, 0x016c);    // KEY_MAIL
    scanCodeHash.insert(Qt::Key_LaunchMedia, 0x016d);   // KEY_MEDIA
    scanCodeHash.insert(Qt::Key_Launch0, 0x016b);   // KEY_COMPUTER
    //scanCodeHash.insert(Qt::Key_Launch1, 0x00);
    //scanCodeHash.insert(Qt::Key_Launch2, 0x00);
    //scanCodeHash.insert(Qt::Key_Launch3, 0x00);
    //scanCodeHash.insert(Qt::Key_Launch4, 0x00);
    //scanCodeHash.insert(Qt::Key_Launch5, 0x00);
    //scanCodeHash.insert(Qt::Key_Launch6, 0x00);
    //scanCodeHash.insert(Qt::Key_Launch7, 0x00);
    //scanCodeHash.insert(Qt::Key_Launch8, 0x00);
    //scanCodeHash.insert(Qt::Key_Launch9, 0x00);
    //scanCodeHash.insert(Qt::Key_LaunchA, 0x00);
    //scanCodeHash.insert(Qt::Key_LaunchB, 0x00);
    //scanCodeHash.insert(Qt::Key_LaunchC, 0x00);
    //scanCodeHash.insert(Qt::Key_LaunchD, 0x00);
    //scanCodeHash.insert(Qt::Key_LaunchE, 0x00);
    //scanCodeHash.insert(Qt::Key_LaunchF, 0x00);
    //scanCodeHash.insert(Qt::Key_LaunchG, 0x00);
    //scanCodeHash.insert(Qt::Key_LaunchH, 0x00);
    scanCodeHash.insert(Qt::Key_MonBrightnessUp, 0x0154);
    scanCodeHash.insert(Qt::Key_MonBrightnessDown, 0x014c);
    scanCodeHash.insert(Qt::Key_KeyboardLightOnOff, 0x0157);    // KEY_KBDILLUMTOGGLE
    scanCodeHash.insert(Qt::Key_KeyboardBrightnessUp, 0x0159);  // KEY_KBDILLUMUP
    scanCodeHash.insert(Qt::Key_KeyboardBrightnessDown, 0x0158);    // KEY_KBDILLUMDOWN
    scanCodeHash.insert(Qt::Key_PowerOff, 0x015e);      // KEY_POWER
    scanCodeHash.insert(Qt::Key_WakeUp, 0x0163);
    scanCodeHash.insert(Qt::Key_Eject, 0x017d);     // KEY_EJECTCLOSECD
    scanCodeHash.insert(Qt::Key_ScreenSaver, 0x0112);   // KEY_SCREENLOCK
    scanCodeHash.insert(Qt::Key_WWW, 0x0102);
    //scanCodeHash.insert(Qt::Key_Memo, 0x00);
    //scanCodeHash.insert(Qt::Key_LightBulb, 0x00);
    scanCodeHash.insert(Qt::Key_Shop, 0x0145);
    //scanCodeHash.insert(Qt::Key_History, 0x00);
    //scanCodeHash.insert(Qt::Key_AddFavorite, 0x00);
    //scanCodeHash.insert(Qt::Key_HotLinks, 0x00);
    //scanCodeHash.insert(Qt::Key_BrightnessAdjust, 0x00);
    scanCodeHash.insert(Qt::Key_Finance, 0x0143);
    //scanCodeHash.insert(Qt::Key_Community, 0x00);
    scanCodeHash.insert(Qt::Key_AudioRewind, 0x0118);   // KEY_REWIND
    //scanCodeHash.insert(Qt::Key_BackForward, 0x00);
    //scanCodeHash.insert(Qt::Key_ApplicationLeft, 0x00);
    //scanCodeHash.insert(Qt::Key_ApplicationRight, 0x00);
    //scanCodeHash.insert(Qt::Key_Book, 0x00);
    //scanCodeHash.insert(Qt::Key_CD, 0x00);
    scanCodeHash.insert(Qt::Key_Calculator, 0x0121);
    //scanCodeHash.insert(Qt::Key_ToDoList, 0x00);
    //scanCodeHash.insert(Qt::Key_ClearGrab, 0x00);
    scanCodeHash.insert(Qt::Key_Close, 0x012f);
    scanCodeHash.insert(Qt::Key_Copy, 0x0178);
    scanCodeHash.insert(Qt::Key_Cut, 0x013c);
    //scanCodeHash.insert(Qt::Key_Display, 0x00);
    scanCodeHash.insert(Qt::Key_DOS, 0x6a);
    scanCodeHash.insert(Qt::Key_Documents, 0x0170);
    //scanCodeHash.insert(Qt::Key_Excel, 0x00);
    //scanCodeHash.insert(Qt::Key_Explorer, 0x00);
    //scanCodeHash.insert(Qt::Key_Game, 0x00);
    //scanCodeHash.insert(Qt::Key_Go, 0x00);
    //scanCodeHash.insert(Qt::Key_iTouch, 0x00);
    //scanCodeHash.insert(Qt::Key_LogOff, 0x00);
    //scanCodeHash.insert(Qt::Key_Market, 0x00);
    //scanCodeHash.insert(Qt::Key_Meeting, 0x00);
    //scanCodeHash.insert(Qt::Key_MenuKB, 0x00);
    //scanCodeHash.insert(Qt::Key_MenuPB, 0x00);
    //scanCodeHash.insert(Qt::Key_MySites, 0x00);
    //scanCodeHash.insert(Qt::Key_News, 0x00);
    //scanCodeHash.insert(Qt::Key_OfficeHome, 0x00);
    //scanCodeHash.insert(Qt::Key_Option, 0x00);
    scanCodeHash.insert(Qt::Key_Paste, 0x65);
    scanCodeHash.insert(Qt::Key_Phone, 0x63);
    //scanCodeHash.insert(Qt::Key_Calendar, 0x00);
    scanCodeHash.insert(Qt::Key_Reply, 0x0164);
    //scanCodeHash.insert(Qt::Key_Reload, 0x00);
    //scanCodeHash.insert(Qt::Key_RotateWindows, 0x00);
    //scanCodeHash.insert(Qt::Key_RotationPB, 0x00);
    //scanCodeHash.insert(Qt::Key_RotationKB, 0x00);
    scanCodeHash.insert(Qt::Key_Save, 0x0155);
    scanCodeHash.insert(Qt::Key_Send, 0x015a);
    //scanCodeHash.insert(Qt::Key_Spell, 0x00);
    //scanCodeHash.insert(Qt::Key_SplitScreen, 0x00);
    //scanCodeHash.insert(Qt::Key_Support, 0x00);
    //scanCodeHash.insert(Qt::Key_TaskPane, 0x00);
    //scanCodeHash.insert(Qt::Key_Terminal, 0x00);
    //scanCodeHash.insert(Qt::Key_Tools, 0x00);
    //scanCodeHash.insert(Qt::Key_Travel, 0x00);
    //scanCodeHash.insert(Qt::Key_Video, 0x00);
    //scanCodeHash.insert(Qt::Key_Word, 0x00);
    scanCodeHash.insert(Qt::Key_Xfer, 0x0113);
    //scanCodeHash.insert(Qt::Key_ZoomIn, 0x00);
    //scanCodeHash.insert(Qt::Key_ZoomOut, 0x00);
    //scanCodeHash.insert(Qt::Key_Away, 0x00);
    scanCodeHash.insert(Qt::Key_Messenger, 0x0140); // KEY_CHAT
    //scanCodeHash.insert(Qt::Key_WebCam, 0x00);
    scanCodeHash.insert(Qt::Key_MailForward, 0x010e);
    //scanCodeHash.insert(Qt::Key_Pictures, 0x00);
    //scanCodeHash.insert(Qt::Key_Music, 0x00);
    scanCodeHash.insert(Qt::Key_Battery, 0x0171);
    scanCodeHash.insert(Qt::Key_Bluetooth, 0x0172);
    scanCodeHash.insert(Qt::Key_WLAN, 0x0173);
    scanCodeHash.insert(Qt::Key_UWB, 0x0174);
    //scanCodeHash.insert(Qt::Key_AudioForward, 0x00);
    //scanCodeHash.insert(Qt::Key_AudioRepeat, 0x00);
    //scanCodeHash.insert(Qt::Key_AudioRandomPlay, 0x00);
    //scanCodeHash.insert(Qt::Key_Subtitle, 0x00);
    //scanCodeHash.insert(Qt::Key_AudioCycleTrack, 0x00);
    //scanCodeHash.insert(Qt::Key_Time, 0x00);
    //scanCodeHash.insert(Qt::Key_Hibernate, 0x00);
    //scanCodeHash.insert(Qt::Key_View, 0x00);
    //scanCodeHash.insert(Qt::Key_TopMenu, 0x00);
    //scanCodeHash.insert(Qt::Key_PowerDown, 0x00);
    scanCodeHash.insert(Qt::Key_Suspend, 0x0125);
    //scanCodeHash.insert(Qt::Key_ContrastAdjust, 0x00);
    //scanCodeHash.insert(Qt::Key_TouchpadToggle, 0x00);
    //scanCodeHash.insert(Qt::Key_TouchpadOn, 0x00);
    //scanCodeHash.insert(Qt::Key_TouchpadOff, 0x00);
    //scanCodeHash.insert(Qt::Key_MicMute, 0x00);
    //scanCodeHash.insert(Qt::Key_Red, 0x00);
    //scanCodeHash.insert(Qt::Key_Green, 0x00);
    //scanCodeHash.insert(Qt::Key_Yellow, 0x00);
    //scanCodeHash.insert(Qt::Key_Blue, 0x00);
    //scanCodeHash.insert(Qt::Key_ChannelUp, 0x00);
    //scanCodeHash.insert(Qt::Key_ChannelDown, 0x00);
    //scanCodeHash.insert(Qt::Key_Guide, 0x00);
    //scanCodeHash.insert(Qt::Key_Info, 0x00);
    //scanCodeHash.insert(Qt::Key_Settings, 0x00);
    //scanCodeHash.insert(Qt::Key_MicVolumeUp, 0x00);
    //scanCodeHash.insert(Qt::Key_MicVolumeDown, 0x00);
    scanCodeHash.insert(Qt::Key_New, 0x0109);
    scanCodeHash.insert(Qt::Key_Open, 0x64);
    scanCodeHash.insert(Qt::Key_Find, 0x0141);
    scanCodeHash.insert(Qt::Key_Undo, 0x0107);
    scanCodeHash.insert(Qt::Key_Redo, 0x010a);
    //scanCodeHash.insert(Qt::Key_MediaLast, 0x00);
    //scanCodeHash.insert(Qt::Key_unknown, 0x00);
    //scanCodeHash.insert(Qt::Key_Call, 0x00);
    scanCodeHash.insert(Qt::Key_Camera, 0x013b);
    //scanCodeHash.insert(Qt::Key_CameraFocus, 0x00);
    //scanCodeHash.insert(Qt::Key_Context1, 0x00);
    //scanCodeHash.insert(Qt::Key_Context2, 0x00);
    //scanCodeHash.insert(Qt::Key_Context3, 0x00);
    //scanCodeHash.insert(Qt::Key_Context4, 0x00);
    //scanCodeHash.insert(Qt::Key_Flip, 0x00);
    //scanCodeHash.insert(Qt::Key_Hangup, 0x00);
    //scanCodeHash.insert(Qt::Key_No, 0x00);
    //scanCodeHash.insert(Qt::Key_Select, 0x00);
    //scanCodeHash.insert(Qt::Key_Yes, 0x00);
    //scanCodeHash.insert(Qt::Key_ToggleCallHangup, 0x00);
    //scanCodeHash.insert(Qt::Key_VoiceDial, 0x00);
    //scanCodeHash.insert(Qt::Key_LastNumberRedial, 0x00);
    //scanCodeHash.insert(Qt::Key_Execute, 0x00);
    //scanCodeHash.insert(Qt::Key_Printer, 0x00);
    scanCodeHash.insert(Qt::Key_Play, 0x0133);
    scanCodeHash.insert(Qt::Key_Sleep, 0x015f);
    //scanCodeHash.insert(Qt::Key_Zoom, 0x00);
    scanCodeHash.insert(Qt::Key_Exit, 0x71);
    scanCodeHash.insert(Qt::Key_Cancel, 0x014a);
}

void QSpiceInputsChannel::initKeypadScanCodeMap()
{
    keypadScanCodeHash.clear();
    keypadScanCodeHash.insert(Qt::Key_Slash, 0x0135);   // KEY_KPSLASH
    keypadScanCodeHash.insert(Qt::Key_Asterisk, 0x37);  // KEY_KPASTERISK
    keypadScanCodeHash.insert(Qt::Key_Home, 0x0147);
    keypadScanCodeHash.insert(Qt::Key_Up, 0x0148);
    keypadScanCodeHash.insert(Qt::Key_PageUp, 0x0149);
    keypadScanCodeHash.insert(Qt::Key_Left, 0x014b);
    keypadScanCodeHash.insert(Qt::Key_Right, 0x014d);
    keypadScanCodeHash.insert(Qt::Key_End, 0x014f);
    keypadScanCodeHash.insert(Qt::Key_Down, 0x0150);
    keypadScanCodeHash.insert(Qt::Key_PageDown, 0x0151);
    keypadScanCodeHash.insert(Qt::Key_Insert, 0x0152);
    keypadScanCodeHash.insert(Qt::Key_Delete, 0x0153);
    keypadScanCodeHash.insert(Qt::Key_Enter, 0x011c);
    keypadScanCodeHash.insert(Qt::Key_7, 0x47);
    keypadScanCodeHash.insert(Qt::Key_8, 0x48);
    keypadScanCodeHash.insert(Qt::Key_9, 0x49);
    keypadScanCodeHash.insert(Qt::Key_Minus, 0x4a);
    keypadScanCodeHash.insert(Qt::Key_4, 0x4b);
    keypadScanCodeHash.insert(Qt::Key_5, 0x4c);
    keypadScanCodeHash.insert(Qt::Key_6, 0x4d);
    keypadScanCodeHash.insert(Qt::Key_Plus, 0x4e);
    keypadScanCodeHash.insert(Qt::Key_1, 0x4f);
    keypadScanCodeHash.insert(Qt::Key_2, 0x50);
    keypadScanCodeHash.insert(Qt::Key_3, 0x51);
    keypadScanCodeHash.insert(Qt::Key_0, 0x52);
    keypadScanCodeHash.insert(Qt::Key_Period, 0x53);    // KEY_KPDOT
    keypadScanCodeHash.insert(Qt::Key_Return, 0x011c);  // Qt::Key_Enter
    keypadScanCodeHash.insert(Qt::Key_Equal, 0x59);     // KEY_KPEQUAL
    keypadScanCodeHash.insert(Qt::Key_Comma, 0x7e);     // KEY_KPCOMMA
    keypadScanCodeHash.insert(Qt::Key_ParenLeft, 0x0176); // KEY_KPLEFTPAREN
    keypadScanCodeHash.insert(Qt::Key_ParenRight, 0x017b);// KEY_KPRIGHTPAREN
}

void QSpiceInputsChannel::initSequenceScanCodeMap()
{
    // Sequence Scan Code Hash
    #define ADD_SCAN1(q, s1)    sequenceCodeHash.insert(q, s1);
    #define ADD_SCAN2(q, s1, s2)    sequenceCodeHash.insert(q, QScanCodeArray(s1, s2));
    #define ADD_SCAN3(q, s1, s2, s3)    sequenceCodeHash.insert(q, QScanCodeArray(s1, s2, s3));
    #define ADD_SCAN4(q, s1, s2, s3, s4)    sequenceCodeHash.insert(q, QScanCodeArray(s1, s2, s3, s4));

    // additional non-keyboard called keys :
    // not applicable in PC AT keyboard,
    // therefore used for send key sequence
    ADD_SCAN3   (Qt::Key_Launch1, 0x1D, 0x38, 0x3B);          // CtrlAltF1
    ADD_SCAN3   (Qt::Key_Launch2, 0x1D, 0x38, 0x3C);          // CtrlAltF2
    ADD_SCAN3   (Qt::Key_Launch3, 0x1D, 0x38, 0x3D);          // CtrlAltF3
    ADD_SCAN3   (Qt::Key_Launch4, 0x1D, 0x38, 0x3E);          // CtrlAltF4
    ADD_SCAN3   (Qt::Key_Launch5, 0x1D, 0x38, 0x3F);          // CtrlAltF5
    ADD_SCAN3   (Qt::Key_Launch6, 0x1D, 0x38, 0x40);          // CtrlAltF6
    ADD_SCAN3   (Qt::Key_Launch7, 0x1D, 0x38, 0x41);          // CtrlAltF7
    ADD_SCAN3   (Qt::Key_Launch8, 0x1D, 0x38, 0x42);          // CtrlAltF8
    ADD_SCAN3   (Qt::Key_LaunchB, 0x1D, 0x38, 0x0E);          // CtrlAltBackSpc
    ADD_SCAN4   (Qt::Key_LaunchD, 0x1D, 0x38, 0xE0, 0x53);    // CtrlAltDel
}

// sends scancodes to inputs-channel
void QSpiceInputsChannel::inputsQKeyPress(int key)
{
    inputsKeyPress(scanCodeHash.value(key));
}

void QSpiceInputsChannel::inputsQKeyRelease(int key)
{
    inputsKeyRelease(scanCodeHash.value(key));
}

void QSpiceInputsChannel::inputsQKeypadKeyPress(int key)
{
    inputsKeyPress(keypadScanCodeHash.value(key));
}

void QSpiceInputsChannel::inputsQKeypadKeyRelease(int key)
{
    inputsKeyRelease(keypadScanCodeHash.value(key));
}

QScanCodeArray QSpiceInputsChannel::QSequenceKeyToScanCode(int key)
{
    ScanCodeHash::iterator it = sequenceCodeHash.find(key);
    if (it == sequenceCodeHash.end())
        return QScanCodeArray();
    else
        return it.value();
}

void QSpiceInputsChannel::inputsQSequenceKeyPress(int key)
{
    QScanCodeArray scanCode = QSequenceKeyToScanCode(key);
    for (int i = 0; i < scanCode.count(); i++)
        inputsKeyPress(scanCode[i]);
}

void QSpiceInputsChannel::inputsQSequenceKeyRelease(int key)
{
    QScanCodeArray scanCode = QSequenceKeyToScanCode(key);
    for (int i = 0; i < scanCode.count(); i++)
        inputsKeyRelease(scanCode[i]);
}
