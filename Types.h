// Useful but not platform specific types

#ifndef _Types_
#define _Types_

#include "stdint.h"

enum direction_t {
  DIRECTION_UP, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT
};

enum AppMode {
  MOUSE,
  INTERACTIVE,
  SCREENSAVER,
  INACTIVE,
};

typedef long long millis_t;
typedef signed long stime_t; // signed time for relative time, deltas, adjustments, etc.
typedef const char* appid_t;
typedef const uint8_t* icon_t;

#endif
