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

  screen.polarToRect(theta+halfSlice, r, x1, y1);
  screen.polarToRect(theta-halfSlice, r, x2, y2);

  screen.fillTriangle(screen.clipMidWidth(), screen.clipMidHeight(), x1, y1, x2, y2, c);
}

void ChromaClockApp::update() {
  coord_t r[slices];
  color_t c[slices];

  for (int i = 0; i < slices; i++) {
    c[i] = screen.grey10;
    r[i] = screen.clipHeight()/2;
  }

  int h = (int)(clock.hourFormat12()) * slices / 12 + (int)clock.minute() * slices / (60*12) + (int)clock.second() * slices / (12*60*60);

  int steps = slices/6;
  for (int i = 0; i < steps; i++) {
    int ic = h - i;

    while (ic < 0) { ic+=slices; }

    while (ic >= slices) { ic -= slices; }

    c[ic] = screen.add(RGBtoC16(255-(i*255/steps),0,0),c[ic]);
  }


  int m = (int)clock.minute() * slices / 60 + (int)clock.second() * slices / (60*60);

  steps = slices/30;
  for (int i = 0; i < steps; i++) {
    int ic = m - i;
    while (ic < 0) { ic+=slices; }
    while (ic >= slices) { ic -= slices; }

    c[ic] = screen.add(RGBtoC16(0,255-(i*255/steps),0), c[ic]);
  }

  int s = (int)clock.second() * slices / 60 + (int32_t)clock.fracMillis() * slices / 1000 / 60;

  steps = slices/60;
  for (int i = 0; i < steps; i++) {
    int ic = s - i;
    while (ic < 0) { ic+=slices; }
    while (ic >= slices) { ic -= slices; }
    c[ic] = screen.add(RGBtoC16(0,0,255-(i*255/steps)), c[ic]);
  }

  for (int i = 0; i < slices; i++) {
    drawSlice(i,c[i], r[i]);
  }
}
