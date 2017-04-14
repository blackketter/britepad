#ifndef _ErgodoxLayout_
#define _ErgodoxLayout_
#include "KeyboardMatrix.h"

#define KEY_MISSING (0)

const keymap_t ergodoxLayout[] = {
// row 0
    { 0, KEY_EQUAL, 0, 0 },
    { 6, KEY_1, 1, 0 },
    { 12, KEY_2, 2, 0 },
    { 18, KEY_3, 3, 0 },
    { 24, KEY_4, 4, 0 },
    { 30, KEY_5, 5, 0 },

    { 72, KEY_6, 6+6, 0 },
    { 66, KEY_7, 7+6, 0 },
    { 60, KEY_8, 8+6, 0 },
    { 54, KEY_9, 9+6, 0 },
    { 48, KEY_0, 10+6, 0 },
    { 42, KEY_MINUS, 11+6, 0 },

// row 1
    { 0+1, KEY_TAB, 0, 1 },
    { 6+1, KEY_Q, 1, 1 },
    { 12+1, KEY_W, 2, 1 },
    { 18+1, KEY_E, 3, 1 },
    { 24+1, KEY_R, 4, 1 },
    { 30+1, KEY_T, 5, 1 },

    { 72+1, KEY_Y, 6+6, 1 },
    { 66+1, KEY_U, 7+6, 1 },
    { 60+1, KEY_I, 8+6, 1 },
    { 54+1, KEY_O, 9+6, 1 },
    { 48+1, KEY_P, 10+6, 1 },
    { 42+1, KEY_BACKSLASH, 11+6, 1 },

// row 2
    { 0+2, MODIFIERKEY_LEFT_CTRL, 0, 2 },
    { 6+2, KEY_A, 1, 2 },
    { 12+2, KEY_S, 2, 2 },
    { 18+2, KEY_D, 3, 2 },
    { 24+2, KEY_F, 4, 2 },
    { 30+2, KEY_G, 5, 2 },

    { 72+2, KEY_H, 6+6, 2 },
    { 66+2, KEY_J, 7+6, 2 },
    { 60+2, KEY_K, 8+6, 2 },
    { 54+2, KEY_L, 9+6, 2 },
    { 48+2, KEY_SEMICOLON, 10+6, 2 },
    { 42+2, KEY_QUOTE, 11+6, 2 },

// row 3
    { 0+3, MODIFIERKEY_LEFT_SHIFT, 0, 3 },
    { 6+3, KEY_Z, 1, 3 },
    { 12+3, KEY_X, 2, 3 },
    { 18+3, KEY_C, 3, 3 },
    { 24+3, KEY_V, 4, 3 },
    { 30+3, KEY_B, 5, 3 },

    { 72+3, KEY_N, 6+6, 3 },
    { 66+3, KEY_M, 7+6, 3 },
    { 60+3, KEY_COMMA, 8+6, 3 },
    { 54+3, KEY_PERIOD, 9+6, 3 },
    { 48+3, KEY_SLASH, 10+6, 3 },
    { 42+3, MODIFIERKEY_RIGHT_SHIFT, 11+6, 3 },

// row 4
    { 0+4, KEY_F1, 0, 4 },
    { 6+4, KEY_TILDE, 1, 4 },
    { 12+4, KEY_ESC, 2, 4 },
    { 18+4, KEY_LEFT, 3, 4 },
    { 24+4, KEY_RIGHT, 4, 4 },

    { 66+4, KEY_UP, 7+6, 4 },
    { 60+4, KEY_DOWN, 8+6, 4 },
    { 54+4, KEY_LEFT_BRACE, 9+6, 4 },
    { 48+4, KEY_RIGHT_BRACE, 10+6, 4 },
    { 42+4, KEY_F2, 11+6, 4 },

// thumb clusters

    { 35, MODIFIERKEY_LEFT_GUI, 7, 2 },
    { 41, MODIFIERKEY_LEFT_ALT, 8, 2 },

    { 83, MODIFIERKEY_RIGHT_CTRL, 10, 2 },
    { 77, MODIFIERKEY_RIGHT_GUI, 11, 2 },

    { 29, KEY_HOME, 8, 3 },

    { 71, KEY_PAGE_UP, 10, 3 },

    { 23, KEY_BACKSPACE, 6, 4 },
    { 17, KEY_DELETE, 7, 4 },
    { 11, KEY_END, 8, 4 },

    { 53, KEY_PAGE_DOWN, 10, 4 },
    { 59, KEY_ENTER, 11, 4 },
    { 65, KEY_SPACE, 12, 4 },
    { NO_KEY, 0, 0, 0 }  // end of table marker
};

#endif
