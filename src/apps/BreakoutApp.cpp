#include "BritepadShared.h"
#include "ScreensaverApp.h"

class BreakoutApp : public BritepadApp {
  public:
    const char* name() { return "Breakout"; };
    bool disablesScreensavers() { return dx || dy || (pad.time() - pad.lastTouchedTime(ANY_PAD)) > timeoutMillis; }
    void run();
    void begin();

    AppType getAppType() { return INTERACTIVE_APP; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "bkou";

  private:
    void newGame();
    bool hit(coord_t x, coord_t y);

    coord_t ballx, bally;
    coord_t dx, dy;

    coord_t brickw;
    coord_t brickh;
    coord_t topbrickh;
    coord_t brickx(int col);
    coord_t bricky(int row);

    coord_t ballr;

    coord_t paddlew;
    coord_t paddleh;
    coord_t paddlex;

    static const millis_t timeoutMillis = 10000;
    static const millis_t millisPerFrame = 16;
    static const int defaultdx = 3;
    static const int defaultdy = 3;
    millis_t lastDraw = 0;

    static const int rows = 6;
    static const int cols = 16;

    bool bricks[rows][cols];
    int bricksleft;

};

BreakoutApp theBreakoutApp;

// todo - show score, have multiple lives
// todo - speed up over time
// todo - make it run as a self-playing screensaver

void BreakoutApp::begin() {
  BritepadApp::begin();
  newGame();
  dx = dy = 0;  // start off without the ball moving
}

coord_t BreakoutApp::brickx(int col) {
  return screen.clipLeft()+brickw*col;
}
coord_t BreakoutApp::bricky(int row) {
  return screen.clipTop()+topbrickh+brickh*row;
}

bool BreakoutApp::hit(coord_t x, coord_t y) {
  int col = (x - screen.clipLeft())/brickw;
  int row = (y - screen.clipTop() - topbrickh)/brickh;
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

void BreakoutApp::newGame() {

  clearScreen();

  brickw = screen.clipWidth()/cols;
  brickh = screen.clipHeight()/16;
  topbrickh = brickh*2;

  ballr = brickh/2;

  paddlew = screen.clipWidth()/5;
  paddleh = brickh;

  bricksleft = rows*cols;

  ballx = screen.clipMidWidth() + random(10);
  bally = screen.clipMidHeight() + ballr*2 + random(10);

  dx = defaultdy;
  dy = defaultdx;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      bricks[i][j]=true;
      screen.fillRect(brickx(j), bricky(i), brickw, brickh, random());
    }
  }
}

void BreakoutApp::run() {
  millis_t millisNow = Uptime::millis();
  if (lastDraw/millisPerFrame == millisNow/millisPerFrame && !pad.pressed(ANY_PAD)) {
    return;
  }

   if (dx == 0 && dy == 0) {
    // game over, man
      if (pad.pressed(ANY_PAD)  || usbMouse.buttons()) {
        newGame();
      } else {
        return;
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
  if (ballx > screen.clipRight()) {
    dx = -dx;
    ballx += 2*dx;
    sound.beep();
  } else if (ballx < screen.clipLeft()) {
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
  if (bally > screen.clipBottom()-paddleh-ballr-dy && dy > 0) {

   if (abs(paddlex+paddlew/2-ballx) <= ballr) {
      // bounce straight up if we hit the middle
      dy = -dy;
      dx = 0;
      bally += 2*dy;
      sound.beep(20,550);
    } else if (ballx > paddlex && ballx < paddlex+paddlew ) {
      // bounce off paddle
      if (dx==0) {
        dx = paddlex+paddlew/2-ballx < 0 ? defaultdx : -defaultdx;
      }
      dy = -dy;
      bally += 2*dy;
      sound.beep(20,330);
    } else if (ballx > paddlex+paddlew && ballx < paddlex+paddlew + ballr*2) {
      // always bounce right if we hit the right edge
      if (!dx) { dx = defaultdx; }
      dx = abs(dx);
      dy = -dy;
      bally += 2*dy;
      sound.beep(20,440);
    } else if (ballx < paddlex && ballx > paddlex-ballr*2) {
      // always bounce left if we hit the left edge
      if (!dx) { dx = defaultdx; }
      dx = -abs(dx);
      dy = -dy;
      bally += 2*dy;
      sound.beep(20,440);
    } else {
      // we hit bottom!

      // stop
      dy = dx = 0;

      sound.beep(800, 220);
    }
  } else if (bally < screen.clipTop()) {
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
  } else {
    newpaddlex = usbMouse.x();
  }

  if (pad.touched(LEFT_PAD)) {
    newpaddlex-= 2*abs(dx);
  }
  if (pad.touched(RIGHT_PAD)) {
    newpaddlex+= 2*abs(dx);
  }

  newpaddlex = max(0, newpaddlex);
  newpaddlex = min(screen.clipWidth(), newpaddlex);

  if (newpaddlex != paddlex) {
    coord_t drawx, erasex, draww;
    if (abs(newpaddlex-paddlex)<paddlew && !pad.pressed(SCREEN_PAD)) {
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

    screen.fillRect(erasex, screen.clipBottom()-paddleh, draww, paddleh, bgColor());
    screen.fillRect(drawx, screen.clipBottom()-paddleh, draww, paddleh, screen.blue);

    paddlex=newpaddlex;
  }

  return;
}


