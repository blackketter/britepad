#include "SeqApp.h"

SeqApp theSeqApp;

const float freq[] = {
      587.330,
      523.251,
      493.883,
      440.000,
      391.995,
      349.228,
      329.628,
      293.665,
      261.626,
};

void SeqApp::run() {

  const color_t highlight = screen.red;
  const color_t noteOn = screen.blue;
  ScreensaverApp::run();

  KeyEvent* key = getNextEvent();

  if (pad.pressed(SCREEN_PAD)) {
    int x, y;
    if (dots->hit(pad.x(), pad.y(), &x, &y)) {
      dots->setDot(x,y, dots->getDot(x,y) ^ noteOn);
      dots->updateDot(x,y);
    }
  }

  if (pad.pressed(BOTTOM_PAD) || (key && key->pressed(KEY_SPACE))) {
    tick = !tick;
  }

  if (pad.pressed(LEFT_PAD) || (key && key->pressed(KEY_DOWN))) {
    bpm -= bpm/25;
    if (bpm < minBpm) {
      bpm = minBpm;
    }
  }

  if (pad.pressed(RIGHT_PAD) || (key && key->pressed(KEY_UP))) {
    bpm += bpm/25;
    if (bpm > maxBpm) {
      bpm = maxBpm;
    }
  }

  if (pad.time() - lastRun >= (60000/bpm)) {
    if (tick) {
      sound.click();
    }

    lastRun = pad.time();

    int lastCol = currCol-1;

    if (lastCol < 0) {
      lastCol = getDotsWide()-1;
    }

    for (int y = 0; y< getDotsHigh(); y++) {
      if (dots->getDot(currCol,y) & noteOn) {
        // play note associated with y
          sound.beep(40, freq[y]);
      }
    }
    for (int y = 0; y<getDotsHigh(); y++) {
      dots->setDot(lastCol,y, (~highlight) & dots->getDot(lastCol,y));
      dots->updateDot(lastCol,y);

      dots->setDot(currCol,y, highlight | dots->getDot(currCol,y));
      dots->updateDot(currCol,y);
    }

    currCol++;
    if (currCol == getDotsWide()) {
      currCol = 0;
    }

    if (pad.touched(LEFT_PAD) && !pad.pressed(LEFT_PAD)) {
      bpm -= bpm/10;
      if (bpm < minBpm) {
        bpm = minBpm;
      }
    }

    if (pad.touched(RIGHT_PAD) && !pad.pressed(RIGHT_PAD)) {
      bpm += bpm/10;
      if (bpm > maxBpm) {
        bpm = maxBpm;
      }
    }

  }
}


