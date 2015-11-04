#include "CalculatorApp.h"


enum keys {
  zero,
  one,
  two,
  three,
  four,
  five,
  six,
  seven,
  eight,
  nine,
  hex_a,
  hex_b,
  hex_c,
  hex_d,
  hex_e,
  hex_f,

  decimal,
  enter,
  clear,
  backspace,
  shift,
  ee,
  divide,
  multiply,
  add,
  subtract,
  inverse,
  percent,
  swap,
  changeBase,
  send,
  hidden
};

enum keyMaps {
  standard_map,
  hex_map
};


const uint8_t backspaceIcon[] = {
 16, 11,
 0b00000111, 0b11111111,
 0b00001111, 0b11111111,
 0b00011100, 0b00000011,
 0b00111001, 0b10110011,
 0b01110001, 0b11110011,
 0b11100000, 0b11100011,
 0b01110001, 0b11110011,
 0b00111001, 0b10110011,
 0b00011100, 0b00000011,
 0b00001111, 0b11111111,
 0b00000111, 0b11111111,
};

void CalculatorApp::begin() {
  BritepadApp::begin();
  coord_t gap = 1;
  coord_t xorig = screen.clipLeft() + gap;
  coord_t yorig = screen.clipTop() + displayHeight + gap;
  coord_t xspacing = (screen.clipWidth()-gap)/keyColumns;
  coord_t yspacing = (screen.clipHeight()-displayHeight-gap)/keyRows;
  coord_t w = xspacing - gap;
  coord_t h =  yspacing - gap;
  color_t bg = screen.yellow;
  font_t f = Arial_12_Bold;
  bool highlight = false;

// button set 0
// top row
  coord_t x = xorig;
  coord_t y = yorig;
  uint8_t r = 0;
  uint8_t c = 0;
  uint8_t m = standard_map;

  button[m][r][c].setID(shift);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"...",f);
  x += xspacing;

  button[m][r][c].setID(seven);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"7",f);
  x += xspacing;

  button[m][r][c].setID(eight);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"8",f);
  x += xspacing;

  button[m][r][c].setID(nine);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"9",f);
  x += xspacing;

  button[m][r][c].setID(divide);
  button[m][r][c++].init(x,y,w,h,screen.cyan,highlight,"/",f);
  x += xspacing;

  button[m][r][c].setID(inverse);
  button[m][r][c++].init(x,y,w,h,screen.cyan,highlight,"1/x",f);
  x += xspacing;

// second row
  x = xorig;
  y += yspacing;
  r++;
  c = 0;

  button[m][r][c].setID(changeBase);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"hex",f);
  x += xspacing;

  button[m][r][c].setID(four);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"4",f);
  x += xspacing;

  button[m][r][c].setID(five);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"5",f);
  x += xspacing;

  button[m][r][c].setID(six);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"6",f);
  x += xspacing;

  button[m][r][c].setID(multiply);
  button[m][r][c++].init(x,y,w,h,screen.cyan,highlight,"x",f);
  x += xspacing;

  button[m][r][c].setID(percent);
  button[m][r][c++].init(x,y,w,h,screen.cyan,highlight,"%",f);
  x += xspacing;

// third row
  x = xorig;
  y += yspacing;
  r++;
  c = 0;

  button[m][r][c].setID(backspace);
  button[m][r][c++].init(x,y,w,h,screen.red,highlight,nullptr,f,screen.black,backspaceIcon);
  x += xspacing;

  button[m][r][c].setID(one);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"1",f);
  x += xspacing;

  button[m][r][c].setID(two);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"2",f);
  x += xspacing;

  button[m][r][c].setID(three);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"3",f);
  x += xspacing;

  button[m][r][c].setID(add);
  button[m][r][c++].init(x,y,w,h,screen.cyan,highlight,"+",f);
  x += xspacing;

  button[m][r][c].setID(swap);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"x<>y",Arial_10_Bold);
  x += xspacing;

// fourth row
  x = xorig;
  y += yspacing;
  r++;
  c = 0;

  button[m][r][c].setID(clear);
  button[m][r][c++].init(x,y,w,h,screen.red,highlight,"clear",f);
  x += xspacing;

  button[m][r][c].setID(zero);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"0",f);
  x += xspacing;

  button[m][r][c].setID(decimal);
  button[m][r][c++].init(x,y,w,h,screen.midgreen,highlight,".",f);
  x += xspacing;

  button[m][r][c].setID(ee);
  button[m][r][c++].init(x,y,w,h,screen.midgreen,highlight,"E",f);
  x += xspacing;

  button[m][r][c].setID(subtract);
  button[m][r][c++].init(x,y,w,h,screen.cyan,highlight,"-",f);
  x += xspacing;

  button[m][r][c].setID(enter);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"enter",f);
  x += xspacing;


// button set 1
// top row
  x = xorig;
  y = yorig;
  r = 0;
  c = 0;
  m = hex_map;

  button[m][r][c].setID(shift);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"...",f);
  x += xspacing;

  button[m][r][c].setID(seven);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"7",f);
  x += xspacing;

  button[m][r][c].setID(eight);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"8",f);
  x += xspacing;

  button[m][r][c].setID(nine);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"9",f);
  x += xspacing;

  button[m][r][c].setID(hex_a);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"a",f);
  x += xspacing;

  button[m][r][c].setID(add);
  button[m][r][c++].init(x,y,w,h,screen.cyan,highlight,"+",f);
  x += xspacing;

// second row
  x = xorig;
  y += yspacing;
  r++;
  c = 0;

  button[m][r][c].setID(changeBase);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"dec",f);
  x += xspacing;

  button[m][r][c].setID(four);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"4",f);
  x += xspacing;

  button[m][r][c].setID(five);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"5",f);
  x += xspacing;

  button[m][r][c].setID(six);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"6",f);
  x += xspacing;

  button[m][r][c].setID(hex_b);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"b",f);
  x += xspacing;

  button[m][r][c].setID(subtract);
  button[m][r][c++].init(x,y,w,h,screen.cyan,highlight,"-",f);
  x += xspacing;

// third row
  x = xorig;
  y += yspacing;
  r++;
  c = 0;

  button[m][r][c].setID(backspace);
  button[m][r][c++].init(x,y,w,h,screen.red,highlight,nullptr,f,screen.black,backspaceIcon);
  x += xspacing;

  button[m][r][c].setID(one);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"1",f);
  x += xspacing;

  button[m][r][c].setID(two);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"2",f);
  x += xspacing;

  button[m][r][c].setID(three);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"3",f);
  x += xspacing;

  button[m][r][c].setID(hex_c);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"c",f);
  x += xspacing;

  button[m][r][c].setID(multiply);
  button[m][r][c++].init(x,y,w,h,screen.cyan,highlight,"x",Arial_10_Bold);
  x += xspacing;

// fourth row
  x = xorig;
  y += yspacing;
  r++;
  c = 0;

  button[m][r][c].setID(clear);
  button[m][r][c++].init(x,y,w,h,screen.red,highlight,"clear",f);
  x += xspacing;

  button[m][r][c].setID(zero);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"0",f);
  x += xspacing;

  button[m][r][c].setID(hex_f);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"f",f);
  x += xspacing;

  button[m][r][c].setID(hex_e);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"e",f);
  x += xspacing;

  button[m][r][c].setID(hex_d);
  button[m][r][c++].init(x,y,w,h,screen.green,highlight,"d",f);
  x += xspacing;

  button[m][r][c].setID(enter);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"enter",f);
  x += xspacing;

  drawKeys();
  drawDisplay();
}

void CalculatorApp::drawKeys() {
  for (int m = 0; m < keyMaps; m++) {
    for (int r = 0; r < keyRows; r++) {
      for (int c = 0; c < keyColumns; c++) {
        button[m][r][c].setVisible(curKeyMap == m && button[m][r][c].getID() != hidden);
        button[m][r][c].draw();
      }
    }
  }
}

void CalculatorApp::run() {
  for (int r = 0; r < keyRows; r++) {
    for (int c = 0; c < keyColumns; c++) {
      if (button[curKeyMap][r][c].down()) {
        handleKey(button[curKeyMap][r][c].getID());
      }
    }
  }

  if (pad.down(SCREEN_PAD) && pad.y() < screen.clipTop()+displayHeight) {
      handleKey(send);
  }
}

void CalculatorApp::handleKey(uint8_t keyPressed) {
  switch (keyPressed) {
    case zero:
    case one:
    case two:
    case three:
    case four:
    case five:
    case six:
    case seven:
    case eight:
    case nine:
    case hex_a:
    case hex_b:
    case hex_c:
    case hex_d:
    case hex_e:
    case hex_f:
      keyDigit(keyPressed);
      break;
    case decimal:
      keyDecimal();
      break;
    case enter:
      keyEnter();
      break;
    case clear:
      keyClear();
      break;
    case backspace:
      keyBackspace();
      break;
    case shift:
      keyShift();
      break;
    case ee:
      keyEE();
      break;
    case divide:
      keyDivide();
      break;
    case multiply:
      keyMultiply();
      break;
    case add:
      keyAdd();
      break;
    case subtract:
      keySubtract();
      break;
    case inverse:
      keyInverse();
      break;
    case percent:
      keyPercent();
      break;
    case swap:
      keySwap();
      break;
    case changeBase:
      keyBase();
      break;
    case send:
      keySend();
      break;
    default:
      break;
  }
  drawDisplay();
}

void CalculatorApp::acceptText() {
  if (strlen(topText)) {
    pop();
    if (base == 10) {
      push(strtod(topText, nullptr));
    } else {
      long long top64 = strtol(topText, nullptr, base);
      // try to do 2s compliment conversion of hex
      if (top64 > 0x7ffffff) {
        top64 =  top64 - 0x100000000;
      }
      push(top64);
    }

    topText[0]=0;
  }
}

void CalculatorApp::keySwap() {
  acceptText();
  double a = pop();
  double b = pop();
  push(a);
  push(b);
}

void CalculatorApp::keyInverse() {
  acceptText();
  if (top() == 0) {
    sound.beep();
  } else {
    push(1/pop());
  }
}

void CalculatorApp::keyDivide() {
  acceptText();
  if (top() == 0) {
    sound.beep();
  } else {
    double divisor = pop();
    double dividend = pop();
    push(dividend/divisor);
  }
}

void CalculatorApp::keyAdd() {
  acceptText();
  push(pop()+pop());
}

void CalculatorApp::keyMultiply() {
  acceptText();
  push(pop()*pop());
}

void CalculatorApp::keySubtract() {
  acceptText();
  double value = pop();
  push(pop()-value);
}

// todo: implement these
void CalculatorApp::keyShift() {
  sound.beep();
}

void CalculatorApp::keyBase() {
  acceptText();
  if (base == 10) {
    setKeyMap(hex_map);
    base = 16;
  } else {
    setKeyMap(standard_map);
    base = 10;
  }
  drawDisplay();
}

void CalculatorApp::keyPercent() {
  acceptText();
  double fractional = pop();
  double whole = top();
  push(fractional*whole/100);
}

void CalculatorApp::keyBackspace() {
  int len = strlen(topText);
  if (len) {
    topText[len-1] = 0;
  } else {
    sound.beep();
  }
}

void CalculatorApp::keyClear() {
  if (strlen(topText)) {
    topText[0]=0;
  } else if (top()) {
    pop();
    push(0.0);
  } else {
    clearStack();
  }
}

void CalculatorApp::keySend() {
  acceptText();
  char textNum[100];
  formatText(textNum, top(0));

  Keyboard.print(textNum);
  // todo: make this not blocking
  for (size_t i = 0; i < strlen(textNum); i++) {
    sound.click();
    delay(50);
  }
}

void CalculatorApp::keyEnter() {
  if (strlen(topText) == 0) {
    push(top());
  } else {
    acceptText();
  }
}

void CalculatorApp::keyDigit(uint8_t digit) {
  int len = strlen(topText);
  if (len < maxTopText()) {
    if (len == 0) {
      push(0);
      if (base == 16) {
        topText[len++]='0';
        topText[len++]='x';
      }
    }
    if (digit <= 9) {
      topText[len] = digit+'0';
    } else {
      topText[len] = digit-10+'a';
    }
    topText[len+1] = 0;
  } else {
    sound.beep();
  }
}

void CalculatorApp::keyEE() {
  int len = strlen(topText);
  if (len >= maxTopText() || len < 1 || strchr(topText, 'e')) {
    sound.beep();
  } else {
    topText[len] = 'e';
    topText[len+1] = 0;
  }
}

void CalculatorApp::keyDecimal() {
  int len = strlen(topText);
  if (len >= maxTopText() || strchr(topText, '.') || strchr(topText, 'e')) {
    sound.beep();
  } else {
    if (len == 0) {
      push(0);
    }
    topText[len] = '.';
    topText[len+1] = 0;
  }
}

void CalculatorApp::drawDisplay() {
  screen.fillRect(screen.clipLeft(), screen.clipTop(), screen.clipWidth(), displayHeight, screen.lightergrey);
  screen.setTextColor(screen.black);

  char number[100];
  char* printNumber;
  if (strlen(topText)) {
    printNumber = topText;
  } else {
    formatText(number, top(0));
    printNumber = number;
  }

  if (strlen(printNumber) < 11) {
    screen.setFont(Arial_28);
  } else if (strlen(number) < 15) {
    screen.setFont(Arial_20);
  } else {
    screen.setFont(Arial_16);
  }

  coord_t w = screen.measureTextWidth(printNumber);

  screen.setCursor(screen.clipRight()-w, screen.clipTop() + displayHeight/2 - screen.measureTextHeight(printNumber)/2);
  screen.drawText(printNumber);

  coord_t topWidth = screen.measureTextWidth(printNumber);

  char line1[100];
  char line2[100];
  char line3[100];
  sprintf(number, "%s\n%s\n%s", formatText(line1, top(3)), formatText(line2, top(2)), formatText(line3, top(1)));

  screen.setFont(Arial_9_Bold);

  // if it doesn't fit, shrink it down
  if ((screen.measureTextWidth(number) + topWidth)>(screen.clipWidth()-10)) {
    screen.setFont(Arial_8);
  }

  screen.setCursor(screen.clipLeft() + 5, screen.clipTop()+displayHeight/2-screen.measureTextHeight(number)/2);
  screen.drawText(number);
}

char* CalculatorApp::formatText(char* fstring, double value) {
  if (base == 16) {
    int intValue = value;
    sprintf(fstring, "0x%x", (unsigned int)intValue);
  } else {
    sprintf(fstring, "%.12G", value);
  }
  return fstring;
}
