#ifndef _SpiroApp_
#define _SpiroApp_

#include "ScreensaverApp.h"

class SpiroApp : public ScreensaverApp {
  public:
    void run();
    void begin();

    bool canBeInteractive() { return true; };  // bottom bar displays info

    const char* name() { return "Spiro"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "spir";

  protected:

  private:
    void reset();
    void drawInfo();
    float theta;
    float dTheta;
    float endTheta;
    float scale;
    int r;  // outside circle radius
    int R;  // rolling circle radius
    int d;  // pen distance from center of rolling circle
    color_t currColor = screen.red;
    point_t center; // center of outside circle
    point_t lastP;
    bool first;
    int iters = 0;
    bool displayInfo = false;
};

#endif
