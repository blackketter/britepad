#include "BritepadShared.h"
#include "HundredBlocksApp.h"

HundredBlocksApp theHundredBlocksApp;

void HundredBlocksApp::begin(AppMode asMode) {

  if (!dots) {
    dots = new DotMatrix(screen.clipLeft(), screen.clipTop(), screen.clipWidth(), screen.clipHeight(), getDotsWide(), getDotsHigh());
  }

  uint16_t block = 0;
  for (coord_t y = 0; y < getDotsHigh(); y++ ) {
    for (coord_t x = 0; x < getDotsWide(); x++) {
      dots->setDot(x,y, block < blocksFilled() ? beforeColor : afterColor);
      block++;
    }
  }

  ClockApp::begin(asMode);
  dots->draw();
}

void HundredBlocksApp::update() {
  uint16_t b = blocksFilled();
  coord_t x = b%getDotsWide();
  coord_t y = b/getDotsHigh();
  dots->setDot(x, y, clock.now()%2 ? beforeColor : afterColor);
  dots->updateDot(x, y);
}

uint16_t HundredBlocksApp::blocksFilled() {
  return ((clock.hour()*60 + clock.minute())*100)/(60*24);
}
