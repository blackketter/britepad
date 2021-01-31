#include "SetCountdownApp.h"
#include "CountdownApp.h"


App* SetCountdownApp::exitsTo() {
  return BritepadApp::getAppByID(CountdownApp::ID);
}

void SetCountdownApp::begin(AppMode asMode) {

  CountdownApp* countdownApp = (CountdownApp*)BritepadApp::getAppByID(CountdownApp::ID);
  if (countdownApp) {
    countdownTime.setSeconds(countdownApp->getTime());
  }

  setDate = true;
  SetClockApp::begin(asMode);
}

void SetCountdownApp::end() {

  CountdownApp* countdownApp = (CountdownApp*)BritepadApp::getAppByID(CountdownApp::ID);
  if (countdownApp) {
    countdownApp->setTime(countdownTime.getSeconds());
  }

  SetClockApp::end();
}

