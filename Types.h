// Useful but not platform specific types

#ifndef _Types_
#define _Types_

#include "stdint.h"

enum direction_t {
  DIRECTION_UP, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT
};

enum AppMode {
  MOUSE_MODE = 1,
  INTERACTIVE_MODE = 2,
  SCREENSAVER_MODE = 4,
  INACTIVE_MODE = 8,
  ANY_MODE = 255
};

enum Alignment {
  ALIGN_LEFT = 0,
  ALIGN_HCENTER = 1,
  ALIGN_CENTER = 1,
  ALIGN_RIGHT = 2,

  ALIGN_TOP = 0,
  ALIGN_VCENTER = 4,
  ALIGN_BOTTOM = 8
};

typedef int64_t millis_t;
typedef int32_t stime_t; // signed time for relative time, deltas, adjustments, etc.
typedef const char* appid_t;

#endif
