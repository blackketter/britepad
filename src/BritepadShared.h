#ifndef _BritepadShared_
#define _BritepadShared_

#include "Arduino.h"

#include "Types.h"

#include "Screen.h"
extern Screen screen;

#include "TouchPad.h"
extern TouchPad pad;

#include "Sound.h"
extern Sound sound;

#include "MousePad.h"
extern MousePad mouse;

#include "Britepad.h"
extern Britepad britepad;

#include "Clock.h"
extern TeensyRTCClock clock;

#include "Timer.h"

#include "Preferences.h"
extern Preferences prefs;

#include "fonts/Fonts.h"

#endif
