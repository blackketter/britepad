#ifndef _USBKeyLayout_
#define _USBKeyLayout_
#include "KeyLayout.h"

const keylayout_t ANSI104KeyLayout[] = {
// code, x, y, w, h
// row 0
    { KEY_ESC, 0, 0, 4, 4 },

    { KEY_F1, 8, 0, 4, 4 },
    { KEY_F2, 12, 0, 4, 4 },
    { KEY_F3, 16, 0, 4, 4 },
    { KEY_F4, 20, 0, 4, 4 },

    { KEY_F5, 26, 0, 4, 4 },
    { KEY_F6, 30, 0, 4, 4 },
    { KEY_F7, 34, 0, 4, 4 },
    { KEY_F8, 38, 0, 4, 4 },

    { KEY_F9, 44, 0, 4, 4 },
    { KEY_F10, 48, 0, 4, 4 },
    { KEY_F11, 52, 0, 4, 4 },
    { KEY_F12, 56, 0, 4, 4 },

// row 1
    { KEY_TILDE, 0, 5, 4, 4 },
    { KEY_1, 4, 5, 4, 4 },
    { KEY_2, 8, 5, 4, 4 },
    { KEY_3, 12, 5, 4, 4 },
    { KEY_4, 16, 5, 4, 4 },
    { KEY_5, 20, 5, 4, 4 },
    { KEY_6, 24, 5, 4, 4 },
    { KEY_7, 28, 5, 4, 4 },
    { KEY_8, 32, 5, 4, 4 },
    { KEY_9, 36, 5, 4, 4 },
    { KEY_0, 40, 5, 4, 4 },
    { KEY_MINUS, 44, 5, 4, 4 },
    { KEY_EQUAL, 48, 5, 4, 4 },
    { KEY_BACKSPACE, 52, 5, 8, 4 },

// row 2
    { KEY_TAB, 0, 9, 6, 4 },
    { KEY_Q, 6, 9, 4, 4 },
    { KEY_W, 10, 9, 4, 4 },
    { KEY_E, 14, 9, 4, 4 },
    { KEY_R, 18, 9, 4, 4 },
    { KEY_T, 22, 9, 4, 4 },
    { KEY_Y, 26, 9, 4, 4 },
    { KEY_U, 30, 9, 4, 4 },
    { KEY_I, 34, 9, 4, 4 },
    { KEY_O, 38, 9, 4, 4 },
    { KEY_P, 42, 9, 4, 4 },
    { KEY_LEFT_BRACE, 46, 9, 4, 4 },
    { KEY_RIGHT_BRACE, 50, 9, 4, 4 },
    { KEY_BACKSLASH, 54, 9, 6, 4 },

// row 3
    { KEY_CAPS_LOCK, 0, 13, 7, 4 },
    { KEY_A, 7, 13, 4, 4 },
    { KEY_S, 11, 13, 4, 4 },
    { KEY_D, 15, 13, 4, 4 },
    { KEY_F, 19, 13, 4, 4 },
    { KEY_G, 23, 13, 4, 4 },
    { KEY_H, 27, 13, 4, 4 },
    { KEY_J, 31, 13, 4, 4 },
    { KEY_K, 35, 13, 4, 4 },
    { KEY_L, 39, 13, 4, 4 },
    { KEY_SEMICOLON, 43, 13, 4, 4 },
    { KEY_QUOTE, 47, 13, 4, 4 },
    { KEY_ENTER, 51, 13, 9, 4 },

// row 4
    { MODIFIERKEY_LEFT_SHIFT, 0, 17, 9, 4 },
    { KEY_Z, 9, 17, 4, 4 },
    { KEY_X, 13, 17, 4, 4 },
    { KEY_C, 17, 17, 4, 4 },
    { KEY_V, 21, 17, 4, 4 },
    { KEY_B, 25, 17, 4, 4 },
    { KEY_N, 29, 17, 4, 4 },
    { KEY_M, 33, 17, 4, 4 },
    { KEY_COMMA, 37, 17, 4, 4 },
    { KEY_PERIOD, 41, 17, 4, 4 },
    { KEY_SLASH, 45, 17, 4, 4 },
    { MODIFIERKEY_RIGHT_SHIFT, 49, 17, 11, 4 },

// row 5
    { MODIFIERKEY_LEFT_CTRL, 0, 21, 6, 4 },
    { MODIFIERKEY_LEFT_GUI, 6, 21, 6, 4 },
    { MODIFIERKEY_LEFT_ALT, 12, 21, 6, 4 },
    { KEY_SPACE, 18, 21, 26, 4 },
    { MODIFIERKEY_RIGHT_ALT, 44, 21, 6, 4 },
    { MODIFIERKEY_RIGHT_GUI, 50, 21, 6, 4 },
    { MODIFIERKEY_RIGHT_CTRL, 56, 21, 4, 4 },

// lower half
#define lowery (26)
// system clusters
    { KEY_PRINTSCREEN, 0, lowery, 4, 4 },
    { KEY_SCROLL_LOCK, 4, lowery, 4, 4 },
    { KEY_PAUSE, 8, lowery, 4, 4 },

    { KEY_INSERT, 0, lowery+6, 4, 4 },
    { KEY_HOME, 4, lowery+6, 4, 4 },
    { KEY_PAGE_UP, 8, lowery+6, 4, 4 },
    { KEY_DELETE, 0, lowery+10, 4, 4 },
    { KEY_END, 4, lowery+10, 4, 4 },
    { KEY_PAGE_DOWN, 8, lowery+10, 4, 4 },

// keypad
#define keypadx (16+4)
    { KEY_NUM_LOCK, keypadx, lowery, 4, 3 },
    { KEYPAD_SLASH, keypadx+4, lowery, 4, 3 },
    { KEYPAD_ASTERIX, keypadx+8, lowery, 4, 3 },
    { KEYPAD_MINUS, keypadx+12, lowery, 4, 3 },

    { KEYPAD_7, keypadx, lowery+3, 4, 3 },
    { KEYPAD_8, keypadx+4, lowery+3, 4, 3 },
    { KEYPAD_9, keypadx+8, lowery+3, 4, 3 },
    { KEYPAD_PLUS, keypadx+12, lowery+3, 4, 6 },

    { KEYPAD_4, keypadx, lowery+6, 4, 3 },
    { KEYPAD_5, keypadx+4, lowery+6, 4, 3 },
    { KEYPAD_6, keypadx+8, lowery+6, 4, 3 },

    { KEYPAD_1, keypadx, lowery+9, 4, 3 },
    { KEYPAD_2, keypadx+4, lowery+9, 4, 3 },
    { KEYPAD_3, keypadx+8, lowery+9, 4, 3 },
    { KEYPAD_ENTER, keypadx+12, lowery+9, 4, 6 },

    { KEYPAD_0, keypadx, lowery+12, 8, 3 },
    { KEYPAD_PERIOD, keypadx+8, lowery+12, 4, 3 },

// arrow cluster
#define arrowx (keypadx+16+8)
    { KEY_UP, arrowx+4, lowery, 4, 4 },
    { KEY_LEFT, arrowx, lowery+4, 4, 4 },
    { KEY_DOWN, arrowx+4, lowery+4, 4, 4 },
    { KEY_RIGHT, arrowx+8, lowery+4, 4, 4 },

  // end of table marker
    { NO_KEY, 0, 0, 0, 0 }
};

const keymap_t ANSI104KeyMap[] = {
// row 0
    { KEY_ESC, KEY_ESC },

    { KEY_F1, KEY_F1 },
    { KEY_F2, KEY_F2 },
    { KEY_F3, KEY_F3 },
    { KEY_F4, KEY_F4 },

    { KEY_F5, KEY_F5 },
    { KEY_F6, KEY_F6 },
    { KEY_F7, KEY_F7 },
    { KEY_F8, KEY_F8 },

    { KEY_F9, KEY_F9 },
    { KEY_F10, KEY_F10 },
    { KEY_F11, KEY_F11 },
    { KEY_F12, KEY_F12 },

    { KEY_PRINTSCREEN, KEY_F13 },
    { KEY_SCROLL_LOCK, KEY_F14 },
    { KEY_PAUSE, KEY_F15 },

// row 1
    { KEY_TILDE, KEY_TILDE },
    { KEY_1, KEY_1 },
    { KEY_2, KEY_2 },
    { KEY_3, KEY_3 },
    { KEY_4, KEY_4 },
    { KEY_5, KEY_5 },
    { KEY_6, KEY_6 },
    { KEY_7, KEY_7 },
    { KEY_8, KEY_8 },
    { KEY_9, KEY_9 },
    { KEY_0, KEY_0 },
    { KEY_MINUS, KEY_MINUS },
    { KEY_EQUAL, KEY_EQUAL },
    { KEY_BACKSPACE, KEY_BACKSPACE },

    { KEY_HOME, KEY_HOME },
    { KEY_END, KEY_END },

    { KEY_NUM_LOCK, KEY_NUM_LOCK },
    { KEYPAD_SLASH, KEYPAD_SLASH },
    { KEYPAD_ASTERIX, KEYPAD_ASTERIX },
    { KEYPAD_MINUS, KEYPAD_MINUS },

// row 2
    { KEY_TAB, KEY_TAB },
    { KEY_Q, KEY_Q },
    { KEY_W, KEY_W },
    { KEY_E, KEY_E },
    { KEY_R, KEY_R },
    { KEY_T, KEY_T },
    { KEY_Y, KEY_Y },
    { KEY_U, KEY_U },
    { KEY_I, KEY_I },
    { KEY_O, KEY_O },
    { KEY_P, KEY_P },
    { KEY_LEFT_BRACE, KEY_LEFT_BRACE },
    { KEY_RIGHT_BRACE, KEY_RIGHT_BRACE },
    { KEY_BACKSLASH, KEY_BACKSLASH },

    { KEY_INSERT, KEY_INSERT },
    { KEY_PAGE_UP, KEY_PAGE_UP },

    { KEYPAD_7, KEYPAD_7 },
    { KEYPAD_8, KEYPAD_8 },
    { KEYPAD_9, KEYPAD_9 },
    { KEYPAD_PLUS, KEYPAD_PLUS },

// row 3
    { KEY_CAPS_LOCK, KEY_CAPS_LOCK },
    { KEY_A, KEY_A },
    { KEY_S, KEY_S },
    { KEY_D, KEY_D },
    { KEY_F, KEY_F },
    { KEY_G, KEY_G },
    { KEY_H, KEY_H },
    { KEY_J, KEY_J },
    { KEY_K, KEY_K },
    { KEY_L, KEY_L },
    { KEY_SEMICOLON, KEY_SEMICOLON },
    { KEY_QUOTE, KEY_QUOTE },
    { KEY_ENTER, KEY_ENTER },

    { KEY_DELETE, KEY_DELETE },
    { KEY_PAGE_DOWN, KEY_PAGE_DOWN },

    { KEYPAD_4, KEYPAD_4 },
    { KEYPAD_5, KEYPAD_5 },
    { KEYPAD_6, KEYPAD_6 },

// row 4
    { MODIFIERKEY_LEFT_SHIFT, MODIFIERKEY_LEFT_SHIFT },
    { KEY_Z, KEY_Z },
    { KEY_X, KEY_X },
    { KEY_C, KEY_C },
    { KEY_V, KEY_V },
    { KEY_B, KEY_B },
    { KEY_N, KEY_N },
    { KEY_M, KEY_M },
    { KEY_COMMA, KEY_COMMA },
    { KEY_PERIOD, KEY_PERIOD },
    { KEY_BACKSLASH, KEY_BACKSLASH },
    { MODIFIERKEY_RIGHT_SHIFT, MODIFIERKEY_RIGHT_SHIFT },

    { KEYPAD_1, KEYPAD_1 },
    { KEYPAD_2, KEYPAD_2 },
    { KEYPAD_3, KEYPAD_3 },
    { KEYPAD_ENTER, KEYPAD_ENTER },

// row 5
    { MODIFIERKEY_LEFT_CTRL, MODIFIERKEY_LEFT_CTRL },
    { MODIFIERKEY_LEFT_GUI, MODIFIERKEY_LEFT_GUI },
    { MODIFIERKEY_LEFT_ALT, MODIFIERKEY_LEFT_ALT },
    { KEY_SPACE, KEY_SPACE },
    { MODIFIERKEY_RIGHT_ALT, MODIFIERKEY_RIGHT_ALT },
    { MODIFIERKEY_RIGHT_GUI, MODIFIERKEY_RIGHT_GUI },
    { MODIFIERKEY_RIGHT_CTRL, MODIFIERKEY_RIGHT_CTRL },

    { KEYPAD_0, KEYPAD_0 },
    { KEYPAD_PERIOD, KEYPAD_PERIOD },
    { KEYPAD_ENTER, KEYPAD_ENTER },

// arrow cluster
    { KEY_UP, KEY_UP },
    { KEY_LEFT, KEY_LEFT },
    { KEY_DOWN, KEY_DOWN },
    { KEY_RIGHT, KEY_RIGHT },

    { NO_KEY, NO_CODE }
};


#endif // _USBKeyLayout_