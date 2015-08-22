#include "Britepad.h"
#include "BubblesApp.h"

void BubblesApp::init(void) {

}

void BubblesApp::end(void) {

}

void BubblesApp::run(void) {
  screen.fillCircle(random(screen.width()), random(screen.height()), random(40), currentColor++);

//  screen.setCursor(random(screen.width()), random(screen.height()));
//  drawTextF("hello %s", "world");
}
