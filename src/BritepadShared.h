#ifndef _BritepadShared_
#define _BritepadShared_

#include "Arduino.h"

#include "Console.h"
extern Console console;

#include "Screen.h"
extern Screen screen;

#include "KeyMatrix.h"
extern KeyMatrix keys;

#include "TouchPad.h"
extern TouchPad pad;

#include "Sound.h"
extern Sound sound;

#include "MousePad.h"
extern MousePad mouse;

#include "Britepad.h"
extern Britepad britepad;

#include "Clock.h"
extern Clock clock;

#include "Timer.h"

#include "Preferences.h"
extern Preferences prefs;

#include "fonts/Fonts.h"

#include "Utilities.h"

#include "PString.h"

#endif
