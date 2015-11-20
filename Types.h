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

typedef long long millis_t;
typedef signed long stime_t; // signed time for relative time, deltas, adjustments, etc.
typedef const char* appid_t;

#endif
