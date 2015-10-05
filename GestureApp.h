#ifndef _GestureApp_
#define _GestureApp_

#include "BritepadApp.h"

class Gesture {
  public:
    void reset() { sampleCount = 0; width=0; height=0; angle=0; centerx=0; centery=0; }
    void addSample(coord_t x, coord_t y);
    uint8_t getSampleCount(void) { return sampleCount; }
    uint16_t compare(Gesture* to);

  private:
    static const int samplesPer = 64;
    uint8_t sampleCount = 0;
    coord_t x[samplesPer];
    coord_t y[samplesPer];
    uint16_t angle;
    coord_t width;
    coord_t height;
    coord_t centerx;
    coord_t centery;
};

class GestureApp : public BritepadApp {
  public:
    BritepadApp* run(void);

    bool canBeInteractive() { return true; }

    const char* name(void) { return "Gesture"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "gest";

  private:
};

#endif
