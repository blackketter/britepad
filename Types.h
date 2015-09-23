// Useful but not platform specific types

#ifndef _Types_
#define _Types_

#ifndef nil
#define nil (0)
#endif

enum Direction {
  DIRECTION_UP, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT
};

typedef long long millis_t;
typedef const char* appid_t;

#endif
