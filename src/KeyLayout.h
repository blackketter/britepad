#ifndef _KeyLayout_
#define _KeyLayout_
#include "KeyInfo.h"

typedef uint8_t keyswitch_t;
const keyswitch_t NO_KEY = 0xff;
const keyswitch_t TOP_PAD_SWITCH = 0xfe;
const keyswitch_t BOTTOM_PAD_SWITCH = 0xfd;
const keyswitch_t LEFT_PAD_SWITCH = 0xfc;
const keyswitch_t RIGHT_PAD_SWITCH = 0xfb;

typedef struct keylayout_t {
  keyswitch_t key;
  uint8_t x;  // relative x position in the matrix
  uint8_t y;  // relative position in the matrix
  uint8_t w;  // relative key height (e.g. single width could be 4, double 8, 1.5 width would then be 5)
  uint8_t h;  // relative key width
} keylayout_t;

typedef struct keymap_t {
  keyswitch_t key;
  keycode_t code;
} keymap_t;

typedef struct keyoverlay_t {
  keycode_t from;  // in an overlay array, make the last item NO_CODE to indicate the end
  keycode_t to;    //   the final .to value should be NO_CODE if the overlay is opaque, ANY_CODE if it is transparent
} keyoverlay_t;

#endif  // _KeyLayout_