#ifndef _BubblesApp_
#define _BubblesApp_

#include "ScreensaverApp.h"

class BubblesApp : public ScreensaverApp {
  public:
    void run(void);
    const char* name(void) { return "Bubbles"; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "bubb";

    bool canBeMouse(void) { return true; }

  private:
    int currentColor = screen.red;
    coord_t lastX, lastY;
    coord_t deltaX, deltaY;

};

#endif
