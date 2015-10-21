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

void GestureApp::run(void) {
  BritepadApp::run();

  if (learndigit < 0) {
    drawDigit(0, screen.red);
    learndigit = 0;
  }

  if (pad.up(SCREEN_PAD)) {
    if (learndigit <= 9) {
      digits[learndigit].setDraw(true);
      if (digits[learndigit].capture()) {
        learndigit++;
        drawDigit(learndigit,screen.red);
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

        for (int i = 0; i < 10; i++) {
          curdist = gesture.compare(digits[i]);
          DEBUG_PARAM_LN("digit",i);
          DEBUG_PARAM_LN("dist",curdist);
          if (curdist < mindist) {
            mindist = curdist;
            mindigit = i;
          }
        }
        gesture.setDraw(screen.green);
        gesture.compare(digits[mindigit]);
        drawDigit(mindigit, screen.green);
      } else {
        // no capture, probably just a tap
        drawDigit(-1, screen.red);
        sound.beep();
      }
    }
  }
}

