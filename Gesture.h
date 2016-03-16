#ifndef _Gesture_
#define _Gesture_

#include "Screen.h"
#include "Debug.h"

typedef uint8_t gesture_t;

typedef uint8_t coord8_t;

typedef uint8_t angle8_t;  // 0 = north, 64 = east, 128 = south, 192 = west

struct pointf_t {
  float x;
  float y;
};
struct point8_t {
  coord8_t x;
  coord8_t y;
};


#define DEGREES(x) ((x*256)/360)

static const int samplesPerGesture = 32;

struct gestureShape_t {
  angle8_t orientation;
  point8_t points[samplesPerGesture];
};

//typedef point8_t gestureShape_t[samplesPerGesture];

struct gestureData_t {
  gesture_t gesture;
  angle8_t minOrientation;
  angle8_t maxOrientation;
  gestureShape_t* shape;
};

// Default, built-in gestures
extern const gestureData_t* defaultGestures;

enum {
  NO_GESTURE = 0,

  GESTURE_SWIPE_RIGHT = 0x08, // right is the same gesture as backspace
  GESTURE_SWIPE_LEFT = 0x20, // left is the same gesture as space
  GESTURE_SWIPE_UP = 0x0f, // up is the same gesture as shift
  GESTURE_SWIPE_DOWN = '1',  // down is the same gesture as the digit 1
};

// encapsulates a touchpad gesture, with a series of points that can be compared against other gestures
class Gesture {
  public:
    Gesture() { reset(); }
    void reset() { sampleCount = 0; orientation=0; centroid.x=0;centroid.y=0;}
    bool capture(); // captures from the global touchpad

    uint16_t compare(Gesture &to); // sum of distances of points.  0 is a perfect match
    uint16_t compare(gestureShape_t& to);
    angle8_t getOrientation();

    void setDraw(color_t color) { draw = color; }

    static const gesture_t NO_GESTURE = 0;
    gesture_t match(const gestureData_t* gestureList = defaultGestures, uint16_t* distance = nullptr);

    void getSample8(int i, point8_t& t) { t.x = samplesf[i].x*255; t.y = samplesf[i].y*255; }

  private:
    static const uint16_t MATCH_THRESHOLD = 2000;  // seems about right
    static const uint16_t MATCH_BESTBY = 10;  // percent

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

    float distance(const point_t& p1, const point_t& p2) {  return sqrtf((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)); }
    float distance(const point_t& p1, float p2x, float p2y ) {  return sqrtf((p1.x-p2x)*(p1.x-p2x)+(p1.y-p2y)*(p1.y-p2y)); }
    float distance(const pointf_t& p1, const pointf_t& p2) { return sqrtf((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)); }
    uint16_t distance8(const point8_t& p1, const point8_t& p2) { int32_t p1x = p1.x, p1y = p1.y, p2x = p2.x, p2y = p2.y;
      return sqrtf((p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y));
    }
    void rotateBy(float by);
    uint8_t sampleCount = 0;

    pointf_t samplesf[samplesPerGesture];
    pointf_t centroid;
    float orientation;
    color_t draw = 0;
};

#endif
