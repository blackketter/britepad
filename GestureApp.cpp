#include "GestureApp.h"
#include "BritepadShared.h"
#include "Screen.h"
#include "Debug.h"
#include "Sound.h"
#include "Limits.h"

void GestureApp::drawDigit(int digit, color_t color) {
  screen.setFont(Arial_20);
  screen.setCursor(screen.clipLeft(), screen.clipTop());
  screen.setTextColor(color, bgColor());
  if (digit >= 0 && digit <= 9) {
    screen.drawTextF("%1d",digit);
  }
}

void GestureApp::run() {
  BritepadApp::run();

  // learndigit > 9 = done learning

  if (pad.up(BOTTOM_PAD)) {
    clearScreen();
    if (learndigit > 9) {
      learndigit = 0;
    } else {
      learndigit++;
    }
    sound.beep();
  }

  if (learndigit <= 9) {
    drawDigit(learndigit, screen.red);
  }

  if (pad.up(SCREEN_PAD)) {
    if (learndigit <= 9) {
      digits[learndigit].setDraw(true);
      if (digits[learndigit].capture()) {
        // captured a gesture
        PRINTF("\ngestureShape_t gesture_%d = { ", learndigit);
        for (int i = 0; i < samplesPerGesture; i++) {
          point8_t p;
          digits[learndigit].getSample8(i,p);
          PRINTF("{%d,%d},", p.x,p.y);
        }
        PRINTF(" };\n\n");
      } else {
        // digit didn't capture
        sound.beep();
      }
    } else {
      Gesture gesture;
      clearScreen();
      if (gesture.capture()) {
        // do the comparison
        uint16_t curdist;
        uint16_t mindist = UINT16_MAX;
        int mindigit = 0;

        DEBUGF("Distances: ");
        for (int i = 0; i < 10; i++) {
          curdist = gesture.compare(digits[i]);
          PRINTF("%d:%d, ",i, curdist);
          if (curdist < mindist) {
            mindist = curdist;
            mindigit = i;
          }
        }
        PRINTF("\n");
        gesture.setDraw(screen.green);
        gesture.compare(digits[mindigit]);
        drawDigit(mindigit, screen.green);

        DEBUGF("Testing against standard gestures: '%c' 0x%02x", pad.getGesture(), pad.getGesture());
      } else {
        // no capture, probably just a tap
        drawDigit(-1, screen.red);
        sound.beep();
      }
    }
  }
}

