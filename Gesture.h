#ifndef _Gesture_
#define _Gesture_

#include "Screen.h"
#include "Debug.h"

struct pointf_t {
  float x;
  float y;
};

class Gesture {
  public:
    Gesture() { reset(); }
    void reset() { sampleCount = 0; width=0; height=0; theta=0; centerx=0; centery=0; centroid.x=0;centroid.y=0;}
    bool capture();
    uint16_t compare(const Gesture &to);
    void addSample(float x, float y) { samples[sampleCount].x = x; samples[sampleCount].y = y; sampleCount++; }
    uint8_t getSampleCount(void) { return sampleCount; }
    uint16_t compare(Gesture* to);
    int minSamplesRequired() { return 10; }  // seems like a reasonable number
    int minPathLength() { return 25; }
    void setDraw(color_t color) { draw = color; }

  private:
    float distance(const point_t& p1, const point_t& p2) {  return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)); }
    float distance(const point_t& p1, float p2x, float p2y ) {  return sqrt((p1.x-p2x)*(p1.x-p2x)+(p1.y-p2y)*(p1.y-p2y)); }
    float distance(const pointf_t& p1, const pointf_t& p2) { return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)); }

    void rotateBy(const pointf_t* p, pointf_t* q, float by);
    static const int samplesPer = 64;
    uint8_t sampleCount = 0;

    pointf_t samples[samplesPer];
    pointf_t centroid;
    float theta;
    color_t draw = 0;

    coord_t width;
    coord_t height;
    coord_t centerx;
    coord_t centery;
};

#endif
