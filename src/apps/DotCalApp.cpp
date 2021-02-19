#include "BritepadShared.h"
#include "DotCalApp.h"

DotCalApp theDotCalApp;

void DotCalApp::begin() {

  if (!dots) {
    dots = new DotMatrix(screen.clipLeft(), screen.clipTop(), screen.clipWidth(), screen.clipHeight(), getDotsWide(), getDotsHigh());
  }

  Time t;
  uint16_t y = clock.year();
  t.setDateTime(y);
  while (t.year() == y) {
    dots->setDot(t.day()-1,t.month()-1, t.millis() < clock.millis() ? beforeColor : afterColor);
    t.adjustSeconds(Time::secsPerDay);
  }

  ClockApp::begin();
  dots->draw();
};

void DotCalApp::update() {

  // make sure the last dot is drawn correctly
  Time t(clock);
  t.adjustSeconds(-Time::secsPerDay);

  if (t.year() == clock.year()) {
    dots->setDot(t.day()-1, t.month()-1, beforeColor);
    dots->updateDot(t.day()-1, t.month()-1);
  }

  dots->setDot(clock.day()-1, clock.month()-1, clock.now()%2 ? beforeColor : afterColor);
  dots->updateDot(clock.day()-1, clock.month()-1);
}

void DotCalApp::end() {
   if (dots) {
    delete dots;
    dots = nullptr;
  }
  ClockApp::end();
}


