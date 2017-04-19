#ifndef _KeyboardLayout_
#define _KeyboardLayout_
#include "keylayouts.h"

typedef uint8_t keyswitch_t;
const keyswitch_t NO_KEY = 0xff;

typedef uint16_t keycode_t;
const keycode_t NO_CODE = 0;

// some new keys
#define KEY_LEFT_FN (200)
#define KEY_RIGHT_FN (201)

typedef struct keylayout_t {
  keyswitch_t key;
  keycode_t code;
  uint8_t x;
  uint8_t y;
} keylayout_t;


#endif // _KeyboardLayout_