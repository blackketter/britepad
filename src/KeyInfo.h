#ifndef _KeyInfo_
#define _KeyInfo_
#include "keylayouts.h"
#include "widgets/Icons.h"

typedef uint16_t keycode_t;

// some new key codes
const keycode_t NO_CODE = 0;
const keycode_t ANY_CODE = 1;
const keycode_t KEY_LEFT_FN = 200;
const keycode_t KEY_RIGHT_FN = 201;

const keycode_t KEY_TOP_PAD = 202;
const keycode_t KEY_BOTTOM_PAD = 203;
const keycode_t KEY_LEFT_PAD = 204;
const keycode_t KEY_RIGHT_PAD = 205;

const keycode_t KEY_EXIT = 206;
const keycode_t KEY_LAUNCHBAR = 207;

const keycode_t MIN_MOUSE_KEY             = 208;
const keycode_t KEY_MOUSE_MOVE_UP         = 208;
const keycode_t KEY_MOUSE_MOVE_DOWN       = 209;
const keycode_t KEY_MOUSE_MOVE_LEFT       = 210;
const keycode_t KEY_MOUSE_MOVE_RIGHT      = 211;
const keycode_t KEY_MOUSE_BUTTON_LEFT     = 212;
const keycode_t KEY_MOUSE_BUTTON_MIDDLE   = 213;
const keycode_t KEY_MOUSE_BUTTON_RIGHT    = 214;
const keycode_t KEY_MOUSE_BUTTON_BACK     = 215;
const keycode_t KEY_MOUSE_BUTTON_FORWARD  = 216;
const keycode_t KEY_MOUSE_SCROLL_UP       = 217;
const keycode_t KEY_MOUSE_SCROLL_DOWN     = 218;
const keycode_t KEY_MOUSE_SCROLL_LEFT     = 219;
const keycode_t KEY_MOUSE_SCROLL_RIGHT    = 220;
const keycode_t MAX_MOUSE_KEY             = 220;

const keycode_t MODIFIERKEY_MOUSE         = 221;
const keycode_t MODIFIERKEY_MOUSE_SCROLL  = 222;

const keycode_t MAX_SOFT_KEY = 255;

typedef uint8_t modifierkey_t;

enum modifierKeys {
  MODIFIER_NONE = 0,
  MODIFIER_SHIFT = 1,
  MODIFIER_GUI = 2,
  MODIFIER_ALT = 4,
  MODIFIER_CTRL = 8,
  MODIFIER_FN = 16,
  MODIFIER_MOUSE = 32,
  MODIFIER_MOUSE_SCROLL = 64
};

typedef struct modifiedkey_t {
  keycode_t code;
  char c;
} modifiedkey_t;

const modifiedkey_t shiftedKeys[] = {
  { KEY_A, 'A' },
  { KEY_B, 'B' },
  { KEY_C, 'C' },
  { KEY_D, 'D' },
  { KEY_E, 'E' },
  { KEY_F, 'F' },
  { KEY_G, 'G' },
  { KEY_H, 'H' },
  { KEY_I, 'I' },
  { KEY_J, 'J' },
  { KEY_K, 'K' },
  { KEY_L, 'L' },
  { KEY_M, 'M' },
  { KEY_N, 'N' },
  { KEY_O, 'O' },
  { KEY_P, 'P' },
  { KEY_Q, 'Q' },
  { KEY_R, 'R' },
  { KEY_S, 'S' },
  { KEY_T, 'T' },
  { KEY_U, 'U' },
  { KEY_V, 'V' },
  { KEY_W, 'W' },
  { KEY_X, 'X' },
  { KEY_Y, 'Y' },
  { KEY_Z, 'Z' },
  { KEY_1, '!' },
  { KEY_2, '@' },
  { KEY_3, '#' },
  { KEY_4, '$' },
  { KEY_5, '%' },
  { KEY_6, '^' },
  { KEY_7, '&' },
  { KEY_8, '*' },
  { KEY_9, '(' },
  { KEY_0, ')' },
  { KEY_MINUS, '_' },
  { KEY_EQUAL, '+' },
  { KEY_LEFT_BRACE, '{' },
  { KEY_RIGHT_BRACE, '}' },
  { KEY_BACKSLASH, '|' },
  { KEY_SEMICOLON, ':' },
  { KEY_QUOTE, '\'' },
  { KEY_TILDE, '~' }, // tilde key is actually a ` in EN_US
  { KEY_COMMA, '<' },
  { KEY_PERIOD, '>' },
  { KEY_SLASH, '?' },
  { NO_CODE, 0 },
};

typedef struct keyinfo_t {
  keycode_t code;
  char c;
  const char* label;
  const icon_t icon;
  modifierkey_t modifier;
} keyinfo_t;

const keyinfo_t keyInfo[] = {
// nonstandard keys
  { KEY_LEFT_FN, 0, "fn", nullptr, MODIFIER_FN },
  { KEY_RIGHT_FN, 0, "fn", nullptr, MODIFIER_FN },

  { KEY_MOUSE_MOVE_UP, 0, "mouse up", mouseUpIcon, false },
  { KEY_MOUSE_MOVE_DOWN, 0, "mouse down", mouseDownIcon, false },
  { KEY_MOUSE_MOVE_LEFT, 0, "mouse left", mouseLeftIcon, false },
  { KEY_MOUSE_MOVE_RIGHT, 0, "mouse right", mouseRightIcon, false },
  { KEY_MOUSE_BUTTON_LEFT, 0, "mouse button left", mouseButtonLeftIcon, false },
  { KEY_MOUSE_BUTTON_MIDDLE, 0, "mouse button middle", mouseButtonMiddleIcon, false },
  { KEY_MOUSE_BUTTON_RIGHT, 0, "mouse button right", mouseButtonRightIcon, false },
  { KEY_MOUSE_BUTTON_BACK, 0, "mouse button back", mouseButtonBackIcon, false },
  { KEY_MOUSE_BUTTON_FORWARD, 0, "mouse button fwd", mouseButtonFwdIcon, false },
  { KEY_MOUSE_SCROLL_UP, 0, "mouse scroll up", mouseScrollUpIcon, false },
  { KEY_MOUSE_SCROLL_DOWN, 0, "mouse scroll down", mouseScrollDownIcon, false },
  { KEY_MOUSE_SCROLL_LEFT, 0, "mouse scroll left", mouseScrollLeftIcon, false },
  { KEY_MOUSE_SCROLL_RIGHT, 0, "mouse scroll right", mouseScrollRightIcon, false },
  { MODIFIERKEY_MOUSE, 0, "mouse", mouseIcon, MODIFIER_MOUSE },
  { MODIFIERKEY_MOUSE_SCROLL, 0, "mouse scroll", mouseScrollIcon, MODIFIER_MOUSE_SCROLL },

// standard keys
  { MODIFIERKEY_LEFT_CTRL, 0, "ctrl", controlIcon, MODIFIER_CTRL },
  { MODIFIERKEY_LEFT_SHIFT, 0, "shift", shiftIcon, MODIFIER_SHIFT },
  { MODIFIERKEY_LEFT_ALT, 0, "opt", optionIcon, MODIFIER_ALT },
  { MODIFIERKEY_LEFT_GUI, 0, "cmd", commandIcon, MODIFIER_GUI },
  { MODIFIERKEY_RIGHT_CTRL, 0, "ctrl", controlIcon, MODIFIER_CTRL },
  { MODIFIERKEY_RIGHT_SHIFT, 0, "shift", shiftIcon, MODIFIER_SHIFT },
  { MODIFIERKEY_RIGHT_ALT, 0, "opt", optionIcon, MODIFIER_ALT },
  { MODIFIERKEY_RIGHT_GUI, 0, "cmd", commandIcon, MODIFIER_GUI },

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

  { KEY_A, 'a', "a", nullptr, false },
  { KEY_B, 'b', "b", nullptr, false },
  { KEY_C, 'c', "c", nullptr, false },
  { KEY_D, 'd', "d", nullptr, false },
  { KEY_E, 'e', "e", nullptr, false },
  { KEY_F, 'f', "f", nullptr, false },
  { KEY_G, 'g', "g", nullptr, false },
  { KEY_H, 'h', "h", nullptr, false },
  { KEY_I, 'i', "i", nullptr, false },
  { KEY_J, 'j', "j", nullptr, false },
  { KEY_K, 'k', "k", nullptr, false },
  { KEY_L, 'l', "l", nullptr, false },
  { KEY_M, 'm', "m", nullptr, false },
  { KEY_N, 'n', "n", nullptr, false },
  { KEY_O, 'o', "o", nullptr, false },
  { KEY_P, 'p', "p", nullptr, false },
  { KEY_Q, 'q', "q", nullptr, false },
  { KEY_R, 'r', "r", nullptr, false },
  { KEY_S, 's', "s", nullptr, false },
  { KEY_T, 't', "t", nullptr, false },
  { KEY_U, 'u', "u", nullptr, false },
  { KEY_V, 'v', "v", nullptr, false },
  { KEY_W, 'w', "w", nullptr, false },
  { KEY_X, 'x', "x", nullptr, false },
  { KEY_Y, 'y', "y", nullptr, false },
  { KEY_Z, 'z', "z", nullptr, false },
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
  { KEY_ENTER, '\r', "return", returnIcon, false },
  { KEY_ESC, 0x1b, "esc", escIcon, false },
  { KEY_BACKSPACE, 0x08, "bs", backspaceIcon, false },
  { KEY_TAB, '\t', "tab", tabIcon, false },
  { KEY_SPACE, ' ', " ", spaceIcon, false },
  { KEY_MINUS, '-', "-", nullptr, false },
  { KEY_EQUAL, '=', "=", nullptr, false },
  { KEY_LEFT_BRACE, '[', "[", nullptr, false },
  { KEY_RIGHT_BRACE, ']', "]", nullptr, false },
  { KEY_BACKSLASH, '\\', "\\", nullptr, false },
  { KEY_NON_US_NUM, 0, "num", nullptr, false },
  { KEY_SEMICOLON, ';', ";", nullptr, false },
  { KEY_QUOTE, '"', "\"", nullptr, false },
  { KEY_TILDE, '`', "'", nullptr, false },  // tilde key is actually ` in EN_US
  { KEY_COMMA, ',', ",", nullptr, false },
  { KEY_PERIOD, '.', ".", nullptr, false },
  { KEY_SLASH, '/', "/", nullptr, false },
  { KEY_CAPS_LOCK, 0, "caps", capsLockIcon, false },
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
  { KEY_PRINTSCREEN, 0, "pts", nullptr, false },
  { KEY_SCROLL_LOCK, 0, "scl", nullptr, false },
  { KEY_PAUSE, 0, "p/b", nullptr, false },
  { KEY_INSERT, 0, "ins", nullptr, false },
  { KEY_HOME, 0, "home", homeIcon, false },
  { KEY_PAGE_UP, 0, "pgup", pageUpIcon, false },
  { KEY_DELETE, 0, "del", deleteIcon, false },
  { KEY_END, 0, "end", endIcon, false },
  { KEY_PAGE_DOWN, 0, "pgdn", pageDownIcon, false },
  { KEY_RIGHT, 0, "right", rightArrowIcon, false },
  { KEY_LEFT, 0, "left", leftArrowIcon, false },
  { KEY_DOWN, 0, "down", downArrowIcon, false },
  { KEY_UP, 0, "up", upArrowIcon, false },
  { KEY_NUM_LOCK, 0, "numlk", numLockIcon, false },
  { KEYPAD_SLASH, '/', "/", nullptr, false },
  { KEYPAD_ASTERIX, '*', "*", nullptr, false },
  { KEYPAD_MINUS, '-', "-", nullptr, false },
  { KEYPAD_PLUS, '+', "+", nullptr, false },
  { KEYPAD_ENTER, '\n', "enter", returnIcon, false },
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
  { KEY_F13, 0, "f13", nullptr, false },
  { KEY_F14, 0, "f14", nullptr, false },
  { KEY_F15, 0, "f15", nullptr, false },
  { KEY_F16, 0, "f16", nullptr, false },
  { KEY_F17, 0, "f17", nullptr, false },
  { KEY_F18, 0, "f18", nullptr, false },
  { KEY_F19, 0, "f19", nullptr, false },
  { KEY_F20, 0, "f20", nullptr, false },
  { KEY_F21, 0, "f21", nullptr, false },
  { KEY_F22, 0, "f22", nullptr, false },
  { KEY_F23, 0, "f23", nullptr, false },
  { KEY_F24, 0, "f24", nullptr, false },
  // end of table marker
  { NO_CODE, 0, nullptr, nullptr }
};

const keycode_t TOTALCODES = sizeof(keyInfo)/sizeof(keyInfo[0]) - 1;  // how many unique codes (not including the end of table marker

#endif // _KeyInfo_