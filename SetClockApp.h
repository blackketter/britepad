#ifndef _SETCLOCKAPP_
#define _SETCLOCKAPP_

#include "BritepadApp.h"
#include "BritepadShared.h"

class SetClockApp : public BritepadApp {
  public:
    SetClockApp(void);
    void begin(void);
    void end(BritepadApp* nextApp);
    BritepadApp* run(void);
    const char* name(void) { return "Set Clock"; };

  private:
    static const int buttoncount = 6;
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

};
#endif


