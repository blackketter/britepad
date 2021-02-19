#include "BritepadApp.h"
#include "BritepadShared.h"
#include "Screen.h"
#include "Sound.h"

class EyesApp : public BritepadApp {
  public:
    void begin();
    void run();

    bool canBeMouse() { return true; }
    bool canBeInteractive() { return true; }
    bool canBeScreensaver() { return true; }
    bool disablesScreensavers() { return getAppMode() == INTERACTIVE_MODE; }

    AppType getAppType() { return (AppType)(MOUSE_APP + INTERACTIVE_APP + SCREENSAVER_APP); }

    const char* name() { return "Eyes"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "eyes";

  private:
    void calcPupil(int eye, point_t& target, point_t& pupil);
    static const int numEyes = 2;
    point_t center[numEyes];
    coord_t radius[numEyes];
    coord_t pupilRadius[numEyes];
    point_t lastTarget;
};

EyesApp theEyesApp;

void EyesApp::begin() {
  BritepadApp::begin();
  coord_t x = screen.clipMidWidth();
  coord_t y = screen.clipMidHeight();
  coord_t r = min(x/2,y/2) - 5;

  center[0].x = (x-screen.clipLeft())/2;
  center[0].y = y;
  radius[0] = r;
  pupilRadius[0] = r/3;

  center[1].x = ((x-screen.clipLeft())*3)/2;
  center[1].y = y;
  radius[1] = r;
  pupilRadius[1] = r/3;

  for (int i = 0; i < numEyes; i++) {
    screen.fillCircle(center[i].x,center[i].y,radius[i],screen.white);

    point_t p;
    calcPupil(i, lastTarget, p);
    screen.fillCircle(p.x,p.y,pupilRadius[i],screen.black);
  }
}
void EyesApp::calcPupil(int eye, point_t& target, point_t& pupil) {

  coord_t r = radius[eye]-(pupilRadius[eye]*3/2);

  coord_t d = sqrt( (center[eye].x-target.x)*(center[eye].x-target.x) + (center[eye].y-target.y)*(center[eye].y-target.y) );

  if (d < r) {
    pupil = target;
  } else {
    pupil.x = (target.x-center[eye].x)*r/d + center[eye].x;
    pupil.y = (target.y-center[eye].y)*r/d + center[eye].y;
  }
}

void EyesApp::run() {
  BritepadApp::run();
  point_t currTarget = lastTarget;

  if (pad.touched(SCREEN_PAD)) {
    currTarget.x = pad.x();
    currTarget.y = pad.y();
  } else if ((Uptime::millis() - usbMouse.lastMove()) < 500) {
    currTarget.x = usbMouse.x();
    currTarget.y = usbMouse.y();
  }

  if (currTarget.x != lastTarget.x || currTarget.y != lastTarget.y) {
    for (int i = 0; i < numEyes; i++) {
      point_t pupil;
      calcPupil(i, lastTarget, pupil);
      screen.fillCircle(pupil.x,pupil.y,pupilRadius[i],screen.white);
      calcPupil(i, currTarget, pupil);
      screen.fillCircle(pupil.x,pupil.y,pupilRadius[i],screen.black);
    }
  }
  lastTarget = currTarget;
}

