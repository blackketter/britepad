#ifndef _BreakoutApp_
#define _BreakoutApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"

class BreakoutApp : public BritepadApp {
  public:
    const char* name() { return "Breakout"; };
    bool disablesScreensavers() { return dx || dy || (pad.time() - pad.lastTouchedTime(ANY_PAD)) > timeoutMillis; }
    void run();
    void begin(AppMode asMode);

    AppType getAppType() { return INTERACTIVE_APP; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "bkou";

  private:
    void newGame();
    bool hit(coord_t x, coord_t y);

    coord_t ballx, bally;
    coord_t dx, dy;

    coord_t brickw;
    coord_t brickh;
    coord_t topbrickh;
    coord_t brickx(int col);
    coord_t bricky(int row);

    coord_t ballr;

    coord_t paddlew;
    coord_t paddleh;
    coord_t paddlex;

    static const millis_t timeoutMillis = 10000;
    static const millis_t millisPerFrame = 16;
    static const int defaultdx = 3;
    static const int defaultdy = 3;
    millis_t lastDraw = 0;

    static const int rows = 6;
    static const int cols = 16;

    bool bricks[rows][cols];
    int bricksleft;

};

#endif



