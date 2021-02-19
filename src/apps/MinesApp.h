#ifndef _MinesApp_
#define _MinesApp_
#include "BritepadApp.h"
#include "widgets/Button.h"
#include "widgets/DotMatrix.h"
#include "widgets/TextField.h"

class MineMatrix : public SquareMatrix {
  private:
    const uint16_t REVEALED_CELL = 0x0001;
    const uint16_t MINED_CELL =    0x0002;
    const uint16_t FLAGGED_CELL =  0x0004;
    const uint16_t LOSER_CELL =    0x0008;

  public:
    MineMatrix(coord_t x, coord_t y, coord_t w, coord_t h, int dotsWide, int dotsHigh) : SquareMatrix( x,  y,  w,  h,  dotsWide,  dotsHigh) {};
    void updateDot(int x, int y);
    bool isMined(int x, int y);
    bool isRevealed( int x, int y);
    bool isFlagged(int x, int y);
    bool isLoser(int x, int y);
    void setMined(int x, int y, bool r);
    void setRevealed( int x, int y, bool r);
    void setFlagged(int x, int y, bool r);
    void setLoser(int x, int y, bool r);
    int countRevealed();
    void revealNeighbors(int x, int y);
    int countNeighbors( int x, int y);
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

    AppType getAppType() { return INTERACTIVE_APP; }

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
    bool flagging;
    bool youlose;
    bool playTune = true;
};

#endif

