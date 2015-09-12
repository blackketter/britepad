#ifndef _DotsDisplayApp_
#define _DotsDisplayApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"

class DotsDisplayApp : public ScreensaverApp {
  public:
    void begin(void) { ScreensaverApp::begin(); setUpDots(16,12); };
    const char* name(void) { return "Dots"; };
    BritepadApp* run(void);

    appid_t id() { return ID; }
    static constexpr appid_t ID = "dots";

  private:
    int pixels_wide = 0;
    int pixels_high = 0;
    color_t* pixels = 0;

    void updateDots(void);
    void updateDot(int x, int y);
    void setDot(int x, int y, color_t c);
    void writeDot(int x, int y, color_t c);
    void setUpDots(int width, int height);

};

#endif

