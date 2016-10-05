#include "ZoneClockApp.h"

ZoneClockApp theZoneClock("Paris", 60*60*9);

ZoneClockApp::ZoneClockApp(const char* name, stime_t offset) {
  _offset = offset;
  _offsetName = name;
};

const char* ZoneClockApp::name() {
  Time secondTime;
  char shortTime[10];
  secondTime.setSeconds(clock.getSeconds()+_offset);
  secondTime.shortTime(shortTime);
  sprintf(_nameStr, "%s\n%s", _offsetName, shortTime);
  return _nameStr;
};

void ZoneClockApp::update() {
  ClockApp::update();
  screen.setFont(Arial_16);
  screen.setCursor(
    screen.clipMidWidth()-screen.measureTextWidth(_offsetName)/2,
    screen.clipTop()+ screen.clipMidHeight()/3  + screen.measureTextHeight(_offsetName)/2
  );
  screen.drawText(_offsetName);
};
