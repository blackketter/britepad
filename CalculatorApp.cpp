#include "CalculatorApp.h"



void CalculatorApp::begin() {
  BritepadApp::begin();
  drawDisplay();
  if (!buttons) {
    buttons = new ButtonMatrix(screen.clipLeft(),(coord_t)(screen.clipTop()+displayHeight),screen.clipWidth(),(coord_t)(screen.clipHeight()-displayHeight),keyRows,keyColumns,keyMaps,(ButtonConfig*)keyConfig);
  }
  buttons->draw();
}

void CalculatorApp::end() {
  BritepadApp::end();
  delete buttons;
  buttons = nullptr;
}

void CalculatorApp::run() {
  Button* hit = buttons->down();
  if (hit) {
    handleKey((keys)hit->getID());
  }

  if (pad.down(SCREEN_PAD) && pad.y() < screen.clipTop()+displayHeight) {
      handleKey(send);
  }
}

void CalculatorApp::handleKey(keys keyPressed) {

  keyMap origKeyMap = (keyMap)buttons->getMap();

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
    default:
      // the following are functions that accept the current input string
      acceptText();
      switch (keyPressed) {
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
      case square_f:
        keySquare();
        break;
      case cube_f:
        keyCube();
        break;
      case pow_f:
        keyPow();
        break;
      case exp_f:
        keyExp();
        break;
      case pow10_f:
        keyPow10();
        break;
      case sqrt_f:
        keySqrt();
        break;
      case cubert_f:
        keyCubert();
        break;
      case xroot_f:
        keyXroot();
        break;
      case ln_f:
        keyLn();
        break;
      case log10_f:
        keyLog10();
        break;
      case sin_f:
        keySin();
        break;
      case cos_f:
        keyCos();
        break;
      case tan_f:
        keyTan();
        break;
      case pi_f:
        keyPi();
        break;
      case e_f:
        keyE();
        break;
      case asin_f:
        keyAsin();
        break;
      case acos_f:
        keyAcos();
        break;
      case atan_f:
        keyAtan();
        break;
      case logy_f:
        keyLogy();
        break;
      case log2_f:
        keyLog2();
         break;
      default:
        break;
    }
  }

  if (origKeyMap == sci_map && buttons->getMap() == sci_map) {
    buttons->setMap(basic_map);
  }

  drawDisplay();
}

void CalculatorApp::acceptText() {
  if (strlen(topText)) {
    pop();
    if (getBase() == 10) {
      push(strtod(topText, nullptr));
    } else {
      long long top64 = strtol(topText, nullptr, getBase());
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
  double a = pop();
  double b = pop();
  push(a);
  push(b);
}

void CalculatorApp::keyInverse() {
  if (top() == 0) {
    sound.beep();
  } else {
    push(1/pop());
  }
}

void CalculatorApp::keyDivide() {
  if (top() == 0) {
    sound.beep();
  } else {
    double divisor = pop();
    double dividend = pop();
    push(dividend/divisor);
  }
}

void CalculatorApp::keyAdd() {
  push(pop()+pop());
}

void CalculatorApp::keyMultiply() {
  push(pop()*pop());
}

void CalculatorApp::keySubtract() {
  double value = pop();
  push(pop()-value);
}

void CalculatorApp::keyShift() {
  if (buttons->getMap() == sci_map) {
    buttons->setMap(basic_map);
  } else {
    buttons->setMap(sci_map);
  }
}

void CalculatorApp::keyBase() {
  if (getBase() == 10) {
    buttons->setMap(hex_map);
    setBase(16);
  } else {
    buttons->setMap(basic_map);
    setBase(10);
  }
  drawDisplay();
}

void CalculatorApp::keyPercent() {
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
      if (getBase() == 16) {
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

  if (getBase() == 16) {
    int intValue = value;
    sprintf(fstring, "0x%x", (unsigned int)intValue);
  } else {
    sprintf(fstring, "%.12G", value);
  }
  return fstring;
}
