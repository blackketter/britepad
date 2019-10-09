#ifndef _BritepadShared_
#define _BritepadShared_

#include "Arduino.h"

#include "Console.h"
extern Console console;

#include "Screen.h"
extern Screen screen;

#include "BB1KeyMatrix.h"
#include "ErgodoxKeyMatrix.h"
extern BB1KeyMatrix keys;

#include "KeyEventQueue.h"
extern KeyEventQueue keyEvents;

#include "TouchPad.h"
extern TouchPad pad;

#include "Sound.h"
extern Sound sound;

#include "MousePad.h"
extern MousePad mousePad;

#include "Britepad.h"
extern Britepad britepad;

#include "Clock.h"
extern Clock clock;
extern Timezone localTimezone;

#include "Timer.h"

#include "Dictionary.h"
extern EEPROMDictionary prefs;

#include "fonts/Fonts.h"

#include "Utilities.h"

#include "PString.h"

#include "Pointer.h"

#endif
