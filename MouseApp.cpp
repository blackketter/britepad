#include "BritepadShared.h"
#include "MouseApp.h"
#include "Screen.h"
#include "Debug.h"
#include "Sound.h"

// Size of the color selection boxes and the paintbrush size
#define PENRADIUS 10

MouseApp theMouseApp;

void MouseApp::begin(AppMode asMode) {
  clearScreen();
  mouse.begin();
}

void MouseApp::end(BritepadApp* nextApp) {
  mouse.end();
}

BritepadApp* MouseApp::run(void) {
  mouse.run();

  // show feedback on touch panels
  if (pad.changed(RIGHT_PAD)) {
    screen.fillCircle(screen.clipWidth(),screen.clipHeight()/2,screen.clipHeight()/4, pad.touched(RIGHT_PAD) ? screen.red : backgroundColor);
  }

  if (pad.changed(LEFT_PAD)) {
    screen.fillCircle(1,screen.clipHeight()/2,screen.clipHeight()/4, pad.touched(LEFT_PAD) ? screen.red : backgroundColor);
  }

  if (pad.changed(BOTTOM_PAD)) {
    screen.fillCircle(screen.clipWidth()/2,screen.clipHeight(),screen.clipHeight()/4, pad.touched(BOTTOM_PAD) ? screen.red : backgroundColor);
  }

  if (pad.touched(SCREEN_PAD)) {
    // touch down
#if DEBUG_TRACKING
    DEBUG_START("Tracking: ");
   // Print out the raw coordinates
    DEBUG("[raw:"); DEBUG(pad.x());    DEBUG(","); DEBUG(pad.y());
    DEBUG_END("]");
    screen.setCursor(0,0);
    screen.setTextColor(currentColor, 0);
    screen.setFont(Arial_16_Bold);
    screen.drawTextF("%3d, %3d", pad.x(), pad.y());
#endif

// draw fat dots when mouse is down
    int radius = PENRADIUS;
//    if (Mouse.isPressed()) {
//      radius *= 2;
//    }

#if 0
    if (scrollMode) {
      // we are in scroll mode
      DEBUG_LN("scroll mode scrolling");
      if (pad.y() < screen.clipHeight()/2) {
        screen.fillCircle(screen.clipWidth(),screen.clipHeight()/4,screen.clipHeight()/4, currentColor);
      } else {
        screen.fillCircle(screen.clipWidth(),screen.clipHeight()*3/4,screen.clipHeight()/4, currentColor);
      }
    }
#endif
     screen.fillCircle(pad.x(), pad.y(), radius, currentColor++);
  }
// draw delta line
//        screen.drawLine(screen.clipWidth()/2, screen.clipHeight()/2, screen.clipWidth()/2+deltax, screen.clipHeight()/2+deltay, ~currentColor);

// draw page up
//            screen.fillCircle(screen.clipWidth(),screen.clipHeight()/4,screen.clipHeight()/4, currentColor);

// draw page down
//            screen.fillCircle(screen.clipWidth(),screen.clipHeight()*3/4,screen.clipHeight()/4, currentColor);

// draw mouse press?
//          screen.fillCircle(pad.x(), pad.y(), PENRADIUS*2, currentColor);

// draw mouse release after timeout
//            clearScreen();
//            screen.fillCircle(pad.x(), pad.y(), PENRADIUS*2, ~currentColor);

  return STAY_IN_APP;  // never exit
}

