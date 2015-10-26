#include "Gesture.h"
#include "Screen.h"
#include "Debug.h"
#include "Touchpad.h"

uint16_t Gesture::compare(const Gesture& to) {
  float dist = 0;
  for (int i = 0; i < getSampleCount(); i++) {
    if (draw) {
      screen.drawLine(samples[i].x*screen.clipWidth()/2+screen.clipMidWidth(),
        samples[i].y*screen.clipHeight()/2+screen.clipMidHeight(),
        to.samples[i].x*screen.clipWidth()/2+screen.clipMidWidth(),
        to.samples[i].y*screen.clipHeight()/2+screen.clipMidHeight(), 4, draw);
    DEBUG_PARAM("i",i);
    DEBUG_PARAM(" to[i].x",to.samples[i].x);
    DEBUG_PARAM(" to[i].y",to.samples[i].y);
    DEBUG_PARAM(" samples[i].x",samples[i].x);
    DEBUG_PARAM_LN(" samples[i].y",samples[i].y);
    }
    dist += distance(samples[i],to.samples[i]);
  }

  return dist;
}

bool Gesture::capture(void) {
  reset();
  if (draw) {
    screen.fillScreen(screen.black);
  }
  point_t* rawPoints = pad.getHistory();
  int rawPointCount = pad.getHistoryCount();
  DEBUG_PARAM_LN("raw point count", rawPointCount);
  if (rawPointCount < minSamplesRequired()) {
    return false;
  }
  // calculate path length
  float pathLength = 0;
  for (int i = 1; i < rawPointCount; i++) {
    pathLength += distance(rawPoints[i],rawPoints[i-1]);
   if (draw) {
      screen.drawLine(rawPoints[i].x,rawPoints[i].y,rawPoints[i-1].x,rawPoints[i-1].y,8, screen.darkblue);
 //     screen.fillCircle(rawPoints[i].x, rawPoints[i].y, 4, screen.blue);
    }
  }
  if (pathLength < minPathLength()) {
    return false;
  }

  float interval = pathLength / (samplesPer-1);

  DEBUG_PARAM_LN("pathLength",pathLength);
  DEBUG_PARAM_LN("interval",interval);
  float dq = 0;

  // resample and add to self
  addSample(rawPoints[0].x,rawPoints[0].y);
  int i = 1;
  float lastpx = rawPoints[i-1].x;
  float lastpy = rawPoints[i-1].y;
  point_t p = rawPoints[i];
  do {
    float dp = distance(p, lastpx, lastpy);
    if ((dq+dp) >= interval) {
      float qx = lastpx + ((interval - dq) / dp) * (p.x - lastpx);
      float qy = lastpy + ((interval - dq) / dp) * (p.y - lastpy);
      addSample(qx,qy);
      lastpx = qx;
      lastpy = qy;
      dq = 0;
    } else {
      dq += dp;
      lastpx = p.x;
      lastpy = p.y;
      i++;
      p = rawPoints[i];
    }
//    DEBUG_PARAM_LN("samples", getSampleCount());
  } while ((i < rawPointCount) && (getSampleCount() < samplesPer-1));

  addSample(rawPoints[rawPointCount-1].x, rawPoints[rawPointCount-1].y);

  DEBUG_PARAM_LN("new samples", getSampleCount());
  if (draw) {
    for (int i = 1; i < getSampleCount(); i++) {
      screen.drawLine(samples[i].x,samples[i].y,samples[i-1].x,samples[i-1].y,3,screen.darkred);
  //    screen.fillCircle(samples[i-1].x, samples[i-1].y, i == 1 ? 8 : 2, screen.red);
    }
  }

  // calculate centroid
  for (int i = 0; i < samplesPer; i++) {
    centroid.x += samples[i].x;
    centroid.y += samples[i].y;
  }
  centroid.x /= samplesPer;
  centroid.y /= samplesPer;
  if (draw) {
    screen.fillCircle(centroid.x, centroid.y, 5, screen.yellow);
  }
  // calculate theta
  float dy = samples[0].y-centroid.y;
  float dx = samples[0].x-centroid.x;
  theta = atan(abs(dy)/abs(dx));
  if (dx < 0) { theta = M_PI - theta; }
  if (dy < 0) { theta = -theta; }

  rotateBy(samples, samples, -theta);

  DEBUG_PARAM_LN("theta", theta);
  if (draw) {
    for (int i = 1; i < getSampleCount(); i++) {
      screen.drawLine(samples[i].x,samples[i].y,samples[i-1].x,samples[i-1].y,2,screen.darkgreen);
//      screen.fillCircle(samples[i-1].x, samples[i-1].y, i == 1 ? 8 : 2, screen.green);
    }
  }

  // find bounding box
  float xmax = samples[0].x;
  float ymax = samples[0].y;
  float xmin = xmax;
  float ymin = ymax;
  for (int i = 1; i < samplesPer; i++) {
    float x = samples[i].x;
    float y = samples[i].y;
    xmax = x > xmax ? x : xmax;
    xmin = x < xmin ? x : xmin;
    ymax = y > ymax ? y : ymax;
    ymin = y < ymin ? y : ymin;
  }

  width = xmax - xmin;
  height = ymax - ymin;

  float aspect = max(width,height);

  // put in unit square and translate to origin
  for (int i = 0; i < samplesPer; i++) {
    samples[i].x = (samples[i].x-centroid.x)/aspect;
    samples[i].y = (samples[i].y-centroid.y)/aspect;
  }
  if (draw) {
    for (int i = 0; i < getSampleCount(); i++) {
      screen.fillCircle(samples[i-1].x*screen.clipWidth()+screen.clipMidWidth(), samples[i-1].y*screen.clipHeight()+screen.clipMidHeight(), i == 1 ? 8 : 2, screen.darkyellow);
    }
  }

  // got a reasonble capture
  return true;
}

void Gesture::rotateBy(const pointf_t* p, pointf_t* q, float by) {
  float sintheta = sin(by);
  float costheta = cos(by);

  for (int i = 0; i < samplesPer; i++) {
    float dx = p[i].x-centroid.x;
    float dy = p[i].y-centroid.y;
    q[i].x = dx*costheta - dy*sintheta + centroid.x;
    q[i].y = dx*sintheta + dy*costheta + centroid.y;
  }
}
