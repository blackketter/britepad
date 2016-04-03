#include "SetCountdownApp.h"
#include "CountdownApp.h"

#include "Debug.h"

BritepadApp* SetCountdownApp::exitsTo() {
  return getAppByID(CountdownApp::ID);
}

void SetCountdownApp::begin(AppMode asMode) {

  CountdownApp* countdownApp = (CountdownApp*)getAppByID(CountdownApp::ID);
  if (countdownApp) {
    countdownTime.setSeconds(countdownApp->getTime());
  }

  setDate = true;
  SetClockApp::begin(asMode);
}

void SetCountdownApp::end() {

  CountdownApp* countdownApp = (CountdownApp*)getAppByID(CountdownApp::ID);
  if (countdownApp) {
    countdownApp->setTime(countdownTime.getSeconds());
  }

  SetClockApp::end();
}

