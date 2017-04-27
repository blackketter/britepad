#ifndef _ErgodoxLayout_
#define _ErgodoxLayout_
#include "KeyLayout.h"

const keylayout_t ergodoxLayout[] = {
// code, x, y, w, h
// row 0
    { 0, 0, 0, 1, 1 },
    { 6, 1, 0, 1, 1 },
    { 12, 2, 0, 1, 1 },
    { 18, 3, 0, 1, 1 },
    { 24, 4, 0, 1, 1 },
    { 30, 5, 0, 1, 1 },

    { 72, 7, 0, 1, 1 },
    { 66, 8, 0, 1, 1 },
    { 60, 9, 0, 1, 1 },
    { 54, 10, 0, 1, 1 },
    { 48, 11, 0, 1, 1 },
    { 42, 12, 0, 1, 1 },

// row 1
    { 0+1, 0, 1, 1, 1 },
    { 6+1, 1, 1, 1, 1 },
    { 12+1, 2, 1, 1, 1 },
    { 18+1, 3, 1, 1, 1 },
    { 24+1, 4, 1, 1, 1 },
    { 30+1, 5, 1, 1, 1 },

    { 72+1, 7, 1, 1, 1 },
    { 66+1, 8, 1, 1, 1 },
    { 60+1, 9, 1, 1, 1 },
    { 54+1, 10, 1, 1, 1 },
    { 48+1, 11, 1, 1, 1 },
    { 42+1, 12, 1, 1, 1 },

// row 2
    { 0+2, 0, 2, 1, 1 },
    { 6+2, 1, 2, 1, 1 },
    { 12+2, 2, 2, 1, 1 },
    { 18+2, 3, 2, 1, 1 },
    { 24+2, 4, 2, 1, 1 },
    { 30+2, 5, 2, 1, 1 },

    { 72+2, 7, 2, 1, 1 },
    { 66+2, 8, 2, 1, 1 },
    { 60+2, 9, 2, 1, 1 },
    { 54+2, 10, 2, 1, 1 },
    { 48+2, 11, 2, 1, 1 },
    { 42+2, 12, 2, 1, 1 },

// row 3
    { 0+3, 0, 3, 1, 1 },
    { 6+3, 1, 3, 1, 1 },
    { 12+3, 2, 3, 1, 1 },
    { 18+3, 3, 3, 1, 1 },
    { 24+3, 4, 3, 1, 1 },
    { 30+3, 5, 3, 1, 1 },

    { 72+3, 7, 3, 1, 1 },
    { 66+3, 8, 3, 1, 1 },
    { 60+3, 9, 3, 1, 1 },
    { 54+3, 10, 3, 1, 1 },
    { 48+3, 11, 3, 1, 1 },
    { 42+3, 12, 3, 1, 1 },

// row 4
    { 0+4, 0, 4, 1, 1 },
    { 6+4, 1, 4, 1, 1 },
    { 12+4, 2, 4, 1, 1 },
    { 18+4, 3, 4, 1, 1 },
    { 24+4, 4, 4, 1, 1 },

    { 66+4, 8, 4, 1, 1 },
    { 60+4, 9, 4, 1, 1 },
    { 54+4, 10, 4, 1, 1 },
    { 48+4, 11, 4, 1, 1 },
    { 42+4, 12, 4, 1, 1 },

// thumb clusters

    { 35, 4, 6, 1, 1 },
    { 41, 5, 6, 1, 1 },

    { 83, 7, 6, 1, 1 },
    { 77, 8, 6, 1, 1 },

    { 29, 5, 7, 1, 1 },

    { 71, 7, 7, 1, 1 },

    { 23, 3, 7, 1, 2 },
    { 17, 4, 7, 1, 2 },
    { 11, 5, 8, 1, 1 },

    { 53, 7, 8, 1, 1 },
    { 59, 8, 7, 1, 2 },
    { 65, 9, 7, 1, 2 },

  // end of table marker
    { NO_KEY, 0, 0, 0, 0 }
};

const keymap_t ergodoxMap[] = {
// row 0
    { 0, KEY_EQUAL },
    { 6, KEY_1 },
    { 12, KEY_2 },
    { 18, KEY_3 },
    { 24, KEY_4 },
    { 30, KEY_5 },

    { 72, KEY_6 },
    { 66, KEY_7 },
    { 60, KEY_8 },
    { 54, KEY_9 },
    { 48, KEY_0 },
    { 42, KEY_MINUS },

// row 1
    { 0+1, KEY_TAB },
    { 6+1, KEY_Q },
    { 12+1, KEY_W },
    { 18+1, KEY_E },
    { 24+1, KEY_R },
    { 30+1, KEY_T },

    { 72+1, KEY_Y },
    { 66+1, KEY_U },
    { 60+1, KEY_I },
    { 54+1, KEY_O },
    { 48+1, KEY_P },
    { 42+1, KEY_BACKSLASH },

// row 2
    { 0+2, MODIFIERKEY_LEFT_CTRL },
    { 6+2, KEY_A },
    { 12+2, KEY_S },
    { 18+2, KEY_D },
    { 24+2, KEY_F },
    { 30+2, KEY_G },

    { 72+2, KEY_H },
    { 66+2, KEY_J },
    { 60+2, KEY_K },
    { 54+2, KEY_L },
    { 48+2, KEY_SEMICOLON },
    { 42+2, KEY_QUOTE },

// row 3
    { 0+3, MODIFIERKEY_LEFT_SHIFT },
    { 6+3, KEY_Z },
    { 12+3, KEY_X },
    { 18+3, KEY_C },
    { 24+3, KEY_V },
    { 30+3, KEY_B },

    { 72+3, KEY_N },
    { 66+3, KEY_M },
    { 60+3, KEY_COMMA },
    { 54+3, KEY_PERIOD },
    { 48+3, KEY_SLASH },
    { 42+3, MODIFIERKEY_RIGHT_SHIFT },

// row 4
    { 0+4, KEY_LEFT_FN },
    { 6+4, KEY_TILDE },
    { 12+4, KEY_ESC },
    { 18+4, KEY_LEFT },
    { 24+4, KEY_RIGHT },

    { 66+4, KEY_UP },
    { 60+4, KEY_DOWN },
    { 54+4, KEY_LEFT_BRACE },
    { 48+4, KEY_RIGHT_BRACE },
    { 42+4, KEY_RIGHT_FN },

// thumb clusters

    { 35, MODIFIERKEY_LEFT_GUI },
    { 41, MODIFIERKEY_LEFT_ALT },

    { 83, MODIFIERKEY_RIGHT_CTRL },
    { 77, MODIFIERKEY_RIGHT_GUI },

    { 29, KEY_HOME },

    { 71, KEY_PAGE_UP },

    { 23, KEY_BACKSPACE },
    { 17, KEY_DELETE },
    { 11, KEY_END },

    { 53, KEY_PAGE_DOWN },
    { 59, KEY_ENTER },
    { 65, KEY_SPACE },

  // end of table marker
    { NO_KEY, NO_CODE }
};

#endif
