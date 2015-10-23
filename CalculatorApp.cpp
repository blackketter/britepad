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

void CalculatorApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);

  coord_t xorig = screen.clipLeft() + 1;
  coord_t yorig = screen.clipTop() + displayHeight + 1;
  coord_t xspacing = screen.clipWidth()/keyColumns;
  coord_t yspacing = (screen.clipHeight()-displayHeight)/keyRows;
  coord_t w = xspacing - 1;
  coord_t h =  yspacing - 1;
  color_t bg = screen.yellow;
  font_t f = Arial_12_Bold;
  bool highlight = false;
// top row
  coord_t x = xorig;
  coord_t y = yorig;
  uint8_t r = 0;
  uint8_t c = 0;
  uint8_t m = 0;

  button[m][r][c].setID(shift);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"...",f);
  x += xspacing;

  button[m][r][c].setID(seven);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"7",f);
  x += xspacing;

  button[m][r][c].setID(eight);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"8",f);
  x += xspacing;

  button[m][r][c].setID(nine);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"9",f);
  x += xspacing;

  button[m][r][c].setID(divide);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"/",f);
  x += xspacing;

  button[m][r][c].setID(inverse);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"1/x",f);
  x += xspacing;

// second row
  x = xorig;
  y += yspacing;
  r++;
  c = 0;

  button[m][r][c].setID(changeBase);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"base",f);
  x += xspacing;

  button[m][r][c].setID(four);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"4",f);
  x += xspacing;

  button[m][r][c].setID(five);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"5",f);
  x += xspacing;

  button[m][r][c].setID(six);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"6",f);
  x += xspacing;

  button[m][r][c].setID(multiply);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"x",f);
  x += xspacing;

  button[m][r][c].setID(percent);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"%",f);
  x += xspacing;

// third row
  x = xorig;
  y += yspacing;
  r++;
  c = 0;

  button[m][r][c].setID(backspace);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"BS",f);
  x += xspacing;

  button[m][r][c].setID(one);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"1",f);
  x += xspacing;

  button[m][r][c].setID(two);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"2",f);
  x += xspacing;

  button[m][r][c].setID(three);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"3",f);
  x += xspacing;

  button[m][r][c].setID(add);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"+",f);
  x += xspacing;

  button[m][r][c].setID(swap);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"<->",f);
  x += xspacing;

// fourth row
  x = xorig;
  y += yspacing;
  r++;
  c = 0;

  button[m][r][c].setID(clear);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"C",f);
  x += xspacing;

  button[m][r][c].setID(decimal);
  button[m][r][c++].init(x,y,w,h,bg,highlight,".",f);
  x += xspacing;

  button[m][r][c].setID(zero);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"0",f);
  x += xspacing;

  button[m][r][c].setID(ee);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"E",f);
  x += xspacing;

  button[m][r][c].setID(subtract);
  button[m][r][c++].init(x,y,w,h,bg,highlight,"-",f);
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
    default:
      break;
  }
  drawDisplay();
}

void CalculatorApp::acceptText() {
  if (strlen(topText)) {
    pop();
    push(strtod(topText, nullptr));
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
  sound.beep();
}

void CalculatorApp::keyPercent() {
  sound.beep();
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
  sprintf(textNum, "%.12G", top(0));
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
  if (len < maxTopText) {
    if (len == 0) {
      push(0);
    }
    if (digit <= 9) {
      topText[len] = digit+'0';
    } else {
      topText[len] = digit+'a';
    }
    topText[len+1] = 0;
  } else {
    sound.beep();
  }
}

void CalculatorApp::keyEE() {
  int len = strlen(topText);
  if (len >= maxTopText || len < 1 || strchr(topText, 'e')) {
    sound.beep();
  } else {
    topText[len] = 'e';
    topText[len+1] = 0;
  }
}

void CalculatorApp::keyDecimal() {
  int len = strlen(topText);
  if (len >= maxTopText || strchr(topText, '.') || strchr(topText, 'e')) {
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
  screen.fillRect(screen.clipLeft(), screen.clipTop(), screen.clipWidth(), displayHeight, screen.lightgrey);
  screen.setTextColor(screen.black);
  screen.setFont(Arial_28_Bold);

  char number[20*3];
  char* printNumber = number;
  if (strlen(topText)) {
    printNumber = topText;
  } else {
    sprintf(number, "%.12G", top(0));
  }

  coord_t w = screen.measureTextWidth(printNumber);

  screen.setCursor(screen.clipRight()-w, screen.clipTop() + displayHeight/2 - screen.measureTextHeight(printNumber)/2);
  screen.drawText(printNumber);

  screen.setFont(Arial_9_Bold);

  sprintf(number, "%.12G\n%.12G\n%.12G", top(3), top(2), top(1));

  screen.setCursor(screen.clipLeft() + 5, screen.clipTop()+displayHeight/2-screen.measureTextHeight(number)/2);
  screen.drawText(number);
}
