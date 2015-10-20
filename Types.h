// Useful but not platform specific types

#ifndef _Types_
#define _Types_

#ifndef nil
#define nil (0)
#endif

enum Direction {
  DIRECTION_UP, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT
};

enum AppMode {
  MOUSE,
  INTERACTIVE,
  SCREENSAVER,
  INACTIVE,
};


typedef long long millis_t;
typedef unsigned long stime_t; // signed time for relative time, deltas, adjustments, etc.
typedef const char* appid_t;

#endif
