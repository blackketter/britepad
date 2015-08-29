#ifndef _DotsDisplayApp_
#define _DotsDisplayApp_

#include "ScreensaverApp.h"
#include "Britepad.h"

class DotsDisplayApp : public ScreensaverApp {
  private:
    int pixels_wide = 0;
    int pixels_high = 0;
    color_t* pixels = 0;

    void updateDots(void);
    void updateDot(int x, int y);
    void setDot(int x, int y, color_t c);
    void writeDot(int x, int y, color_t c);

  public:

    void begin(int width, int height) {
      pixels_wide = width;
      pixels_high = height;
      pixels = (color_t*)BritepadApp_SCRATCH_PAD;
    };

    BritepadApp* run(void);
    const char* name(void) { return "Dots"; };
  private:

};

#endif

