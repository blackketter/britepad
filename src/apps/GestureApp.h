#ifndef _GestureApp_
#define _GestureApp_

#include "BritepadApp.h"
#include "FastLED.h"
#include "Gesture.h"
class GestureApp : public BritepadApp {
  public:

    void run();

    bool disablesScreensavers() { return true; };

    const char* name() { return "Gesture"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "gest";

    AppType getAppType() { return INTERACTIVE_APP; }

  private:
    bool learning = false;
    Gesture learned;
    point_t lastPoint;
    void printData(Gesture g);
};

#endif
