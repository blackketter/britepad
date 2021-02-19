#include "HexDotClockApp.h"

HexDotClockApp theHexDotClockApp;

void HexDotClockApp::begin() {
  if (dots == nullptr) {
    // approx 4:1 aspect ratio
    int w = 9 * 4 + 3;
    int h = 9 * 3;
    int dotsize = min(screen.clipWidth()/w, screen.clipHeight()/h);
    coord_t wpixels = w * dotsize;
    coord_t hpixels = h * dotsize;
    coord_t xorig = (screen.clipWidth()-wpixels)/2;
    coord_t yorig = (screen.clipHeight()-hpixels)/2;

    dots = new HexDotMatrix(screen.clipLeft()+xorig, screen.clipTop()+yorig, wpixels, hpixels, w, h);
    dots->setStaggerV(true);  // stagger vertically
  }
  ClockApp::begin();
}

void HexDotClockApp::end() {
  if (dots) {
    delete dots;
    dots = nullptr;
  }
  ClockApp::end();
}

void HexDotClockApp::update() {

    // first column is 10s of hours
    int h10 = clock.hourFormat12() / 10;
    int h1 = clock.hourFormat12() % 10;
    int m10 = clock.minute() / 10;
    int m1 = clock.minute() % 10;

    if (h10 == 0) { h10 = 10; } // supress leading digit

    drawDigit(h10,  0, 7);
    drawDigit(h1, 10, 7);
    drawDigit(m10, 20, 7);
    drawDigit(m1, 30, 7);
    dots->draw();

};

//  line arrangement
//          0
//        5 6 0
//      5   6   0
//    5     6     0
//  5 b     6     7 1
//  4   b   6   7   1
//  4     b * 7     1
//  4    a  9  8    1
//  4  a    9    8  2
//    3     9     2
//      3   9   2
//        3 9 2
//          3

bool digits[11][12] = {  // which lines are filled for a given digit
  { 1,1,1,1,1, 1,0,0,0,0, 0,0 }, // 0
  { 0,0,0,0,0, 0,1,0,0,1, 0,0 }, // 1
  { 1,0,1,0,0, 1,0,1,0,1, 0,0 }, // 2
  { 1,0,1,1,0, 1,0,1,1,0, 0,0 }, // 3
  { 0,0,0,0,1, 0,1,1,0,1, 1,0 }, // 4
  { 1,0,1,1,0, 0,1,0,1,0, 0,0 }, // 5
  { 1,0,1,1,1, 1,0,0,1,0, 1,0 }, // 6
  { 1,0,0,0,0, 1,0,1,0,1, 0,0 }, // 7
  { 1,0,1,1,0, 1,0,1,1,0, 1,1 }, // 8
  { 1,1,1,1,0, 1,0,1,0,0, 0,1 }, // 9
  { 0,0,0,0,0, 0,0,0,0,0, 0,0 }, // none of the above
//  0 1 2 3 4  5 6 7 8 9  a b  - line numbers
};

//  dots are encoded as 2 digits, first digit is y, second digit is x, for each lines
//  rows are arranged:
//  0     0     0     0   ...
//     0     0     0     0 ...
//  1     1     1     1   ...
//     1     1     1     1 ...

//  columns:
//  0     2     4     6
//     1     3     5     7
//  0     2     4     6
//     1     3     5   7

uint8_t lines[12][5] = { // which dots are filled for a given line
  { 04, 05, 16, 17, 28 }, // line 0
  { 28, 38, 48, 58, 68 }, // line 1
  { 84, 75, 76, 67, 68 }, // line 2
  { 60, 61, 72, 73, 84 }, // line 3
  { 20, 30, 40, 50, 60 }, // line 4
  { 20, 11, 12, 03, 04 }, // line 5
  { 44, 34, 24, 14, 04 }, // line 6
  { 44, 35, 36, 27, 28 }, // line 7
  { 44, 45, 56, 57, 68 }, // line 8
  { 44, 54, 64, 74, 84 }, // line 9
  { 60, 51, 52, 43, 44 }, // line 10
  { 20, 21, 32, 33, 44 }, // line 11
};

void HexDotClockApp::drawDigit(uint8_t digit, uint8_t xoff, uint8_t yoff) {
  color_t fc = screen.red;
  color_t bc = screen.grey10;
  // since lines overlap at the ends, clear out the off lines, then draw the on lines
  for (uint8_t line = 0; line < 12; line++) {
    for (uint8_t dot = 0; dot < 5; dot++) {
       if (digits[digit][line] == 0) {
         uint8_t x = lines[line][dot] % 10 + xoff;
         uint8_t y = lines[line][dot] / 10 + yoff;
         dots->setDot(x,y,bc);
       }
    }
  }
  for (uint8_t line = 0; line < 12; line++) {
    for (uint8_t dot = 0; dot < 5; dot++) {
       if (digits[digit][line]) {
         uint8_t x = lines[line][dot] % 10 + xoff;
         uint8_t y = lines[line][dot] / 10 + yoff;
         dots->setDot(x,y,fc);
       }
    }
  }
};

