#include "SetCountdownApp.h"
#include "CountdownApp.h"

#include "Debug.h"

BritepadApp* SetCountdownApp::exitsTo() {
  return getAppByID(CountdownApp::ID);
}

void SetCountdownApp::begin() {

  CountdownApp* countdownApp = (CountdownApp*)getAppByID(CountdownApp::ID);
  if (countdownApp) {
    countdownTime.set(countdownApp->getTime());
  }

  setDate = true;
  SetClockApp::begin();
}

void SetCountdownApp::end() {

  CountdownApp* countdownApp = (CountdownApp*)getAppByID(CountdownApp::ID);
  if (countdownApp) {
    countdownApp->setTime(countdownTime.getSeconds());
  }

  SetClockApp::end();
}

