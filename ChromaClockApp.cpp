#include "ChromaClockApp.h"
#include "Debug.h"

void ChromaClockApp::begin() {
  ClockApp::begin();

  update();
}

void ChromaClockApp::drawSlice(int index, color_t c, coord_t r) {
  coord_t x1, y1, x2, y2;
  uint16_t slice = 0x10000/slices;
  uint16_t theta = slice * index;
  uint16_t halfSlice = slice/2;

  dotpos(theta+halfSlice, r, x1, y1);
  dotpos(theta-halfSlice, r, x2, y2);

  screen.fillTriangle(screen.clipMidWidth(), screen.clipMidHeight(), x1, y1, x2, y2, c);
}

void ChromaClockApp::update() {
  coord_t r[slices];
  color_t c[slices];

  for (int i = 0; i < slices; i++) {
    c[i] = screen.black;
    r[i] = screen.clipHeight()/2;
  }

  int h = (int)(clock.hourFormat12()) * slices / 12;

  for (int i = h-2; i<=h+2; i++) {
    int ic = i;
    if (ic < 0) { ic+=slices; }
    if (ic >= slices) { ic -= slices; }
    c[ic] = screen.red;
//    r[ic] = screen.clipHeight()/4;
  }


  int m = (int)clock.minute() * slices / 60;

  for (int i = m; i<=m; i++) {
    int ic = i;
    if (ic < 0) { ic+=slices; }
    if (ic >= slices) { ic -= slices; }
    c[ic] = screen.green;
    r[ic] = screen.clipHeight()/2;
  }

  int s = (int)clock.second() * slices / 60;
  c[s] = screen.blue;
  r[s] = screen.clipHeight()/2;

  for (int i = 0; i < slices; i++) {
    drawSlice(i,c[i], r[i]);
  }
}
