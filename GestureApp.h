#ifndef _GestureApp_
#define _GestureApp_

#include "BritepadApp.h"
#include "FastLED.h"
#include "Gesture.h"
class GestureApp : public BritepadApp {
  public:

    void run(void);

    bool disablesScreensavers(void) { return true; };

    const char* name(void) { return "Gesture"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "gest";

  private:
    void drawDigit(int digit, color_t color);
    int learndigit = -1;
    Gesture digits[10];
};

#endif
