// Useful but not platform specific types

#ifndef _Types_
#define _Types_

#include "stdint.h"

enum direction_t {
  DIRECTION_UP,
  DIRECTION_RIGHT,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_N = DIRECTION_UP,
  DIRECTION_E = DIRECTION_RIGHT,
  DIRECTION_S = DIRECTION_DOWN,
  DIRECTION_W = DIRECTION_LEFT,
  DIRECTION_NE,
  DIRECTION_SE,
  DIRECTION_SW,
  DIRECTION_NW
};

enum AppMode {
  MOUSE_MODE = 1,
  INTERACTIVE_MODE = 2,
  SCREENSAVER_MODE = 4,
  INACTIVE_MODE = 8,
  ANY_MODE = 255
};

enum alignment_t {
  ALIGN_LEFT = 0,
  ALIGN_HCENTER = 1,
  ALIGN_CENTER = 1,
  ALIGN_RIGHT = 2,

  ALIGN_TOP = 0,
  ALIGN_VCENTER = 4,
  ALIGN_BOTTOM = 8
};

typedef const char* appid_t;
typedef const uint8_t* tune_t;
#endif
