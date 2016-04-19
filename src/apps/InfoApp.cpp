#include "InfoApp.h"

InfoApp theInfoApp;

void InfoApp::run() {

  BritepadApp::run();
  screen.setTextColor(screen.white, bgColor());
  screen.setFont(Arial_14);
  screen.setCursor(screen.clipLeft() + screen.fontGap(), screen.clipTop()+screen.fontGap());
  char string[100];
  clock.longTime(string);
  screen.drawTextF("%s %06d\nX: %d Y: %d        \n", string, clock.fracMicros(), pad.x(), pad.y());
  screen.drawTextF("Points captured: %d        \n", pad.getHistoryCount());
  uint8_t raw = pad.getProximityRaw();
  if (raw > max) { max = raw; sound.beep();}
  screen.drawTextF("Distance:%d,raw:%d,max: %d        \n", pad.getProximityDistance(), raw,max);
  screen.drawTextF("Ambient: %d         \n", pad.getAmbientLight());
  screen.drawTextF("Backlight: %d         \n", screen.getBacklight());
  screen.drawTextF("Free ram: %d            \n", FreeMem());
  screen.drawTextF("Uptime: %d\n", (int32_t)(Uptime::millis()/1000));
  screen.drawTextF("FPS: %d      \n", 1000/(pad.time()-lastUpdate));
  lastUpdate = pad.time();
  uint64_t x = 0;
  unsigned long n = millis();
  while (n == millis()){};
  n = millis();
  while (n == millis()) {
    x++;
  }
  screen.drawTextF("uint64_t loops per millis: %d", (unsigned long)x);
};
