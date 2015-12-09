#include "SeqApp.h"


void SeqApp::run() {

  const color_t highlight = screen.red;
  const color_t noteOn = screen.blue;
  ScreensaverApp::run();

  if (pad.down(SCREEN_PAD)) {
    int x, y;
    if (dots->hit(pad.x(), pad.y(), &x, &y)) {
      dots->setDot(x,y, dots->getDot(x,y) ^ noteOn);
      dots->updateDot(x,y);
    }
  }

  if (pad.down(BOTTOM_PAD)) {
    tick = !tick;
  }

  if (pad.down(LEFT_PAD)) {
    bpm -= bpm/25;
    if (bpm < minBpm) {
      bpm = minBpm;
    }
  }

  if (pad.down(RIGHT_PAD)) {
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

    if (pad.touched(LEFT_PAD) && !pad.down(LEFT_PAD)) {
      bpm -= bpm/10;
      if (bpm < minBpm) {
        bpm = minBpm;
      }
    }

    if (pad.touched(RIGHT_PAD) && !pad.down(RIGHT_PAD)) {
      bpm += bpm/10;
      if (bpm > maxBpm) {
        bpm = maxBpm;
      }
    }

  }
}


