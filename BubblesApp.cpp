#include "Britepad.h"
#include "BubblesApp.h"
#include "Debug.h"

void BubblesApp::end(void) {

}

BPApp* BubblesApp::run(void) {
  screen.fillCircle(random(screen.width()), random(screen.height()), random(40), currentColor++);

//  screen.setCursor(random(screen.width()), random(screen.height()));
//  drawTextF("hello %s", "world");
  return nil;
}
