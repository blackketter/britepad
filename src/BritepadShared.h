#ifndef _BritepadShared_
#define _BritepadShared_

#include "Arduino.h"

#include "Clock.h"
extern Clock clock;
extern Timezone localTimezone;

#include "Timer.h"

#include "Console.h"
extern Console console;

#include "Screen.h"
extern Screen screen;

#include "BritepadKeyEventQueue.h"
extern BritepadKeyEventQueue keyEvents;

#include "TouchPad.h"
extern TouchPad pad;

#include "Sound.h"
extern Sound sound;

#include "MousePad.h"
extern MousePad mousePad;

#include "BB1KeyMatrix.h"
#include "ErgodoxKeyMatrix.h"
#if defined(BB1)
extern BB1KeyMatrix keys;
#elif defined(BB100)
#include "BB100KeyMatrix.h"
extern BB100KeyMatrix keys;
#endif

#include "Dictionary.h"
extern EEPROMDictionary prefs;

#include "fonts/Fonts.h"

#include "Utilities.h"

#include "PString.h"

#include "Pointer.h"

#include "apps/BritepadApp.h"

#include "BritepadKeyEventQueue.h"

#include "Britepad.h"
extern Britepad britepad;

#endif
