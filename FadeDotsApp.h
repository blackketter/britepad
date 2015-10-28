#ifndef _FadeDotsApp_
#define _FadeDotsApp_

#include "DotsDisplayApp.h"

class FadeDotsApp : public DotsDisplayApp {
  public:
    void begin(AppMode asMode)  { ScreensaverApp::begin(asMode); dots.init(16,12,(color_t*)dotData); };
    void run();

    const char* name() { return "Fade Dots"; };
    static constexpr appid_t ID = "fade";
    appid_t id() { return ID; };
  protected:
    color_t dotData[16][12];
};

#endif


