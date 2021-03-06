#include "GestureApp.h"
#include "BritepadShared.h"
#include "Screen.h"
#include "Sound.h"
#include <limits.h>

GestureApp theGestureApp;

void GestureApp::run() {
  BritepadApp::run();

  screen.setFont(&Arial_14);
  screen.setCursor(screen.clipLeft(), screen.clipTop());

  if (pad.released(BOTTOM_PAD)) {
    learning = !learning;
    clearScreen();
    if (learning) {
      sound.beep();
      screen.setTextColor(screen.red);
      screen.drawText("Learning");
    }
  }

  if (pad.pressed(SCREEN_PAD)) {
    clearScreen();
    lastPoint.x = pad.x();
    lastPoint.y = pad.y();
  }

  if (pad.touched(SCREEN_PAD)) {
    screen.fillCircle(pad.x(),pad.y(), 4, screen.yellow);
    screen.drawLine(lastPoint.x, lastPoint.y, pad.x(), pad.y(), screen.yellow);
    lastPoint.x = pad.x();
    lastPoint.y = pad.y();
  }

  if (pad.released(SCREEN_PAD)) {
    if (learning) {
      if (learned.capture()) {
        printData(learned);
        learned.setDraw(true);
      } else {
        // digit didn't capture
        sound.beep();
      }
      learning = false;
    } else {
      Gesture gesture;
      if (gesture.capture()) {
        printData(gesture);
        gesture.setDraw(screen.green);
        uint16_t learnedDist = gesture.compare(learned);

        console.debugf("Match against learned gesture distance: %d\n", learnedDist);
        console.debugf("Match against standard gestures: '%c' 0x%02x, angle: %d\n", pad.getGesture(), pad.getGesture(), pad.getGestureOrientation());
        screen.setTextColor(screen.green);
        if (pad.getGesture() == NO_GESTURE) {
          sound.beep();
          screen.setTextColor(screen.red);
          screen.drawText("No match\n");
        } else if (pad.getGesture() < 0x21) {
          screen.drawTextF("Match: 0x%02x\n", pad.getGesture());
        } else {
          screen.drawTextF("Match: %c\n",pad.getGesture());
        }
        screen.drawTextF("Distance: %d\nLearned distance: %d", pad.getGestureDistance(), learnedDist);

      } else {
        // no capture, probably just a tap
        sound.beep();
      }
    }
  }
}

void GestureApp::printData(Gesture g) {
  // captured a gesture
  console.debugf("\ngestureShape_t gesture_SYMBOL = { %d, {", g.getOrientation());
  for (int i = 0; i < samplesPerGesture; i++) {
    point8_t p;
    g.getSample8(i,p);
    console.debugf("{%d,%d},", p.x,p.y);
  }
  console.debugf(" }};\n\n");
}
