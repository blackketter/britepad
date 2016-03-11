#ifndef _DefaultGestures_
#define _DefaultGestures_
#include "Gesture.h"

enum {
  NO_GESTURE = 0,

  GESTURE_SWIPE_RIGHT = 0x08, // right is the same gesture as backspace
  GESTURE_SWIPE_LEFT = 0x20, // left is the same gesture as space
  GESTURE_SWIPE_UP = 0x0f, // up is the same gesture as shift
  GESTURE_SWIPE_DOWN = '1',  // down is the same gesture as the digit 1

  GESTURE_0 = '0',
  GESTURE_1 = '1',
  GESTURE_2 = '2',
  GESTURE_3 = '3',
  GESTURE_4 = '4',
  GESTURE_5 = '5',
  GESTURE_6 = '6',
  GESTURE_7 = '7',
  GESTURE_8 = '8',
  GESTURE_9 = '9',
};

#endif
