#include "BritepadShared.h"
#include "TrianglesApp.h"
#include "Debug.h"

TrianglesApp theTrianglesApp;

void TrianglesApp::begin(AppMode asMode) {
  if (!dots) {
    coord_t tWidth = screen.clipWidth() / (getDotsWide()-2);
    dots = new TriangleMatrix(screen.clipLeft()-tWidth, screen.clipTop(), screen.clipWidth()+tWidth*2, screen.clipHeight(), getDotsWide(), getDotsHigh());
  }
  DotsDisplayApp::begin(asMode);
}


