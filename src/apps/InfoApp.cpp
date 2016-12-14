#include "InfoApp.h"

InfoApp theInfoApp;

void InfoApp::run() {

  BritepadApp::run();
  screen.setTextColor(screen.white, bgColor());
  screen.setFont(Arial_12);
  screen.setCursor(screen.clipLeft() + screen.fontGap(), screen.clipTop()+screen.fontGap());
  char string[100];
  clock.longTime(string);
  screen.drawTextF("%s %06d\nX: %3d Y: %3d\n", string, clock.fracMicros(), pad.x(), pad.y());
  screen.drawTextF("Points captured: %3d\n", pad.getHistoryCount());
  uint8_t raw = pad.getProximityRaw();
  if (raw > max) { max = raw; sound.beep();}
  screen.drawTextF("Distance:%d,raw:%d,max: %d\n", pad.getProximityDistance(), raw,max);
  screen.drawTextF("Ambient: %3d\n", pad.getAmbientLight());
  screen.drawTextF("Backlight: %3d\n", screen.getBacklight());
  screen.drawTextF("Free ram: %10d\n", FreeMem());
  screen.drawTextF("Uptime: %f\n", Uptime::micros()/1000000.0);
  screen.drawTextF("FPS: %5.2f\n", 1000.0/(pad.time()-lastUpdate));
  lastUpdate = pad.time();
  
  millis_t n = Uptime::millis();
  
  while (n == Uptime::millis()) {}
  n++;
  
  uint32_t x = 0;
  while (n == Uptime::millis()) {
    x++;
  }
  
  screen.drawTextF("Uptime::millis(): %4d   \n", x);

  n = millis();
  while (n == millis()) {};
  n++;
  
  x = 0;
  while (n == millis()) {
    x++;
  }
  screen.drawTextF("millis(): %4d   \n", x);


  n = Uptime::micros()+1000;
  x = 0;

  while (n > Uptime::micros()) {
    x++;
  }
  screen.drawTextF("Uptime::micros(): %4d   \n", x);

  x = 0;
  n = micros() + 1000;
  while (n > micros()) {
    x++;
  }
  screen.drawTextF("micros(): %4d   \n", x);
};
