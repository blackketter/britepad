#include "MinesApp.h"
#include "Sound.h"

#include "WilliamTell.h"

bool MineMatrix::isRevealed(int x, int y) {
  return valid(x,y) && (getDot(x,y) & REVEALED_CELL) != 0;
}

bool MineMatrix::isMined(int x, int y) {
  return valid(x,y) && (getDot(x,y) & MINED_CELL) != 0;
}

bool MineMatrix::isFlagged(int x, int y) {
  return valid(x,y) && (getDot(x,y) & FLAGGED_CELL) != 0;
}

bool MineMatrix::isLoser(int x, int y) {
  return valid(x,y) && (getDot(x,y) & LOSER_CELL) != 0;
}

void MineMatrix::setRevealed(int x, int y, bool r) {
  setDot(x,y, r ?  getDot(x,y) | REVEALED_CELL :  getDot(x,y)  &~REVEALED_CELL);
}

void MineMatrix::setMined(int x, int y, bool r) {
  setDot(x,y,r ? getDot(x,y) | MINED_CELL : getDot(x,y) & ~MINED_CELL);
}

void MineMatrix::setFlagged(int x, int y, bool r) {
  setDot(x,y, r ? getDot(x,y) | FLAGGED_CELL : getDot(x,y) & ~FLAGGED_CELL);
}

void MineMatrix::setLoser(int x, int y, bool r) {
  setDot(x,y, r ? getDot(x,y) | LOSER_CELL : getDot(x,y) & ~LOSER_CELL);
}

int MineMatrix::countNeighbors(int x, int y) {
  return isMined(x-1,y-1) + isMined(x,y-1) + isMined(x+1,y-1) +
         isMined(x-1,y)   +                  isMined(x+1,y) +
         isMined(x-1,y+1) + isMined(x,y+1) + isMined(x+1,y+1);
}

int MineMatrix::countRevealed() {
  int count = 0;
  for (int x = 0; x < getDotsWide(); x++) {
    for (int y = 0; y < getDotsHigh(); y++) {
      if (isRevealed(x,y)) { count++; };
    }
  }
  return count;
}


void MineMatrix::revealNeighbors(int x, int y) {
  if (x<0 || x>=getDotsWide() || y<0 || y>=getDotsHigh()) { return; }

  if (isMined(x,y)) {
    return;
  } else {
    if (!isRevealed(x,y) && !isMined(x,y) && !isFlagged(x,y)) {
      setRevealed(x,y,true);
      if (countNeighbors(x,y)==0) {
        revealNeighbors(x-1,y-1); revealNeighbors(x,y-1); revealNeighbors(x+1,y-1);
        revealNeighbors(x-1,y);                           revealNeighbors(x+1,y);
        revealNeighbors(x-1,y+1); revealNeighbors(x,y+1); revealNeighbors(x+1,y+1);
      }
    }
  }
}

void MineMatrix::updateDot(int x, int y) {
  coord_t l,t,w,h,b,r;

  l = getLeft()+x*dotspacing_w;
  t = getTop()+y*dotspacing_h;
  w = dotspacing_w;
  h = dotspacing_h;
  b = t+h-1;
  r = l+w-1;

  bool revealed = isRevealed(x,y);
  bool flagged = isFlagged(x,y);
  bool mined = isMined(x,y);
  bool loser = isLoser(x,y);
  int neighbors = countNeighbors(x,y);

  if (!revealed) {
      screen.fillRect(l,t,w,h,screen.grey50);

      screen.drawLine(l,t,r,t,screen.grey70);
      screen.drawLine(l,t,l,b,screen.grey70);
      screen.drawLine(r,t,r,b,screen.grey30);
      screen.drawLine(l,b,r,b,screen.grey30);

      if (flagged) {
        screen.fillRect(l+w/4,t+h/4,w/2,h/2,screen.blue);
      }
  } else if (flagged && !mined) {
      screen.fillRect(l,t,w,h,screen.grey50);
      screen.drawRect(l+w/4,t+h/4,w/2,h/2,screen.blue);
  } else if (mined) {
      screen.fillRect(l,t,w,h,loser ? screen.red : screen.grey50);
      screen.fillCircle(l+w/2,t+h/2,w/4, flagged ? screen.blue : screen.black);
  } else if (neighbors > 0) {
      screen.fillRect(l,t,w,h,screen.grey50);
      screen.setCursor(l+w/3, t+h/3);
      screen.setFont(Arial_10_Bold);
      screen.setTextColor(screen.black);
      screen.drawTextF("%d",neighbors);
  } else {
      screen.fillRect(l,t,w,h,screen.grey50);
  }
}

void MinesApp::begin() {
  BritepadApp::begin();
  if (!field) {
    field = new MineMatrix(screen.clipMidWidth()-screen.clipHeight()/2,(coord_t)(screen.clipTop()),screen.clipHeight(),screen.clipHeight(),minesHeight,minesWidth);
  }

  minesLeft.init(screen.clipLeft()+2,screen.clipBottom()-20,field->getLeft()-screen.clipLeft()-4,20,Arial_12_Bold,screen.blue, screen.black, ALIGN_CENTER);
  timer.init(field->getRight()+2,screen.clipBottom()-20,screen.clipRight()-field->getRight()-4,20,Arial_12_Bold,screen.white, screen.black, ALIGN_CENTER);

  field->draw();

  startTime = 0;
  mines = minesMax;
  gameOver = false;
  youlose = false;
  firstTap = true;
  flagging = false;
}


void MinesApp::end() {
  BritepadApp::end();
  delete field;
  field = nullptr;
  sound.tunePlay(nullptr);
}

// todo: add the flag
void MinesApp::run() {
  int xhit, yhit;
  if (gameOver) {
    if (pad.up()) {
      end();
      begin();
    } else {
      return;
    }
  }

  if (pad.touched() && !flagging && pad.holdTime() > 500 && field->hit(pad.x(),pad.y(), &xhit, &yhit) && !field->isRevealed(xhit,yhit)) {
    field->setFlagged(xhit,yhit,!field->isFlagged(xhit,yhit));
    field->updateDot(xhit,yhit);
    flagging = true;
    sound.click();
    if (mines) {mines--;}
  }

  if (pad.up(BOTTOM_PAD)) {
    playTune = !playTune;
    sound.tuneVolume(playTune ? 1.0 : 0);
  }

  if (pad.up() && field->hit(pad.x(),pad.y(), &xhit, &yhit) && !flagging) {

    // lay the mines, but not close to the first tap
    if (firstTap) {
      int m = minesMax;
      while (m) {
        int x = random(minesWidth);
        int y = random(minesHeight);
        if (!field->isRevealed(x,y) && !field->isMined(x,y) && ((abs(xhit-x) > 1) || (abs(yhit-y) > 1))) {
          field->setMined(x,y,true);
          m--;
        }
      }
      firstTap = false;
      sound.tuneTranspose(12);  // move it up an octave
      sound.tunePlay(williamtell);
    }
    bool m = field->isMined(xhit,yhit);
    bool r = field->isRevealed(xhit,yhit);
    bool f = field->isFlagged(xhit,yhit);

    if (f && !r) {
        // hide the flag
        field->setFlagged(xhit,yhit, false);
        mines++;
        sound.click();
    } else if (!m && !r)  {
        // show it's empty and reveal the surroundings
        field->revealNeighbors(xhit,yhit);
        sound.click();
        if ((field->countRevealed() + minesMax) == (minesWidth*minesHeight)) {
          // you win!
          for (int x = 0; x < minesWidth; x++) {
            for (int y = 0; y < minesHeight; y++) {
              field->setRevealed(x,y,true);
            }
          }
          sound.tunePlay(nullptr);
          sound.beep(800,880);
          gameOver = true;
        }
      } else if (!r && m) {
        // you lose
        for (int x = 0; x < minesWidth; x++) {
          for (int y = 0; y < minesHeight; y++) {
            field->setRevealed(x,y,true);
          }
        }
        field->setLoser(xhit,yhit, true);
        sound.tunePlay(nullptr);
        sound.beep(800, 440);
        gameOver = true;
        youlose = true;
      }

    field->draw();
    if (startTime == 0) {
      startTime = Uptime::millis();
    }
  }
  // draw times
  int t = 0;
  if (startTime) {
    t = (Uptime::millis() - startTime) / 1000;
  }
  timer.drawf("%d", t);

  // draw mine count
  if (gameOver) {
    minesLeft.setFColor(youlose ? screen.red : screen.green);
  }
  minesLeft.drawf("%d", mines);
  if (pad.up()) {
    flagging = false;
  }
  if (mines) {
    sound.tuneTempo((1.0-((float)(mines)/minesMax))*2.0+1);  // speed up as we get to the end
  }
}

