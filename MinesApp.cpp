#include "MinesApp.h"
#include "Sound.h"
#include <usb_keyboard.h>

enum {
  EMPTY_MINE,      // blank square with no mine underneath
  HIDDEN_MINE,      // blank square with mine underneath
  FLAG_EMPTY_MINE,  // show flag on a blank square
  FLAG_HIDDEN_MINE,        // show flag on a blank square with a mine

  SHOW_EMPTY_MINE,  // reveal an empty mine
  N1_MINE,         // reveal an empty mine and how many mines are nearby
  N2_MINE,
  N3_MINE,
  N4_MINE,
  N5_MINE,
  N6_MINE,
  N7_MINE,
  N8_MINE,

  SHOW_MINE,        // reveal the mine
  RED_MINE,         // reveal the mine on the losing square
};

bool MineMatrix::revealed(int x, int y) {
  bool reved = true;
  switch (getDot(x,y)) {
    case EMPTY_MINE:
    case HIDDEN_MINE:
    case FLAG_EMPTY_MINE:
    case FLAG_HIDDEN_MINE:
      reved = false;
      break;
    default:
      break;
  }

  return reved;
}

int MineMatrix::revealed() {
  int count = 0;
  for (int x = 0; x < getDotsWide(); x++) {
    for (int y = 0; y < getDotsHigh(); y++) {
      if (revealed(x,y)) { count++; };
    }
  }
  return count;
}


int MineMatrix::neighbors(int x, int y) {
  return mined(x-1,y-1) + mined(x,y-1) + mined(x+1,y-1) +
         mined(x-1,y)   +                mined(x+1,y) +
         mined(x-1,y+1) + mined(x,y+1) + mined(x+1,y+1);
}

void MineMatrix::reveal(int x, int y) {
  if (x<0 || x>=getDotsWide() || y<0 || y>=getDotsHigh()) { return; }

  if (mined(x,y)) {
    return;
  } else {
    if (getDot(x,y) == EMPTY_MINE) {
      int n = neighbors(x,y);
      setDot(x,y,SHOW_EMPTY_MINE+n);
      if (n==0) {
        reveal(x-1,y-1); reveal(x,y-1); reveal(x+1,y-1);
        reveal(x-1,y);                  reveal(x+1,y);
        reveal(x-1,y+1); reveal(x,y+1); reveal(x+1,y+1);
      }
    }
  }
}

int MineMatrix::mined(int x, int y) {
  if (x<0 || x>=getDotsWide() || y<0 || y>=getDotsHigh()) { return false; }

  int minethere = 0;

  switch (getDot(x,y)) {
    case SHOW_MINE:
    case RED_MINE:
    case HIDDEN_MINE:
    case FLAG_HIDDEN_MINE:
      minethere = 1;
      break;
    default:
      break;
  }
  return minethere;
}


void MineMatrix::updateDot(int x, int y) {
  coord_t l,t,w,h,b,r;

  l = getLeft()+x*dotspacing_w;
  t = getTop()+y*dotspacing_h;
  w = dotspacing_w;
  h = dotspacing_h;
  b = t+h-1;
  r = l+w-1;
//    screen.fillRect(,getTop()+y*dotspacing_h,dotspacing_w,dotspacing_h,getDot(x,y));
  color_t contents = getDot(x,y);
  switch (contents) {
    case EMPTY_MINE:
    case HIDDEN_MINE:
    case FLAG_HIDDEN_MINE:
    case FLAG_EMPTY_MINE:
      screen.fillRect(l,t,w,h,screen.grey50);

      screen.drawLine(l,t,r,t,screen.grey70);
      screen.drawLine(l,t,l,b,screen.grey70);
      screen.drawLine(r,t,r,b,screen.grey30);
      screen.drawLine(l,b,r,b,screen.grey30);

      if (contents == FLAG_HIDDEN_MINE || contents == FLAG_EMPTY_MINE) {
        screen.fillRect(l+w/4,t+h/4,w/2,h/2,screen.blue);
      }
      break;

    case SHOW_EMPTY_MINE:
      screen.fillRect(l,t,w,h,screen.grey50);
      break;
    case N1_MINE:
    case N2_MINE:
    case N3_MINE:
    case N4_MINE:
    case N5_MINE:
    case N6_MINE:
    case N7_MINE:
    case N8_MINE:
      screen.fillRect(l,t,w,h,screen.grey50);
      screen.setCursor(l+w/3, t+h/3);
      screen.setFont(Arial_10_Bold);
      screen.setTextColor(screen.black);
      screen.drawTextF("%d",contents - N1_MINE + 1);
      break;

    case SHOW_MINE:
      screen.fillRect(l,t,w,h,screen.grey50);
      screen.fillCircle(l+w/2,t+h/2,w/4, screen.black);
      break;
    case RED_MINE:
      screen.fillRect(l,t,w,h,screen.red);
      screen.fillCircle(l+w/2,t+h/2,w/4, screen.black);
     break;

    default:
      DEBUG_LN("Unknown state in mine matrix");
      break;
  }
}

void MinesApp::begin() {
  BritepadApp::begin();
  if (!field) {
    field = new MineMatrix(screen.clipMidWidth()-screen.clipHeight()/2,(coord_t)(screen.clipTop()),screen.clipHeight(),screen.clipHeight(),minesHeight,minesWidth);
  }

  minesLeft.init(screen.clipLeft()+2,screen.clipBottom()-20,field->getLeft()-screen.clipLeft()-4,20,Arial_12_Bold,screen.blue, screen.black, ALIGN_CENTER);
  timer.init(field->getRight()+2,screen.clipBottom()-20,screen.clipRight()-field->getRight()-4,20,Arial_12_Bold,screen.white, screen.black, ALIGN_CENTER);

  mines = minesMax;
  while (mines) {
    int x = random(minesWidth);
    int y = random(minesHeight);
    if (field->getDot(x,y) == EMPTY_MINE) {
      field->setDot(x,y, HIDDEN_MINE);
      mines--;
    }
  }

  field->draw();

  startTime = 0;
  mines = minesMax;
  gameOver = false;
  youlose = false;
  firstTap = true;
  flagged = false;
}


void MinesApp::end() {
  BritepadApp::end();
  delete field;
  field = nullptr;
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

  if (pad.touched() && !flagged && pad.holdTime() > 500 && field->hit(pad.x(),pad.y(), &xhit, &yhit)) {
    color_t contents = field->getDot(xhit,yhit);
    if (contents == EMPTY_MINE) {
      field->setDot(xhit,yhit, FLAG_EMPTY_MINE);
      field->updateDot(xhit,yhit);
      flagged = true;
      sound.click();
      mines--;
    }
    if (contents == HIDDEN_MINE) {
      field->setDot(xhit,yhit, FLAG_HIDDEN_MINE);
      field->updateDot(xhit,yhit);
      flagged = true;
      sound.click();
      mines--;
    }
  }

  if (pad.up() && field->hit(pad.x(),pad.y(), &xhit, &yhit) && !flagged) {
    color_t contents = field->getDot(xhit,yhit);
    switch (contents) {
      case EMPTY_MINE:
        // show it's empty and reveal the surroundings
        field->reveal(xhit,yhit);
        sound.click();
        if ((field->revealed() + minesMax) == (minesWidth*minesHeight)) {
          // you win!
          for (int x = 0; x < minesWidth; x++) {
            for (int y = 0; y < minesHeight; y++) {
              if (field->getDot(x,y) == HIDDEN_MINE) {
                field->setDot(x,y,SHOW_MINE);
              }
            }
          }
          sound.beep(800,440);
          gameOver = true;
        }
        break;

      case FLAG_HIDDEN_MINE:
      case FLAG_EMPTY_MINE:
        // hide the flag
        if (contents == FLAG_HIDDEN_MINE) {
           field->setDot(xhit,yhit, HIDDEN_MINE);
        } else {
          field->setDot(xhit,yhit, EMPTY_MINE);
        }
        mines++;
        sound.click();
        break;

      case HIDDEN_MINE:
        // you lose
        for (int x = 0; x < minesWidth; x++) {
          for (int y = 0; y < minesHeight; y++) {
            if (field->getDot(x,y) == HIDDEN_MINE) {
              field->setDot(x,y,SHOW_MINE);
            }
          }
        }
        field->setDot(xhit,yhit, RED_MINE);
        sound.beep(800, 220);
        gameOver = true;
        youlose = true;
        break;

      default:
        // just ignore
        break;

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
    flagged = false;
  }
}

