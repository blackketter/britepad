#include "ChromaClockApp.h"

ChromaClockApp theChromaClockApp;

void ChromaClockApp::begin(AppMode asMode) {
  ClockApp::begin(asMode);

  update();
}

void ChromaClockApp::run() {
  ClockApp::run();

  millis_t start = Uptime::millis();
  while (Uptime::millis() - start < _maxDrawTime) {
    drawSlice(_curSlice, c[_curSlice],r[_curSlice]);
    _curSlice++;
    if (_curSlice >= _slices) {
      _curSlice = 0;
    };
  }
}

void ChromaClockApp::drawSlice(int index, color_t c, coord_t r) {
  coord_t x1, y1, x2, y2;
  uint16_t slice = 0x10000/_slices;
  uint16_t theta = slice * index;
  uint16_t halfSlice = slice/2;

  screen.polarToRect(theta+halfSlice, r, x1, y1);
  screen.polarToRect(theta-halfSlice, r, x2, y2);

  screen.fillTriangle(screen.clipMidWidth(), screen.clipMidHeight(), x1, y1, x2, y2, c);
}

void ChromaClockApp::update() {

  for (int i = 0; i < _slices; i++) {
    c[i] = screen.grey10;
    r[i] = screen.clipHeight()/2;
  }

  int h = (int)(clock.hourFormat12()) * _slices / 12 + (int)clock.minute() * _slices / (60*12) + (int)clock.second() * _slices / (12*60*60);

  int steps = _slices/6;
  for (int i = 0; i < steps; i++) {
    int ic = h - i;

    while (ic < 0) { ic+=_slices; }

    while (ic >= _slices) { ic -= _slices; }

    c[ic] = screen.add(RGBtoC16(255-(i*255/steps),0,0),c[ic]);
  }


  int m = (int)clock.minute() * _slices / 60 + (int)clock.second() * _slices / (60*60);

  steps = _slices/12;
  for (int i = 0; i < steps; i++) {
    int ic = m - i;
    while (ic < 0) { ic+=_slices; }
    while (ic >= _slices) { ic -= _slices; }

    c[ic] = screen.add(RGBtoC16(0,255-(i*255/steps),0), c[ic]);
  }

  int s = (int)clock.second() * _slices / 60 + (int32_t)clock.fracMillis() * _slices / 1000 / 60;

  steps = _slices/24;
  for (int i = 0; i < steps; i++) {
    int ic = s - i;
    while (ic < 0) { ic+=_slices; }
    while (ic >= _slices) { ic -= _slices; }
    c[ic] = screen.add(RGBtoC16(0,0,255-(i*255/steps)), c[ic]);
  }

}
