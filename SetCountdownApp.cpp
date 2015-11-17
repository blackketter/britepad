#include "SetCountdownApp.h"
#include "CountdownApp.h"

#include "Debug.h"

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
    countdownApp->setTime(countdownTime.get());
  }

  SetClockApp::end();
}

