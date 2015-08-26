#ifndef _SETCLOCKAPP_
#define _SETCLOCKAPP_

#include "BPApp.h"
#include "Britepad.h"

class SetClockApp : public BPApp {
  private:
    static const int buttoncount = 8;
    static const int buttonradius = 24;
    static const int nobutton = -1;

    void drawClock(void);
    void drawButton(int i, color_t color);
    int hitButton(int x, int y);
    void drawButtons(void);
    time_t lastTime = 0;
    int buttonx[buttoncount];
    int buttony[buttoncount];
    char buttonsym[buttoncount];

  public:
    SetClockApp(void);
    void begin(void);
    BPApp* run(void);
    const char* name(void) { return "Set Clock"; };
};
#endif


