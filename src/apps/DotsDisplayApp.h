#ifndef _DotsDisplayApp_
#define _DotsDisplayApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"
#include "widgets/DotMatrix.h"
#include "Screen.h"

class DotsDisplayApp : public ScreensaverApp {
  public:
    void begin();
    void run();
    void end();

    bool canBeMouse() { return true; };

    const char* name() { return "Dots"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "dots";

  protected:
    virtual int getDotsWide() { return screen.width() / dotSize; }
    virtual int getDotsHigh() { return screen.height() / dotSize; }
    DotMatrix* dots = nullptr;
    color_t lastColor;
    const int dotSize = 20;
};

#endif

