#ifndef _ErgodoxLayout_
#define _ErgodoxLayout_
#include "KeyLayout.h"

const keylayout_t ergodoxLayout[] = {
// row 0
    { 0, KEY_EQUAL, 0, 0, 1, 1 },
    { 6, KEY_1, 1, 0, 1, 1 },
    { 12, KEY_2, 2, 0, 1, 1 },
    { 18, KEY_3, 3, 0, 1, 1 },
    { 24, KEY_4, 4, 0, 1, 1 },
    { 30, KEY_5, 5, 0, 1, 1 },

    { 72, KEY_6, 7, 0, 1, 1 },
    { 66, KEY_7, 8, 0, 1, 1 },
    { 60, KEY_8, 9, 0, 1, 1 },
    { 54, KEY_9, 10, 0, 1, 1 },
    { 48, KEY_0, 11, 0, 1, 1 },
    { 42, KEY_MINUS, 12, 0, 1, 1 },

// row 1
    { 0+1, KEY_TAB, 0, 1, 1, 1 },
    { 6+1, KEY_Q, 1, 1, 1, 1 },
    { 12+1, KEY_W, 2, 1, 1, 1 },
    { 18+1, KEY_E, 3, 1, 1, 1 },
    { 24+1, KEY_R, 4, 1, 1, 1 },
    { 30+1, KEY_T, 5, 1, 1, 1 },

    { 72+1, KEY_Y, 7, 1, 1, 1 },
    { 66+1, KEY_U, 8, 1, 1, 1 },
    { 60+1, KEY_I, 9, 1, 1, 1 },
    { 54+1, KEY_O, 10, 1, 1, 1 },
    { 48+1, KEY_P, 11, 1, 1, 1 },
    { 42+1, KEY_BACKSLASH, 12, 1, 1, 1 },

// row 2
    { 0+2, MODIFIERKEY_LEFT_CTRL, 0, 2, 1, 1 },
    { 6+2, KEY_A, 1, 2, 1, 1 },
    { 12+2, KEY_S, 2, 2, 1, 1 },
    { 18+2, KEY_D, 3, 2, 1, 1 },
    { 24+2, KEY_F, 4, 2, 1, 1 },
    { 30+2, KEY_G, 5, 2, 1, 1 },

    { 72+2, KEY_H, 7, 2, 1, 1 },
    { 66+2, KEY_J, 8, 2, 1, 1 },
    { 60+2, KEY_K, 9, 2, 1, 1 },
    { 54+2, KEY_L, 10, 2, 1, 1 },
    { 48+2, KEY_SEMICOLON, 11, 2, 1, 1 },
    { 42+2, KEY_QUOTE, 12, 2, 1, 1 },

// row 3
    { 0+3, MODIFIERKEY_LEFT_SHIFT, 0, 3, 1, 1 },
    { 6+3, KEY_Z, 1, 3, 1, 1 },
    { 12+3, KEY_X, 2, 3, 1, 1 },
    { 18+3, KEY_C, 3, 3, 1, 1 },
    { 24+3, KEY_V, 4, 3, 1, 1 },
    { 30+3, KEY_B, 5, 3, 1, 1 },

    { 72+3, KEY_N, 7, 3, 1, 1 },
    { 66+3, KEY_M, 8, 3, 1, 1 },
    { 60+3, KEY_COMMA, 9, 3, 1, 1 },
    { 54+3, KEY_PERIOD, 10, 3, 1, 1 },
    { 48+3, KEY_SLASH, 11, 3, 1, 1 },
    { 42+3, MODIFIERKEY_RIGHT_SHIFT, 12, 3, 1, 1 },

// row 4
    { 0+4, KEY_LEFT_FN, 0, 4, 1, 1 },
    { 6+4, KEY_TILDE, 1, 4, 1, 1 },
    { 12+4, KEY_ESC, 2, 4, 1, 1 },
    { 18+4, KEY_LEFT, 3, 4, 1, 1 },
    { 24+4, KEY_RIGHT, 4, 4, 1, 1 },

    { 66+4, KEY_UP, 8, 4, 1, 1 },
    { 60+4, KEY_DOWN, 9, 4, 1, 1 },
    { 54+4, KEY_LEFT_BRACE, 10, 4, 1, 1 },
    { 48+4, KEY_RIGHT_BRACE, 11, 4, 1, 1 },
    { 42+4, KEY_RIGHT_FN, 12, 4, 1, 1 },

// thumb clusters

    { 35, MODIFIERKEY_LEFT_GUI, 4, 6, 1, 1 },
    { 41, MODIFIERKEY_LEFT_ALT, 5, 6, 1, 1 },

    { 83, MODIFIERKEY_RIGHT_CTRL, 7, 6, 1, 1 },
    { 77, MODIFIERKEY_RIGHT_GUI, 8, 6, 1, 1 },

    { 29, KEY_HOME, 5, 7, 1, 1 },

    { 71, KEY_PAGE_UP, 7, 7, 1, 1 },

    { 23, KEY_BACKSPACE, 3, 7, 1, 2 },
    { 17, KEY_DELETE, 4, 7, 1, 2 },
    { 11, KEY_END, 5, 8, 1, 1 },

    { 53, KEY_PAGE_DOWN, 7, 8, 1, 1 },
    { 59, KEY_ENTER, 8, 7, 1, 2 },
    { 65, KEY_SPACE, 9, 7, 1, 2 },

  // end of table marker
    { NO_KEY, 0, 0, 0, 0, 0 }
};

#endif
