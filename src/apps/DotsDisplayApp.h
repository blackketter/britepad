#ifndef _DotsDisplayApp_
#define _DotsDisplayApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"
#include "widgets/DotMatrix.h"
#include "Screen.h"

class DotsDisplayApp : public ScreensaverApp {
  public:
    void begin(AppMode asMode);
    void run();
    void end();

    bool canBeMouse() { return true; };

    const char* name() { return "Dots"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "dots";

  protected:
    virtual int getDotsWide() { return 16; }
    virtual int getDotsHigh() { return 12; }
    DotMatrix* dots = nullptr;
    color_t lastColor;
};

#endif

