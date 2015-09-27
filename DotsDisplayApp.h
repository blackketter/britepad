#ifndef _DotsDisplayApp_
#define _DotsDisplayApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"
#include "DotMatrix.h"

class DotsDisplayApp : public ScreensaverApp {
  public:
    void begin(bool asScreensaver) { ScreensaverApp::begin(asScreensaver); dots.init(16,12, BritepadAppScratchPad); };
    const char* name(void) { return "Dots"; };
    BritepadApp* run(void);

    appid_t id() { return ID; };
    static constexpr appid_t ID = "dots";

  protected:
    DotMatrix dots;
};

#endif

