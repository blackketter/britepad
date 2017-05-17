#include "ZoneClockApp.h"

//Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Standard Time

ZoneClockApp theZoneClock("Paris", CEST, CET);

ZoneClockApp::ZoneClockApp(const char* name, TimeChangeRule dstStart, TimeChangeRule stdStart) {
  _offsetName = name;
  _zone.setRules(dstStart,stdStart);
  _zoneClock.setZone(&_zone);
};

const char* ZoneClockApp::name() {
  sprintf(_nameStr, "%s %d:%02d\xa0%s", _offsetName, _zoneClock.hourFormat12(), _zoneClock.minute(), _zoneClock.isAM() ? "am":"pm");
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
