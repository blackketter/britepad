#include "BritepadShared.h"
#include "PercentApp.h"

PercentApp thePercentApp;

void PercentApp::begin(AppMode asMode) {

  if (!dots) {
    dots = new DotMatrix(screen.clipLeft(), screen.clipTop(), screen.clipWidth(), screen.clipHeight(), getDotsWide(), getDotsHigh());
  }


  int k = 0;
  int n = dotsNow();
  for (int i = 0; i < getDotsHigh(); i++) {
    for (int j = 0; j < getDotsWide(); j++) {
      dots->setDot(j,i, k > n ? afterColor : beforeColor);
      k++;
    }
  }
  ClockApp::begin(asMode);
  dots->draw();
};

void PercentApp::update() {

  coord_t x,y;
  int n = dotsNow();
  // make sure the last dot is drawn correctly
  if (n > 0) {
    x = (n-1)%getDotsWide();
    y = (n-1)/getDotsWide();
    dots->setDot(x, y, beforeColor);
    dots->updateDot(x, y);
  }

  x = n%getDotsWide();
  y = n/getDotsWide();

  dots->setDot(x, y, nowColor());
  dots->updateDot(x, y);
}

void PercentApp::end() {
   if (dots) {
    delete dots;
    dots = nullptr;
  }
  ClockApp::end();
}


int PercentApp::dotsNow() {
  int totalDots = getDotsWide() * getDotsHigh();

  return (((clock.now() % Time::secsPerDay) * totalDots) / Time::secsPerDay);

}
