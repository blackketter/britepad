#ifndef _SparkleApp_
#define _SparkleApp_

#include "BritepadApp.h"

class SparkleApp : public BritepadApp {
  public:
    void run();

    bool canBeMouse() { return true; }
    bool canBeInteractive() { return true; }
    bool disablesScreensavers() { return getAppMode() == INTERACTIVE_MODE; }


    const char* name() { return "Sparkle"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "spkl";

  private:
    void drawSparkle(int i, color_t c);
    static const int pointCount = 100;
    point_t points[pointCount];
    int currPoint = 0;
    static const int spread = 60;
    static const int radius = 4;
};

#endif
