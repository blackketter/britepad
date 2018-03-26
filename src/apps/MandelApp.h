#ifndef _MandelApp_
#define _MandelApp_

#include "ScreensaverApp.h"

class MandelApp : public ScreensaverApp {
  public:
    void run();
    void begin(AppMode asMode);
    void end();
    bool canBeMouse() { return false; }
    bool canBeInteractive() { return true; }

    const char* name() { return "Mandel"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "mand";

    void restart();

  private:
    float xOrigin;
    float yOrigin;
    float scale = 1.0f;
    uint32_t pixelSum;
    uint32_t lastPixelSum;
    coord_t xPixel;
    coord_t yPixel;
    float x1, x2,y1,y2,sx,sy;
    color_t* _column;
};

#endif
