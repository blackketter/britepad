#ifndef _KeyInfo_
#define _KeyInfo_
#include "keylayouts.h"
#include "widgets/Icons.h"

typedef uint16_t keycode_t;

// some new key codes
const keycode_t NO_CODE = 0;
const keycode_t KEY_LEFT_FN = 200;
const keycode_t KEY_RIGHT_FN = 201;

const keycode_t KEY_TOP_PAD = 202;
const keycode_t KEY_BOTTOM_PAD = 203;
const keycode_t KEY_LEFT_PAD = 204;
const keycode_t KEY_RIGHT_PAD = 205;

const keycode_t KEY_EXIT = 206;
const keycode_t KEY_LAUNCHBAR = 207;

typedef struct keyinfo_t {
  keycode_t code;
  char c;
  const char* label;
  const icon_t icon;
  bool modifier;
} keyinfo_t;

const keyinfo_t keyInfo[] = {
// nonstandard keys
  { KEY_LEFT_FN, 0, "fn", nullptr, true },
  { KEY_RIGHT_FN, 0, "fn", nullptr, true },
// standard keys
  { MODIFIERKEY_CTRL, 0, "ctrl", controlIcon, true },
  { MODIFIERKEY_SHIFT, 0, "shift", shiftIcon, true },
  { MODIFIERKEY_ALT, 0, "opt", optionIcon, true },
  { MODIFIERKEY_GUI, 0, "cmd", commandIcon, true },
  { MODIFIERKEY_LEFT_CTRL, 0, "ctrl", controlIcon, true },
  { MODIFIERKEY_LEFT_SHIFT, 0, "shift", shiftIcon, true },
  { MODIFIERKEY_LEFT_ALT, 0, "opt", optionIcon, true },
  { MODIFIERKEY_LEFT_GUI, 0, "cmd", commandIcon, true },
  { MODIFIERKEY_RIGHT_CTRL, 0, "ctrl", controlIcon, true },
  { MODIFIERKEY_RIGHT_SHIFT, 0, "shift", shiftIcon, true },
  { MODIFIERKEY_RIGHT_ALT, 0, "opt", optionIcon, true },
  { MODIFIERKEY_RIGHT_GUI, 0, "cmd", commandIcon, true },

  { KEY_SYSTEM_POWER_DOWN, 0, "pow", nullptr, false },
  { KEY_SYSTEM_SLEEP, 0, "sleep", nullptr, false },
  { KEY_SYSTEM_WAKE_UP, 0, "wake", nullptr, false },

  { KEY_MEDIA_PLAY, 0, "play", playIcon, false },
  { KEY_MEDIA_PAUSE, 0, "pause", pauseIcon, false },
  { KEY_MEDIA_RECORD, 0, "rec", recordIcon, false },
  { KEY_MEDIA_FAST_FORWARD, 0, "ffw", ffIcon, false },
  { KEY_MEDIA_REWIND, 0, "rew", rewIcon, false },
  { KEY_MEDIA_NEXT_TRACK, 0, "next", ffIcon, false },
  { KEY_MEDIA_PREV_TRACK, 0, "prev", rewIcon, false },
  { KEY_MEDIA_STOP, 0, "stop", stopIcon, false },
  { KEY_MEDIA_EJECT, 0, "eject", ejectIcon, false },
  { KEY_MEDIA_RANDOM_PLAY, 0, "random", nullptr, false },
  { KEY_MEDIA_PLAY_PAUSE, 0, "pause", pauseIcon, false },
  { KEY_MEDIA_PLAY_SKIP, 0, "skip", ffIcon, false },
  { KEY_MEDIA_MUTE, 0, "mute", muteIcon, false },
  { KEY_MEDIA_VOLUME_INC, 0, "vol+", volPlusIcon, false },
  { KEY_MEDIA_VOLUME_DEC, 0, "vol-", volMinusIcon, false },

  { KEY_A, 'A', "A", nullptr, false },
  { KEY_B, 'B', "B", nullptr, false },
  { KEY_C, 'C', "C", nullptr, false },
  { KEY_D, 'D', "D", nullptr, false },
  { KEY_E, 'E', "E", nullptr, false },
  { KEY_F, 'F', "F", nullptr, false },
  { KEY_G, 'G', "G", nullptr, false },
  { KEY_H, 'H', "H", nullptr, false },
  { KEY_I, 'I', "I", nullptr, false },
  { KEY_J, 'J', "J", nullptr, false },
  { KEY_K, 'K', "K", nullptr, false },
  { KEY_L, 'L', "L", nullptr, false },
  { KEY_M, 'M', "M", nullptr, false },
  { KEY_N, 'N', "N", nullptr, false },
  { KEY_O, 'O', "O", nullptr, false },
  { KEY_P, 'P', "P", nullptr, false },
  { KEY_Q, 'Q', "Q", nullptr, false },
  { KEY_R, 'R', "R", nullptr, false },
  { KEY_S, 'S', "S", nullptr, false },
  { KEY_T, 'T', "T", nullptr, false },
  { KEY_U, 'U', "U", nullptr, false },
  { KEY_V, 'V', "V", nullptr, false },
  { KEY_W, 'W', "W", nullptr, false },
  { KEY_X, 'X', "X", nullptr, false },
  { KEY_Y, 'Y', "Y", nullptr, false },
  { KEY_Z, 'Z', "Z", nullptr, false },
  { KEY_1, '1', "1", nullptr, false },
  { KEY_2, '2', "2", nullptr, false },
  { KEY_3, '3', "3", nullptr, false },
  { KEY_4, '4', "4", nullptr, false },
  { KEY_5, '5', "5", nullptr, false },
  { KEY_6, '6', "6", nullptr, false },
  { KEY_7, '7', "7", nullptr, false },
  { KEY_8, '8', "8", nullptr, false },
  { KEY_9, '9', "9", nullptr, false },
  { KEY_0, '0', "0", nullptr, false },
  { KEY_ENTER, 'A', "return", returnIcon, false },
  { KEY_ESC, 0x1b, "esc", escIcon, false },
  { KEY_BACKSPACE, 0x08, "bs", backspaceIcon, false },
  { KEY_TAB, '\t', "tab", tabIcon, false },
  { KEY_SPACE, ' ', " ", spaceIcon, false },
  { KEY_MINUS, '-', "-", nullptr, false },
  { KEY_EQUAL, '=', "=", nullptr, false },
  { KEY_LEFT_BRACE, '{', "{", nullptr, false },
  { KEY_RIGHT_BRACE, '}', "}", nullptr, false },
  { KEY_BACKSLASH, '\\', "\\", nullptr, false },
  { KEY_NON_US_NUM, 0, "num", nullptr, false },
  { KEY_SEMICOLON, ';', ";", nullptr, false },
  { KEY_QUOTE, '"', "\"", nullptr, false },
  { KEY_TILDE, '~', "~", nullptr, false },
  { KEY_COMMA, ',', ",", nullptr, false },
  { KEY_PERIOD, '.', ".", nullptr, false },
  { KEY_SLASH, '/', "/", nullptr, false },
  { KEY_CAPS_LOCK, 0, "capslock", nullptr, false },
  { KEY_F1, 0, "f1", nullptr, false },
  { KEY_F2, 0, "f2", nullptr, false },
  { KEY_F3, 0, "f3", nullptr, false },
  { KEY_F4, 0, "f4", nullptr, false },
  { KEY_F5, 0, "f5", nullptr, false },
  { KEY_F6, 0, "f6", nullptr, false },
  { KEY_F7, 0, "f7", nullptr, false },
  { KEY_F8, 0, "f8", nullptr, false },
  { KEY_F9, 0, "f9", nullptr, false },
  { KEY_F10, 0, "f10", nullptr, false },
  { KEY_F11, 0, "f11", nullptr, false },
  { KEY_F12, 0, "f12", nullptr, false },
  { KEY_PRINTSCREEN, 0, "prtsc", nullptr, false },
  { KEY_SCROLL_LOCK, 0, "scrlk", nullptr, false },
  { KEY_PAUSE, 0, "pause", nullptr, false },
  { KEY_INSERT, 0, "insert", nullptr, false },
  { KEY_HOME, 0, "home", homeIcon, false },
  { KEY_PAGE_UP, 0, "pgup", pageUpIcon, false },
  { KEY_DELETE, 0, "del", deleteIcon, false },
  { KEY_END, 0, "end", endIcon, false },
  { KEY_PAGE_DOWN, 0, "pgdn", pageDownIcon, false },
  { KEY_RIGHT, 0, "right", rightArrowIcon, false },
  { KEY_LEFT, 0, "left", leftArrowIcon, false },
  { KEY_DOWN, 0, "down", downArrowIcon, false },
  { KEY_UP, 0, "up", upArrowIcon, false },
  { KEY_NUM_LOCK, 0, "numlk", nullptr, false },
  { KEYPAD_SLASH, '/', "/", nullptr, false },
  { KEYPAD_ASTERIX, '*', "*", nullptr, false },
  { KEYPAD_MINUS, '-', "-", nullptr, false },
  { KEYPAD_PLUS, '+', "+", nullptr, false },
  { KEYPAD_ENTER, '\r', "enter", nullptr, false },
  { KEYPAD_1, '1', "1", nullptr, false },
  { KEYPAD_2, '2', "2", nullptr, false },
  { KEYPAD_3, '3', "3", nullptr, false },
  { KEYPAD_4, '4', "4", nullptr, false },
  { KEYPAD_5, '5', "5", nullptr, false },
  { KEYPAD_6, '6', "6", nullptr, false },
  { KEYPAD_7, '7', "7", nullptr, false },
  { KEYPAD_8, '8', "8", nullptr, false },
  { KEYPAD_9, '9', "9", nullptr, false },
  { KEYPAD_0, '0', "0", nullptr, false },
  { KEYPAD_PERIOD, '.', ".", nullptr, false },
  { KEY_NON_US_BS, 0x08, "backspace", backspaceIcon, false },
  { KEY_MENU, 0, "menu", nullptr, false },
  { KEY_F13, 0, "F13", nullptr, false },
  { KEY_F14, 0, "F14", nullptr, false },
  { KEY_F15, 0, "F15", nullptr, false },
  { KEY_F16, 0, "F16", nullptr, false },
  { KEY_F17, 0, "F17", nullptr, false },
  { KEY_F18, 0, "F18", nullptr, false },
  { KEY_F19, 0, "F19", nullptr, false },
  { KEY_F20, 0, "F20", nullptr, false },
  { KEY_F21, 0, "F21", nullptr, false },
  { KEY_F22, 0, "F22", nullptr, false },
  { KEY_F23, 0, "F23", nullptr, false },
  { KEY_F24, 0, "F24", nullptr, false },
  // end of table marker
  { NO_CODE, 0, nullptr, nullptr }
};

#endif // _KeyInfo_