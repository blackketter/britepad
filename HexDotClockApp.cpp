#include "HexDotClockApp.h"

void HexDotClockApp::begin() {
  if (dots == nullptr) {
    dots = new HexDotMatrix(screen.clipLeft(), screen.clipMidHeight()-screen.clipWidth()/8, screen.clipWidth(), screen.clipWidth()/4, 9 * 4 + 3, 9); // approx 4:1 aspect ratio
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

    drawDigit(h10, 0);
    drawDigit(h1, 1);
    drawDigit(m10, 2);
    drawDigit(m1, 3);
    dots->draw();

};
//  line arrangement
//      0
//     560
//    5 6 0
//   5  6  0
//  5b  6  71
//  4 b 6 7 1
//  4  a 7  1
//  4 a   8 1
//  4a  9  82
//   3  9  2
//    3 9 2
//     392
//      3

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

void HexDotClockApp::drawDigit(uint8_t digit, uint8_t position) {
  color_t fc = screen.red;
  color_t bc = screen.grey20;
  // since lines overlap at the ends, clear out the off lines, then draw the on lines
  for (uint8_t line = 0; line < 12; line++) {
    for (uint8_t dot = 0; dot < 5; dot++) {
       if (digits[digit][line] == 0) {
         uint8_t x = lines[line][dot] % 10 + position*10;
         uint8_t y = lines[line][dot] / 10;
         dots->setDot(x,y,bc);
       }
    }
  }
  for (uint8_t line = 0; line < 12; line++) {
    for (uint8_t dot = 0; dot < 5; dot++) {
       if (digits[digit][line]) {
         uint8_t x = lines[line][dot] % 10 + position*10;
         uint8_t y = lines[line][dot] / 10;
         dots->setDot(x,y,fc);
       }
    }
  }
};

