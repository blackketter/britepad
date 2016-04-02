#include "FastLED.h"

#include "BritepadShared.h"
#include "TrianglesApp.h"
#include "Debug.h"

void TrianglesApp::begin() {
  if (!dots) {
    dots = new TriangleMatrix(screen.clipLeft(), screen.clipTop(), screen.clipWidth(), screen.clipHeight(), getDotsWide(), getDotsHigh());
  }
  DotsDisplayApp::begin();
}


