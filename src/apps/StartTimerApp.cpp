#include "StartTimerApp.h"

StartTimerApp timer10sec("10 sec", 10,0);
StartTimerApp timer3min("3 min", 3*60,1);
StartTimerApp timer25min("25 min", 25*60,2);
StartTimerApp timer55min("55 min", 55*60,3);
StartCustomTimerApp customTimerApp(9);

////////////////////////////////////////////////////////////////////////////////
// StartTimerApp

StartTimerApp::StartTimerApp(const char* name, time_t duration, int32_t pos) : BritepadApp() {
  _timerDuration = duration;
  _nameStr = name;
  setLauncherPosition(pos);
}

StartTimerApp::StartTimerApp() : BritepadApp() {};

void StartTimerApp::run() {
  if (theTimerApp.getTime() != getDuration() || theTimerApp.isReset()) {
    theTimerApp.setTime(getDuration());
  }
  launchApp(&theTimerApp);
};

////////////////////////////////////////////////////////////////////////////////
// StartCustomTimerApp
StartCustomTimerApp::StartCustomTimerApp(int32_t pos) : StartTimerApp() {
  setLauncherPosition(pos);
};