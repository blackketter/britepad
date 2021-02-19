#include "SetCountdownApp.h"
#include "CountdownApp.h"


BritepadApp* SetCountdownApp::exitsTo() {
  return (BritepadApp*)BritepadApp::getAppByID(CountdownApp::ID);
}

void SetCountdownApp::begin() {

  CountdownApp* countdownApp = (CountdownApp*)BritepadApp::getAppByID(CountdownApp::ID);
  if (countdownApp) {
    countdownTime.setSeconds(countdownApp->getTime());
  }

  setDate = true;
  SetClockApp::begin();
}

void SetCountdownApp::end() {

  CountdownApp* countdownApp = (CountdownApp*)BritepadApp::getAppByID(CountdownApp::ID);
  if (countdownApp) {
    countdownApp->setTime(countdownTime.getSeconds());
  }

  SetClockApp::end();
}

