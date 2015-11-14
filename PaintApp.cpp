#include "BritepadShared.h"
#include "PaintApp.h"
#include "Screen.h"
#include "Debug.h"
#include "Sound.h"

// Size of the color selection boxes and the paintbrush size
#define PENRADIUS 10

void PaintApp::run() {
  BritepadApp::run();

  // show feedback on touch panels
  if (pad.changed(RIGHT_PAD)) {
    screen.fillCircle(screen.clipWidth(),screen.clipMidHeight(),screen.clipHeight()/4, pad.touched(RIGHT_PAD) ? screen.red : bgColor());
  }

  if (pad.changed(LEFT_PAD)) {
    screen.fillCircle(1,screen.clipMidHeight(),screen.clipHeight()/4, pad.touched(LEFT_PAD) ? screen.red : bgColor());
  }

  if (pad.changed(BOTTOM_PAD)) {
    screen.fillCircle(screen.clipMidWidth(),screen.clipHeight(),screen.clipHeight()/4, pad.touched(BOTTOM_PAD) ? screen.red : bgColor());
  }

  if (pad.touched(SCREEN_PAD)) {
      screen.fillCircle(pad.x(), pad.y(), penRadius, currentColor++);
  }

}

