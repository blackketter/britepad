#include "ChimeApp.h"
ChimeApp theChimeApp;


void chimeCallback(void* data) {
  ((ChimeApp*)data)->chimerCallback();
}

void ChimeApp::resetChime() {
  if (clock.hasBeenSet() && _chimeEnabled) {

    time_t nextChime = ((clock.now() / Time::secsPerHour)+1) * Time::secsPerHour;
    _chimeTimer.setClockTime( nextChime, chimeCallback, (void*)this);

    // how many chimes at the next hour
    _chimesRemaining = clock.hourFormat12() + 1;
    if (_chimesRemaining == 13) { _chimesRemaining = 1; }
    console.debugf("%d chimes set up\n", _chimesRemaining);
  } else {
    console.debugln("chime not set up");
  }
}


void ChimeApp::chimerCallback() {
  console.debugf("Chimes: %d\n", _chimesRemaining);
  if (_chimesRemaining == 0) {
    resetChime();
  } else {
    _chimeTimer.setMillis(_chimeInterval, chimeCallback, (void*)this);
    sound.beep();
    _chimesRemaining--;
  }
}
