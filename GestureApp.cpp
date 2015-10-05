#include "GestureApp.h"

#include "BritepadShared.h"
#include "PaintApp.h"
#include "Screen.h"
#include "Debug.h"
#include "Sound.h"

// Size of the color selection boxes and the paintbrush size
#define PENRADIUS 10

BritepadApp* GestureApp::run(void) {
  BritepadApp::run();

  return STAY_IN_APP;  // never exit
}

