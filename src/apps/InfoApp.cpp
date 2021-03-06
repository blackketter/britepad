#include "InfoApp.h"
#include "BritepadShared.h"

void printBpInfo(Print* p) {
  char string[100];
  clock.longTime(string);

  p->printf("Time: %s\n", string);
  p->printf("Compiled: " __DATE__ " " __TIME__ "\n");
  p->printf("Free ram: %10d\n", FreeMem());
  p->printf("Uptime: %f\n", Uptime::micros()/1000000.0);
  p->printf("Prefs usage: %d, remaining: %d\n", prefs.used(), prefs.remaining());
  p->println(resetType());

  uint32_t rtcMillis = (uint32_t)((clock.getRTCMicros()%1000000)/1000);
  uint32_t clockMillis = clock.fracMillis();

  p->printf("RTC millis:%03d, clock: %03d, diff: %d\n", (uint32_t)rtcMillis, (uint32_t)clockMillis, (int)rtcMillis - (int)clockMillis);


  p->printf("Touchscreen X: %3d Y: %3d %s\n",  pad.x(), pad.y(), pad.touched() ? "TOUCHED" : "");
#ifndef __IMXRT1062__
  p->printf("Touch: %c(%d) %c(%d) %c(%d) %c(%d)\n",
    pad.touched(TOP_PAD) ? 'T':'t', touchRead(T_TOUCH_PIN),
    pad.touched(LEFT_PAD) ? 'L':'l', touchRead(L_TOUCH_PIN),
    pad.touched(TOP_PAD) ? 'B':'b', touchRead(B_TOUCH_PIN),
    pad.touched(RIGHT_PAD) ? 'R':'r', touchRead(R_TOUCH_PIN));
#endif
  p->printf("Resolution: %d, %d\n", screen.width(), screen.height());
  p->printf("Ambient: %3d\n", pad.getAmbientLight());
  p->printf("Backlight: %3d\n", screen.getBacklight());

/*
  millis_t n = Uptime::millis();

  while (n == Uptime::millis()) {}
  n++;

  uint32_t x = 0;
  while (n == Uptime::millis()) {
    x++;
  }

  p->printf("Uptime::millis(): %4d   \n", x);

  n = millis();
  while (n == millis()) {};
  n++;

  x = 0;
  while (n == millis()) {
    x++;
  }
  p->printf("millis(): %4d   \n", x);


  n = Uptime::micros()+1000;
  x = 0;

  while (n > Uptime::micros()) {
    x++;
  }
  p->printf("Uptime::micros(): %4d   \n", x);

  x = 0;
  n = micros() + 1000;
  while (n > micros()) {
    x++;
  }
  p->printf("micros(): %4d   \n", x);
*/
}

class bpCommand : public Command {
  public:
    const char* getName() { return "bp"; }
    const char* getHelp() { return "Print britepad System Info"; }
    void execute(Console* c, uint8_t paramCount, char** params) {
      printBpInfo(c);
    }
};

bpCommand theBpCommand;
InfoApp theInfoApp;

void InfoApp::init() {
}

void InfoApp::run() {
  BritepadApp::run();
  screen.setTextColor(screen.white, bgColor());
  screen.setFont(&Arial_10);
  screen.setCursor(screen.clipLeft() + screen.fontGap(), screen.clipTop()+screen.fontGap());

  printBpInfo(&screen);

  // this info only makes sense when printed on screen
  uint8_t raw = pad.getProximityRaw();
  if (raw > max) { max = raw; sound.beep();}
  screen.printf("Points captured: %3d\n", pad.getHistoryCount());
  screen.printf("Distance (%d):%d,raw:%d,max: %d\n", pad.touched(PROXIMITY_SENSOR), pad.getProximityDistance(), raw,max);
  screen.printf("FPS: %5.2f\n", 1000.0/(Uptime::millis()-lastUpdate));
  lastUpdate = Uptime::millis();

};

