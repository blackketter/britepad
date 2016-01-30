#ifndef _MinesApp_
#define _MinesApp_
#include "BritepadApp.h"
#include "Button.h"
#include "DotMatrix.h"
#include "Icons.h"
#include "TextField.h"


class MineMatrix : public SquareMatrix {
  public:
    MineMatrix(coord_t x, coord_t y, coord_t w, coord_t h, int dotsWide, int dotsHigh) : SquareMatrix( x,  y,  w,  h,  dotsWide,  dotsHigh) {};
    void updateDot(int x, int y);
    int mined(int x, int y);
    bool revealed( int x, int y);
    int revealed();
    void reveal(int x, int y);
    int neighbors( int x, int y);
};

class MinesApp : public BritepadApp {
  public:
    void begin();
    void end();
    void run();

    virtual bool disablesScreensavers() { return true; }

    const char* name() { return "Mines"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "mine";

  private:
    bool gameOver;
    MineMatrix* field = nullptr;
    const uint8_t minesWidth = 8;
    const uint8_t minesHeight = 8;
    const uint8_t minesMax = 10;
    uint8_t mines;
    millis_t startTime;
    bool firstTap;
    TextField minesLeft;
    TextField timer;
    bool flagged;
    bool youlose;
    bool playTune = true;
};

#endif

