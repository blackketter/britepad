#include "BritepadShared.h"
#include "BreakoutApp.h"

#define DEBUG_ON 1
#include "Debug.h"

// todo - fix flicker
// todo - show score, have multiple lives
// todo - adjust angle based on where on the paddle
// todo - speed up over time
// todo - make it run as a self-playing screensaver

void BreakoutApp::begin(bool asScreensaver) {
  BritepadApp::begin(asScreensaver);
  newGame();
  dx = dy = 0;  // start off without the ball moving
}

coord_t BreakoutApp::brickx(int col) {
  return left()+brickw*col;
}
coord_t BreakoutApp::bricky(int row) {
  return top()+topbrickh+brickh*row;
}

bool BreakoutApp::hit(coord_t x, coord_t y) {
  int col = (x - left())/brickw;
  int row = (y - top() - topbrickh)/brickh;
  if (row >= 0 && row < rows &&
      col >= 0 && col < cols &&
      bricks[row][col]) {
    bricks[row][col] = false;
    bricksleft--;
    screen.fillRect(brickx(col), bricky(row), brickw, brickh, bgColor());
    return true;
  } else {
    return false;
  }
}

void BreakoutApp::newGame(void) {

  clearScreen();

  brickw = width()/cols;
  brickh = height()/16;
  topbrickh = brickh*2;

  ballr = brickh/2;

  paddlew = width()/5;
  paddleh = brickh;

  bricksleft = rows*cols;

  ballx = width()/2 + random(10);
  bally = top() + height()/2 + ballr*2 + random(10);

  dx = 3;
  dy = 3;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      bricks[i][j]=true;
      screen.fillRect(brickx(j), bricky(i), brickw, brickh, random());
    }
  }
}

BritepadApp* BreakoutApp::run(void) {
  millis_t millisNow = clock.millis();
  if (lastDraw/millisPerFrame == millisNow/millisPerFrame && !pad.down(ANY_PAD)) {
    return STAY_IN_APP;
  }

   if (dx == 0 && dy == 0) {
    // game over, man
      if (pad.down(ANY_PAD)) {
        newGame();
      } else {
        return STAY_IN_APP;
      }
   }

  if (bricksleft <= 0) {
    newGame();
  }

  lastDraw = millisNow;

  // erase old circle
  screen.fillCircle(ballx, bally, ballr, bgColor());

  // check if we hit the sides
  ballx += dx;
  if (ballx > right()) {
    dx = -dx;
    ballx += 2*dx;
    sound.beep();
  } else if (ballx < left()) {
    dx = -dx;
    ballx += 2*dx;
    sound.beep();
  }

  // check for hitting bricks
  coord_t xtest = dx < 0 ? ballx - ballr : ballx + ballr;

  if (hit(xtest, bally+ballr) || hit(xtest, bally-ballr) ) {
    dx = -dx;
    ballx += 2*dx;
    sound.beep(20,880);
  }

  bally += dy;

  coord_t ytest = dy < 0 ? bally - ballr : bally + ballr;

  if (hit(ballx+ballr, ytest) || hit(ballx-ballr,ytest)) {
    dy = -dy;
    bally += 2*dy;
    sound.beep(20,880);
  }

  // check if we hit the paddle, bottom or top
  if (bally > bottom()-paddleh-ballr-dy && dy > 0) {
    if (ballx > paddlex && ballx < paddlex+paddlew ) {
      // bounce off paddle
      dy = -dy;
      bally += 2*dy;
      sound.beep(20,220);
    } else {
      // we hit bottom!

      // stop
      dy = dx = 0;

      sound.beep(100, 220);
    }
  } else if (bally < top()) {
    dy = -dy;
    bally += 2*dy;
    sound.beep();
  }

  // draw the ball
  screen.fillCircle(ballx, bally, ballr, screen.red);

  coord_t newpaddlex = paddlex;

  // update the paddle
  if (pad.touched(SCREEN_PAD)) {
    newpaddlex = pad.x() - paddlew/2; // touch is in the middle of the paddle
  }

  if (pad.touched(LEFT_PAD)) {
    newpaddlex-= 2*abs(dx);
  }
  if (pad.touched(RIGHT_PAD)) {
    newpaddlex+= 2*abs(dx);
  }

  newpaddlex = max(0, newpaddlex);
  newpaddlex = min(screen.width(), newpaddlex);

  if (newpaddlex != paddlex) {
    coord_t drawx, erasex, draww;
    if (abs(newpaddlex-paddlex)<paddlew && !pad.down(SCREEN_PAD)) {
      if (newpaddlex < paddlex) {
        erasex = newpaddlex+paddlew;
        drawx = newpaddlex;
        draww = paddlex-newpaddlex;
      } else {
        erasex = paddlex;
        drawx = paddlex+paddlew;
        draww = newpaddlex-paddlex;
      }
    } else {
      drawx = newpaddlex;
      erasex = paddlex;
      draww = paddlew;
    }

    screen.fillRect(erasex, bottom()-paddleh, draww, paddleh, bgColor());
    screen.fillRect(drawx, bottom()-paddleh, draww, paddleh, screen.blue);

    paddlex=newpaddlex;
  }

  return STAY_IN_APP;
}

void BreakoutApp::end(BritepadApp* nextApp) {
  BritepadApp::end(nextApp);
}


