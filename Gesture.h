#ifndef _Gesture_
#define _Gesture_

#include "Screen.h"
#include "Debug.h"
#include "DefaultGestures.h"

typedef uint8_t gesture_t;

typedef uint8_t angle8_t;  // 0 = north, 64 = east, 128 = south, 192 = west

#define DEGREES(x) ((x*256)/360)

struct pointf_t {
  float x;
  float y;
};
struct point8_t {
  int8_t x;
  int8_t y;
};
static const int samplesPerGesture = 32;

typedef point_t gestureShape_t[samplesPerGesture];

struct gestureData_t {
  gesture_t gesture;
  angle8_t minOrientation;
  angle8_t maxOrientation;
  gestureShape_t* shape;
};

typedef gestureData_t gestureList_t[];

extern const gestureList_t defaultGestures;

// encapsulates a touchpad gesture, with a series of points that can be compared against other gestures
class Gesture {
  public:
    Gesture() { reset(); }
    void reset() { sampleCount = 0; width=0; height=0; orientation=0; centerx=0; centery=0; centroid.x=0;centroid.y=0;}
    bool capture(); // captures from the global touchpad
    uint16_t compare(Gesture &to); // sum of distances of points.  0 is a perfect match
    uint16_t compare(gestureShape_t& to);

    void setDraw(color_t color) { draw = color; }

    static const gesture_t NO_GESTURE = 0;
    gesture_t match(const gestureList_t gestureList = defaultGestures);

    void getSample8(int i, point8_t& t) { t.x = samplesf[i].x*128; t.y = samplesf[i].y*128; }

  private:
    static const uint16_t MATCH_THRESHOLD = 1000;  // todo
    uint8_t getSampleCount() { return sampleCount; }
    int minSamplesRequired() { return 10; }  // seems like a reasonable number
    int minPathLength() { return 25; }

    void addSample(float x, float y) { samplesf[sampleCount].x = x; samplesf[sampleCount].y = y; sampleCount++; }
    void setSample(int i, float x, float y) { samplesf[i].x = x; samplesf[i].y = y; }
    void setSample(int i, const point_t& pt) { samplesf[i].x = pt.x; samplesf[i].y = pt.y; }

    void getSample(int i, float& x, float& y) { x = samplesf[i].x; y = samplesf[i].y; }
    void getSample(int i, pointf_t& t) { t.x = samplesf[i].x; t.y = samplesf[i].y; }

    float getSampleX(int i) { return samplesf[i].x; }
    float getSampleY(int i) { return samplesf[i].y; }

    float distance(const point_t& p1, const point_t& p2) {  return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)); }
    float distance(const point_t& p1, float p2x, float p2y ) {  return sqrt((p1.x-p2x)*(p1.x-p2x)+(p1.y-p2y)*(p1.y-p2y)); }
    float distance(const pointf_t& p1, const pointf_t& p2) { return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)); }
    uint16_t distance8(const point8_t& p1, const point8_t& p2) { int32_t p1x = p1.x, p1y = p1.y, p2x = p2.x, p2y = p2.y;
      return sqrt((p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y));
    }
    void rotateBy(float by);
    uint8_t sampleCount = 0;

    pointf_t samplesf[samplesPerGesture];
    pointf_t centroid;
    float orientation;
    color_t draw = 0;

    coord_t width;
    coord_t height;
    coord_t centerx;
    coord_t centery;
};

#endif
