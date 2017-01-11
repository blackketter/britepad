#include "InfoApp.h"

void printInfo(Print* p) {
  p->println("SYSTEM INFO");
  char string[100];
  clock.longTime(string);
  p->printf("Time: %s %06d\nX: %3d Y: %3d\n", string, clock.fracMicros(), pad.x(), pad.y());
  p->printf("Ambient: %3d\n", pad.getAmbientLight());
  p->printf("Backlight: %3d\n", screen.getBacklight());
  p->printf("Free ram: %10d\n", FreeMem());
  p->printf("Uptime: %f\n", Uptime::micros()/1000000.0);
  
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

class InfoCommand : public Command {
  public:
    const char* getName() { return "info"; }
    const char* getHelp() { return "Print System Info"; }
    void execute(Stream* c, uint8_t paramCount, char** params) { 
      printInfo(c);
    }
};

InfoCommand theInfoCommand;
InfoApp theInfoApp;

void InfoApp::init() {
}

void InfoApp::run() {
  BritepadApp::run();
  screen.setTextColor(screen.white, bgColor());
  screen.setFont(Arial_12);
  screen.setCursor(screen.clipLeft() + screen.fontGap(), screen.clipTop()+screen.fontGap());
  
  printInfo(&screen);
  
  // this info only makes sense when printed on screen
  uint8_t raw = pad.getProximityRaw();
  if (raw > max) { max = raw; sound.beep();}
  screen.printf("Points captured: %3d\n", pad.getHistoryCount());
  screen.printf("Distance:%d,raw:%d,max: %d\n", pad.getProximityDistance(), raw,max);
  screen.printf("FPS: %5.2f\n", 1000.0/(pad.time()-lastUpdate));
  lastUpdate = pad.time();

};
