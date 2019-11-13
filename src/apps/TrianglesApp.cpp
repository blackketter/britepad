#include "BritepadShared.h"
#include "TrianglesApp.h"

TrianglesApp theTrianglesApp;

void TrianglesApp::begin(AppMode asMode) {
  if (!dots) {
    coord_t tWidth = screen.clipWidth() / (getDotsWide());
    coord_t tHeight = screen.clipHeight() / (getDotsHigh());
    // triangles hang off the edge of the screen
    dots = new TriangleMatrix(screen.clipLeft()-tWidth, screen.clipTop()-tHeight, screen.clipWidth()+tWidth*2, screen.clipHeight()+tHeight*2, getDotsWide(), getDotsHigh());
  }
  DotsDisplayApp::begin(asMode);
}


