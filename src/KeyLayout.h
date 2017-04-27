#ifndef _KeyLayout_
#define _KeyLayout_
#include "keylayouts.h"
#include "widgets/Icons.h"

typedef uint8_t keyswitch_t;
const keyswitch_t NO_KEY = 0xff;

typedef uint16_t keycode_t;
const keycode_t NO_CODE = 0;

// some new key codes
const keycode_t KEY_LEFT_FN = 200;
const keycode_t KEY_RIGHT_FN = 201;

typedef struct keylayout_t {
  keyswitch_t key;
  uint8_t x;
  uint8_t y;
  uint8_t w;
  uint8_t h;
} keylayout_t;

typedef struct keymap_t {
  keyswitch_t key;
  keycode_t code;
} keymap_t;

typedef struct keyinfo_t {
  keycode_t code;
  char c;
  const char* label;
  const icon_t icon;
} keyinfo_t;

const keyinfo_t keyInfo[] = {
// nonstandard keys
  { KEY_LEFT_FN, 0, "fn", nullptr },
  { KEY_RIGHT_FN, 0, "fn", nullptr },
// standard keys
  { MODIFIERKEY_CTRL, 0, "ctrl", controlIcon },
  { MODIFIERKEY_SHIFT, 0, "shift", shiftIcon },
  { MODIFIERKEY_ALT, 0, "opt", optionIcon },
  { MODIFIERKEY_GUI, 0, "cmd", commandIcon },
  { MODIFIERKEY_LEFT_CTRL, 0, "ctrl", controlIcon },
  { MODIFIERKEY_LEFT_SHIFT, 0, "shift", shiftIcon },
  { MODIFIERKEY_LEFT_ALT, 0, "opt", optionIcon },
  { MODIFIERKEY_LEFT_GUI, 0, "cmd", commandIcon },
  { MODIFIERKEY_RIGHT_CTRL, 0, "ctrl", controlIcon },
  { MODIFIERKEY_RIGHT_SHIFT, 0, "shift", shiftIcon },
  { MODIFIERKEY_RIGHT_ALT, 0, "opt", optionIcon },
  { MODIFIERKEY_RIGHT_GUI, 0, "cmd", commandIcon },

  { KEY_SYSTEM_POWER_DOWN, 0, "pow", nullptr },
  { KEY_SYSTEM_SLEEP, 0, "sleep", nullptr },
  { KEY_SYSTEM_WAKE_UP, 0, "wake", nullptr },

  { KEY_MEDIA_PLAY, 0, "play", playIcon },
  { KEY_MEDIA_PAUSE, 0, "pause", pauseIcon },
  { KEY_MEDIA_RECORD, 0, "rec", recordIcon },
  { KEY_MEDIA_FAST_FORWARD, 0, "ffw", ffIcon },
  { KEY_MEDIA_REWIND, 0, "rew", rewIcon },
  { KEY_MEDIA_NEXT_TRACK, 0, "next", ffIcon },
  { KEY_MEDIA_PREV_TRACK, 0, "prev", rewIcon },
  { KEY_MEDIA_STOP, 0, "stop", stopIcon },
  { KEY_MEDIA_EJECT, 0, "eject", ejectIcon },
  { KEY_MEDIA_RANDOM_PLAY, 0, "random", nullptr },
  { KEY_MEDIA_PLAY_PAUSE, 0, "pause", pauseIcon },
  { KEY_MEDIA_PLAY_SKIP, 0, "skip", ffIcon },
  { KEY_MEDIA_MUTE, 0, "mute", muteIcon },
  { KEY_MEDIA_VOLUME_INC, 0, "vol+", volPlusIcon },
  { KEY_MEDIA_VOLUME_DEC, 0, "vol-", volMinusIcon },

  { KEY_A, 'A', "A", nullptr },
  { KEY_B, 'B', "B", nullptr },
  { KEY_C, 'C', "C", nullptr },
  { KEY_D, 'D', "D", nullptr },
  { KEY_E, 'E', "E", nullptr },
  { KEY_F, 'F', "F", nullptr },
  { KEY_G, 'G', "G", nullptr },
  { KEY_H, 'H', "H", nullptr },
  { KEY_I, 'I', "I", nullptr },
  { KEY_J, 'J', "J", nullptr },
  { KEY_K, 'K', "K", nullptr },
  { KEY_L, 'L', "L", nullptr },
  { KEY_M, 'M', "M", nullptr },
  { KEY_N, 'N', "N", nullptr },
  { KEY_O, 'O', "O", nullptr },
  { KEY_P, 'P', "P", nullptr },
  { KEY_Q, 'Q', "Q", nullptr },
  { KEY_R, 'R', "R", nullptr },
  { KEY_S, 'S', "S", nullptr },
  { KEY_T, 'T', "T", nullptr },
  { KEY_U, 'U', "U", nullptr },
  { KEY_V, 'V', "V", nullptr },
  { KEY_W, 'W', "W", nullptr },
  { KEY_X, 'X', "X", nullptr },
  { KEY_Y, 'Y', "Y", nullptr },
  { KEY_Z, 'Z', "Z", nullptr },
  { KEY_1, '1', "1", nullptr },
  { KEY_2, '2', "2", nullptr },
  { KEY_3, '3', "3", nullptr },
  { KEY_4, '4', "4", nullptr },
  { KEY_5, '5', "5", nullptr },
  { KEY_6, '6', "6", nullptr },
  { KEY_7, '7', "7", nullptr },
  { KEY_8, '8', "8", nullptr },
  { KEY_9, '9', "9", nullptr },
  { KEY_0, '0', "0", nullptr },
  { KEY_ENTER, 'A', "return", returnIcon },
  { KEY_ESC, 0x1b, "esc", escIcon },
  { KEY_BACKSPACE, 0x08, "bs", backspaceIcon },
  { KEY_TAB, '\t', "tab", tabIcon },
  { KEY_SPACE, ' ', " ", nullptr },
  { KEY_MINUS, '-', "-", nullptr },
  { KEY_EQUAL, '=', "=", nullptr },
  { KEY_LEFT_BRACE, '{', "{", nullptr },
  { KEY_RIGHT_BRACE, '}', "}", nullptr },
  { KEY_BACKSLASH, '\\', "\\", nullptr },
  { KEY_NON_US_NUM, 0, "num", nullptr },
  { KEY_SEMICOLON, ';', ";", nullptr },
  { KEY_QUOTE, '"', "\"", nullptr },
  { KEY_TILDE, '~', "~", nullptr },
  { KEY_COMMA, ',', ",", nullptr },
  { KEY_PERIOD, '.', ".", nullptr },
  { KEY_SLASH, '/', "/", nullptr },
  { KEY_CAPS_LOCK, 0, "capslock", nullptr },
  { KEY_F1, 0, "f1", nullptr },
  { KEY_F2, 0, "f2", nullptr },
  { KEY_F3, 0, "f3", nullptr },
  { KEY_F4, 0, "f4", nullptr },
  { KEY_F5, 0, "f5", nullptr },
  { KEY_F6, 0, "f6", nullptr },
  { KEY_F7, 0, "f7", nullptr },
  { KEY_F8, 0, "f8", nullptr },
  { KEY_F9, 0, "f9", nullptr },
  { KEY_F10, 0, "f10", nullptr },
  { KEY_F11, 0, "f11", nullptr },
  { KEY_F12, 0, "f12", nullptr },
  { KEY_PRINTSCREEN, 0, "prtsc", nullptr },
  { KEY_SCROLL_LOCK, 0, "scrlk", nullptr },
  { KEY_PAUSE, 0, "pause", nullptr },
  { KEY_INSERT, 0, "insert", nullptr },
  { KEY_HOME, 0, "home", homeIcon },
  { KEY_PAGE_UP, 0, "pgup", pageUpIcon },
  { KEY_DELETE, 0, "del", deleteIcon },
  { KEY_END, 0, "end", endIcon },
  { KEY_PAGE_DOWN, 0, "pgdn", pageDownIcon },
  { KEY_RIGHT, 0, "right", rightArrowIcon },
  { KEY_LEFT, 0, "left", leftArrowIcon },
  { KEY_DOWN, 0, "down", downArrowIcon },
  { KEY_UP, 0, "up", upArrowIcon },
  { KEY_NUM_LOCK, 0, "numlk", nullptr },
  { KEYPAD_SLASH, '/', "/", nullptr },
  { KEYPAD_ASTERIX, '*', "*", nullptr },
  { KEYPAD_MINUS, '-', "-", nullptr },
  { KEYPAD_PLUS, '+', "+", nullptr },
  { KEYPAD_ENTER, '\r', "enter", nullptr },
  { KEYPAD_1, '1', "1", nullptr },
  { KEYPAD_2, '2', "2", nullptr },
  { KEYPAD_3, '3', "3", nullptr },
  { KEYPAD_4, '4', "4", nullptr },
  { KEYPAD_5, '5', "5", nullptr },
  { KEYPAD_6, '6', "6", nullptr },
  { KEYPAD_7, '7', "7", nullptr },
  { KEYPAD_8, '8', "8", nullptr },
  { KEYPAD_9, '9', "9", nullptr },
  { KEYPAD_0, '0', "0", nullptr },
  { KEYPAD_PERIOD, '.', ".", nullptr },
  { KEY_NON_US_BS, 0x08, "backspace", backspaceIcon },
  { KEY_MENU, 0, "menu", nullptr },
  { KEY_F13, 0, "F13", nullptr },
  { KEY_F14, 0, "F14", nullptr },
  { KEY_F15, 0, "F15", nullptr },
  { KEY_F16, 0, "F16", nullptr },
  { KEY_F17, 0, "F17", nullptr },
  { KEY_F18, 0, "F18", nullptr },
  { KEY_F19, 0, "F19", nullptr },
  { KEY_F20, 0, "F20", nullptr },
  { KEY_F21, 0, "F21", nullptr },
  { KEY_F22, 0, "F22", nullptr },
  { KEY_F23, 0, "F23", nullptr },
  { KEY_F24, 0, "F24", nullptr },
  // end of table marker
  { NO_CODE, 0, nullptr, nullptr }
};

#endif // _KeyLayout_