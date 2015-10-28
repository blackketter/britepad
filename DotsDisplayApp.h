#ifndef _DotsDisplayApp_
#define _DotsDisplayApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"
#include "DotMatrix.h"

class DotsDisplayApp : public ScreensaverApp {
  public:
    void begin(AppMode asMode) { ScreensaverApp::begin(asMode); dots.init(16,12, (color_t*)dotData); };
    void run();

    bool canBeMouse() { return true; };

    const char* name() { return "Dots"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "dots";

  protected:
    color_t dotData[16][12];
    DotMatrix dots;
    color_t lastColor;
};

#endif

