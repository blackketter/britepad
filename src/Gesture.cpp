#include "Gesture.h"
#include "Screen.h"
#include "TouchPad.h"
#include "BritepadShared.h"

uint16_t Gesture::compare(Gesture& to) {
  uint16_t dist = 0;
  for (int i = 0; i < getSampleCount(); i++) {
    if (draw) {
      screen.drawWideLine(getSampleX(i)*screen.clipWidth()/2+screen.clipMidWidth(),
        getSampleY(i)*screen.clipHeight()/2+screen.clipMidHeight(),
        to.getSampleX(i)*screen.clipWidth()/2+screen.clipMidWidth(),
        to.getSampleY(i)*screen.clipHeight()/2+screen.clipMidHeight(), 4, draw);
    }
    point8_t fromPoint, toPoint;
    getSample8(i, fromPoint);
    to.getSample8(i, toPoint);
    dist += distance8(fromPoint,toPoint);
  }

  return dist;
}

uint16_t Gesture::compare(gestureShape_t& to) {
  uint16_t dist = 0;
  for (int i = 0; i < getSampleCount(); i++) {
    point8_t fromPoint, toPoint;
    getSample8(i, fromPoint);
    toPoint.x = to.points[i].x;
    toPoint.y = to.points[i].y;
    dist += distance8(fromPoint,toPoint);
  }

  return dist;
}

bool Gesture::capture() {
  reset();
  if (draw) {
    screen.fillScreen(screen.black);
  }
  point_t* rawPoints = pad.getHistory();
  int rawPointCount = pad.getHistoryCount();
//  console.debugf("raw point count %d\n", rawPointCount);
  if (rawPointCount < minSamplesRequired()) {
    return false;
  };
  // calculate path length
  float pathLength = 0;
  for (int i = 1; i < rawPointCount; i++) {
    pathLength += distance(rawPoints[i],rawPoints[i-1]);
   if (draw) {
      screen.drawWideLine(rawPoints[i].x,rawPoints[i].y,rawPoints[i-1].x,rawPoints[i-1].y,8, screen.darkblue);
 //     screen.fillCircle(rawPoints[i].x, rawPoints[i].y, 4, screen.blue);
    }
  }
  if (pathLength < minPathLength()) {
    return false;
  }

  float interval = pathLength / (samplesPerGesture-1);

  console.debugf("pathLength: %d\n",pathLength);
  console.debugf("interval %f\n",interval);
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
  } while ((i < rawPointCount) && (getSampleCount() < samplesPerGesture-1));

  addSample(rawPoints[rawPointCount-1].x, rawPoints[rawPointCount-1].y);

  console.debugf("new samples %d\n", getSampleCount());
  if (draw) {
    for (int i = 1; i < getSampleCount(); i++) {
      screen.drawWideLine(getSampleX(i),getSampleY(i),getSampleX(i-1),getSampleY(i-1),3,screen.darkred);
    }
  }

  // calculate centroid
  for (int i = 0; i < samplesPerGesture; i++) {
    centroid.x += getSampleX(i);
    centroid.y += getSampleY(i);
  }

  centroid.x /= samplesPerGesture;
  centroid.y /= samplesPerGesture;

  if (draw) {
    screen.fillCircle(centroid.x, centroid.y, 5, screen.yellow);
  }

  // calculate the orientation (i.e. the angle, relative to the centroid, where the first touch was placed
  float dy = getSampleY(0)-centroid.y;
  float dx = getSampleX(0)-centroid.x;
  orientation = atan(abs(dy)/abs(dx));
  if (dx < 0) { orientation = M_PI - orientation; }
  if (dy < 0) { orientation = -orientation; }

  rotateBy(-orientation);

  if (draw) {
    for (int i = 1; i < getSampleCount(); i++) {
      screen.drawWideLine(getSampleX(i),getSampleY(i),getSampleX(i-1),getSampleY(i-1),2,screen.darkgreen);
    }
  }

  // find bounding box
  float xmax = getSampleX(0);
  float ymax = getSampleY(0);
  float xmin = xmax;
  float ymin = ymax;
  for (int i = 1; i < samplesPerGesture; i++) {
    float x = getSampleX(i);
    float y = getSampleY(i);
    xmax = x > xmax ? x : xmax;
    xmin = x < xmin ? x : xmin;
    ymax = y > ymax ? y : ymax;
    ymin = y < ymin ? y : ymin;
  }

  float width = xmax - xmin;
  float height = ymax - ymin;

  float aspect = max(width,height);

  console.debugf("orientation: %f\n", orientation);
  console.debugf("xmax: %f, xmin: %f, ymax: %f, ymin: %f, width: %d, height: %d, aspect: %f\n", xmax,xmin,ymax,ymin,width,height,aspect);
  console.debugf("centroid %f,%f\n",centroid.x, centroid.y);

  // put in unit square and translate to origin
  for (int i = 0; i < samplesPerGesture; i++) {
    setSample(i, (getSampleX(i)-xmin)/aspect, (getSampleY(i)-ymin)/aspect);
//    console.debugf("sample[%d] = %f, %f\n", i, getSampleX(i), getSampleY(i));
  }
  if (draw) {
    for (int i = 0; i < getSampleCount(); i++) {
      screen.fillCircle(getSampleX(i)*screen.clipWidth()+screen.clipLeft(), getSampleY(i)*screen.clipHeight()+screen.clipTop(), i == 1 ? 8 : 2, screen.darkyellow);
    }
  }


  // got a reasonable capture
  return true;
}

void Gesture::rotateBy(float by) {
  float sinOrientation = sinf(by);
  float cosOrientation = cosf(by);

  for (int i = 0; i < samplesPerGesture; i++) {
    float dx = getSampleX(i)-centroid.x;
    float dy = getSampleY(i)-centroid.y;
    setSample(i, dx*cosOrientation - dy*sinOrientation + centroid.x, dx*sinOrientation + dy*cosOrientation + centroid.y);
  }
}

angle8_t Gesture::getOrientation() {
  return (uint8_t)((orientation + M_PI)*256/(2*M_PI))-64;
}

gesture_t Gesture::match(const gestureData_t* gestureList, uint16_t* distance) {
  int currGestureIndex = 0;
  gesture_t bestGesture = NO_GESTURE;
  uint16_t bestDistance = UINT16_MAX;
  uint16_t secondBestDistance = UINT16_MAX;

  angle8_t o8 = getOrientation();

  while (gestureList[currGestureIndex].gesture != NO_GESTURE) {
    const gestureData_t* d = &gestureList[currGestureIndex];

    angle8_t min = d->minOrientation;
    angle8_t max = d->maxOrientation;

    bool validOrientation = false;
    if (min == max) {
      validOrientation = true;
    } else if (min < max) {
      if (o8 < max && o8 > min) {
        validOrientation = true;
      }
    } else if (o8 > min || o8 < max) {
      validOrientation = true;
    }

    gesture_t currGesture = gestureList[currGestureIndex].gesture;
    if (validOrientation) {
      uint16_t currDist = compare(*(gestureList[currGestureIndex].shape));
      if (currGesture < 0x20) {
        console.debugf("Match orientation & distance for 0x%02x: %d\n", currGesture, currDist);
      } else {
        console.debugf("Match orientation & distance for '%c': %d\n", currGesture, currDist);
      }

      if (currDist < bestDistance) {
        secondBestDistance = bestDistance;
        bestDistance = currDist;
        bestGesture = currGesture;
      } else if (currDist < secondBestDistance) {
        secondBestDistance = currDist;
      }
    } else {
      if (currGesture < 0x20) {
        //console.debugf("No match on orientation for 0x%02x\n", currGesture);
      } else {
        //console.debugf("No match on orientation for '%c' \n", currGesture);
      }
    }
    currGestureIndex++;
  }

  int bestBy = (secondBestDistance - bestDistance)*100/secondBestDistance;

  console.debugf("bestDistance: %d, secondBest: %d, bestby: %d\%\n", bestDistance, secondBestDistance, bestBy);
  if ( (bestDistance > MATCH_THRESHOLD) ||
       (bestBy < MATCH_BESTBY)) {
    bestGesture = NO_GESTURE;
  }

  if (distance) { *distance = bestDistance; }

  return bestGesture;
}

