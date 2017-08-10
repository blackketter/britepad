#include "ClockApp.h"

class BinaryClockApp : public ClockApp {
  public:
    void update();

    const char* name() { return "Binary"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "binc";

  protected:
    millis_t lastRedraw = 0;
    color_t currentColor = screen.red;
    coord_t lastWidth = 0;
    static const millis_t redrawInterval = 1000;
};

BinaryClockApp theBinaryClockApp;

void BinaryClockApp::update() {
    coord_t rowspace = screen.clipHeight()/9*2;
    coord_t colspace = screen.clipWidth()/15*2;
    coord_t r = min(rowspace,colspace)/4;

    coord_t ystart = rowspace*3/4;
    coord_t xstart = colspace*3/4;

    coord_t x = xstart;
    coord_t y = ystart;

    color_t c = currentColor++;
    color_t d = screen.darken(c);

    // first column is 10s of hours
    int h = clock.hourFormat12();
    int m = clock.minute();
    int s = clock.second();

    screen.fillCircle(x,y,r,(h/10)&8 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(h/10)&4 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(h/10)&2 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(h/10)&1 ? c :d);
    y+= rowspace;

    y = ystart;
    x += colspace;

    screen.fillCircle(x,y,r,(h%10)&8 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(h%10)&4 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(h%10)&2 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(h%10)&1 ? c :d);
    y+= rowspace;

    y = ystart;
    x += colspace*3/2;

    screen.fillCircle(x,y,r,(m/10)&8 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(m/10)&4 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(m/10)&2 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(m/10)&1 ? c :d);
    y+= rowspace;

    y = ystart;
    x += colspace;

    screen.fillCircle(x,y,r,(m%10)&8 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(m%10)&4 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(m%10)&2 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(m%10)&1 ? c :d);
    y+= rowspace;

    y = ystart;
    x += colspace*3/2;

    screen.fillCircle(x,y,r,(s/10)&8 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(s/10)&4 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(s/10)&2 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(s/10)&1 ? c :d);
    y+= rowspace;

    y = ystart;
    x += colspace;

    screen.fillCircle(x,y,r,(s%10)&8 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(s%10)&4 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(s%10)&2 ? c :d);
    y+= rowspace;
    screen.fillCircle(x,y,r,(s%10)&1 ? c :d);
    y+= rowspace;
}
