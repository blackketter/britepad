#if FLASH_SIZE > (1024*256)

#include "CalculatorApp.h"

CalculatorApp theCalculatorApp;

enum keyMap {
  basic_map,
  hex_map,
  sci_map,
  conv_map,
  keyMaps
};

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
  adds,
  subtract,
  inverse,
  percent,
  swap,
  changeBase,
  send,

  square_f,
  cube_f,
  pow_f,
  exp_f,
  pow10_f,
  sqrt_f,
  cubert_f,
  xroot_f,
  ln_f,
  log10_f,
  sin_f,
  cos_f,
  tan_f,
  pi_f,
  e_f,
  asin_f,
  acos_f,
  atan_f,
  logy_f,
  log2_f,
  in2mm_f,
  mm2in_f,
  g2lb_f,  lb2g_f ,  c2f_f ,  f2c_f ,
  ppi_f , // pixels per inch

  unused
};

const int keyColumns = 6;
const int keyRows = 4;
#if 0
const ButtonConfig keyConfig[1][3][3] = {

        { // Sci calc map
          {
            {screen.cyan, "x^3", Arial_12_Bold, screen.black, nullptr, cube_f, '0'},
            {screen.cyan, "x^y", Arial_12_Bold, screen.black, nullptr, pow_f, '0'},
            {screen.cyan, "e^x", Arial_12_Bold, screen.black, nullptr, exp_f, '0'},
          },
          {
             {screen.cyan, "cubert", Arial_12_Bold, screen.black, nullptr, cubert_f, '0'},
            {screen.cyan, "xroot", Arial_12_Bold, screen.black, nullptr, xroot_f, '0'},
            {screen.cyan, "ln", Arial_12_Bold, screen.black, nullptr, ln_f, '0'},
           },
          {
            {screen.cyan, "cos", Arial_12_Bold, screen.black, nullptr, cos_f, '0'},
            {screen.cyan, "tan", Arial_12_Bold, screen.black, nullptr, tan_f, '0'},
            {screen.cyan, "pi", Arial_12_Bold, screen.black, nullptr, pi_f, '0'},
          },
       },
      };
#else
const ButtonConfig keyConfig[keyMaps][keyRows][keyColumns] = {
        { // Basic calc map
          {
            {screen.yellow, "...", Arial_12_Bold, screen.black, nullptr, shift, '\t'},
            {screen.green, "7", Arial_12_Bold, screen.black, nullptr, seven, '7'},
            {screen.green, "8", Arial_12_Bold, screen.black, nullptr, eight, '8'},
            {screen.green, "9", Arial_12_Bold, screen.black, nullptr, nine, '9'},
            {screen.cyan, "/", Arial_12_Bold, screen.black, nullptr, divide, '/'},
            {screen.cyan, "1/x", Arial_12_Bold, screen.black, nullptr, inverse, '\\'},
          },
          {
            {screen.yellow, "hex", Arial_12_Bold, screen.black, nullptr, changeBase, MODIFIERKEY_LEFT_CTRL},
            {screen.green, "4", Arial_12_Bold, screen.black, nullptr, four, '4'},
            {screen.green, "5", Arial_12_Bold, screen.black, nullptr, five, '5'},
            {screen.green, "6", Arial_12_Bold, screen.black, nullptr, six, '6'},
            {screen.cyan, "x", Arial_12_Bold, screen.black, nullptr, multiply, '*'},
            {screen.cyan, "%", Arial_12_Bold, screen.black, nullptr, percent, '%'},
          },
          {
            {screen.red, nullptr, Arial_12_Bold, screen.black, backspaceIcon, backspace, '\b'},
            {screen.green, "1", Arial_12_Bold, screen.black, nullptr, one, '1'},
            {screen.green, "2", Arial_12_Bold, screen.black, nullptr, two, '2'},
            {screen.green, "3", Arial_12_Bold, screen.black, nullptr, three, '3'},
            {screen.cyan, "+", Arial_12_Bold, screen.black, nullptr, adds, '+'},
            {screen.yellow, "x<>y", Arial_12_Bold, screen.black, nullptr, swap, 's'},
          },
          {
            {screen.red, "clear", Arial_12_Bold, screen.black, nullptr, clear, 'c' },
            {screen.green, "0", Arial_12_Bold, screen.black, nullptr, zero, '0'},
            {screen.green, ".", Arial_12_Bold, screen.black, nullptr, decimal, '.'},
            {screen.green, "E", Arial_12_Bold, screen.black, nullptr, ee, 'e'},
            {screen.cyan, "-", Arial_12_Bold, screen.black, nullptr, subtract, '-'},
            {screen.yellow, "enter", Arial_12_Bold, screen.black, nullptr, enter, '\r'},
          },
        },
        { // Hex calc map
          {
            {screen.yellow, "...", Arial_12_Bold, screen.black, nullptr, shift, '\t'},
            {screen.green, "7", Arial_12_Bold, screen.black, nullptr, seven, '7'},
            {screen.green, "8", Arial_12_Bold, screen.black, nullptr, eight, '8'},
            {screen.green, "9", Arial_12_Bold, screen.black, nullptr, nine, '9'},
            {screen.green, "a", Arial_12_Bold, screen.black, nullptr, hex_a, 'a'},
            {screen.cyan, "+", Arial_12_Bold, screen.black, nullptr, adds, '+'},
          },
          {
            {screen.yellow, "dec", Arial_12_Bold, screen.black, nullptr, changeBase, MODIFIERKEY_LEFT_CTRL},
            {screen.green, "4", Arial_12_Bold, screen.black, nullptr, four, '4'},
            {screen.green, "5", Arial_12_Bold, screen.black, nullptr, five, '5'},
            {screen.green, "6", Arial_12_Bold, screen.black, nullptr, six, '6'},
            {screen.green, "b", Arial_12_Bold, screen.black, nullptr, hex_b, 'b'},
            {screen.cyan, "-", Arial_12_Bold, screen.black, nullptr, subtract, '-'},
          },
          {
            {screen.red, nullptr, Arial_12_Bold, screen.black, backspaceIcon, backspace, '\b'},
            {screen.green, "1", Arial_12_Bold, screen.black, nullptr, one, '1'},
            {screen.green, "2", Arial_12_Bold, screen.black, nullptr, two, '2'},
            {screen.green, "3", Arial_12_Bold, screen.black, nullptr, three, '3'},
            {screen.green, "c", Arial_12_Bold, screen.black, nullptr, hex_c, 'c'},
            {screen.cyan, "x", Arial_12_Bold, screen.black, nullptr, multiply, '*'},
          },
          {
            {screen.red, "clear", Arial_12_Bold, screen.black, nullptr, clear, 0 },
            {screen.green, "0", Arial_12_Bold, screen.black, nullptr, zero, '0'},
            {screen.green, "f", Arial_12_Bold, screen.black, nullptr, hex_f, 'f'},
            {screen.green, "e", Arial_12_Bold, screen.black, nullptr, hex_e, 'e'},
            {screen.green, "d", Arial_12_Bold, screen.black, nullptr, hex_d, 'd'},
            {screen.yellow, "enter", Arial_12_Bold, screen.black, nullptr, enter, '\n'},
          },
        },
        { // Sci calc map
          {
            {screen.yellow, "...", Arial_12_Bold, screen.black, nullptr, shift, '\t'},
            {screen.cyan, "x^2", Arial_12_Bold, screen.black, nullptr, square_f, 'q'},
            {screen.cyan, "x^3", Arial_12_Bold, screen.black, nullptr, cube_f, 'w'},
            {screen.cyan, "x^y", Arial_12_Bold, screen.black, nullptr, pow_f, 'e'},
            {screen.cyan, "e^x", Arial_12_Bold, screen.black, nullptr, exp_f, 'r'},
            {screen.cyan, "10^x", Arial_12_Bold, screen.black, nullptr, pow10_f, 't'},
          },
          {
            {screen.yellow, "hex", Arial_12_Bold, screen.black, nullptr, changeBase, MODIFIERKEY_LEFT_CTRL},
            {screen.cyan, "sqrt", Arial_12_Bold, screen.black, nullptr, sqrt_f, 'a'},
            {screen.cyan, "cubert", Arial_12_Bold, screen.black, nullptr, cubert_f, 's'},
            {screen.cyan, "xroot", Arial_12_Bold, screen.black, nullptr, xroot_f, 'd'},
            {screen.cyan, "ln", Arial_12_Bold, screen.black, nullptr, ln_f, 'f'},
            {screen.cyan, "log10", Arial_12_Bold, screen.black, nullptr, log10_f, 'g'},
          },
          {
            {screen.red, nullptr, Arial_12_Bold, screen.black, backspaceIcon, backspace, '\b'},
            {screen.cyan, "sin", Arial_12_Bold, screen.black, nullptr, sin_f, 'z'},
            {screen.cyan, "cos", Arial_12_Bold, screen.black, nullptr, cos_f, 'x'},
            {screen.cyan, "tan", Arial_12_Bold, screen.black, nullptr, tan_f, 'c'},
            {screen.cyan, "pi", Arial_12_Bold, screen.black, nullptr, pi_f, 'v'},
            {screen.cyan, "e", Arial_12_Bold, screen.black, nullptr, e_f, 'b'},
          },
          {
            {screen.red, "clear", Arial_12_Bold, screen.black, nullptr, clear, 0 },
            {screen.cyan, "asin", Arial_12_Bold, screen.black, nullptr, asin_f, 'Z'},
            {screen.cyan, "acos", Arial_12_Bold, screen.black, nullptr, acos_f, 'X'},
            {screen.cyan, "atan", Arial_12_Bold, screen.black, nullptr, atan_f, 'C'},
            {screen.cyan, "logy", Arial_12_Bold, screen.black, nullptr, logy_f, 'V'},
            {screen.cyan, "log2", Arial_12_Bold, screen.black, nullptr, log2_f, 'B'},
          },
        },
        { // Conversions map
          {
            {screen.yellow, "...", Arial_12_Bold, screen.black, nullptr, shift, '\t'},
            {screen.cyan, "in > mm", Arial_9_Bold, screen.black, nullptr, in2mm_f, 'q'},
            {screen.cyan, "mm > in", Arial_9_Bold, screen.black, nullptr, mm2in_f, 'w'},
            {screen.cyan, "g > lb", Arial_9_Bold, screen.black, nullptr, g2lb_f, 'e'},
            {screen.cyan, "lb > g", Arial_9_Bold, screen.black, nullptr, lb2g_f, 'r'},
            {screen.cyan, "", Arial_9_Bold, screen.black, nullptr, unused, 0},
          },
          {
            {screen.yellow, "hex", Arial_12_Bold, screen.black, nullptr, changeBase, MODIFIERKEY_LEFT_CTRL},
            {screen.cyan, "c > f", Arial_9_Bold, screen.black, nullptr, c2f_f, 'a'},
            {screen.cyan, "f > c", Arial_9_Bold, screen.black, nullptr, f2c_f, 's'},
            {screen.cyan, "ppi", Arial_12_Bold, screen.black, nullptr, ppi_f, 'd'},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused, 'f'},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused, 'g'},
          },
          {
            {screen.red, nullptr, Arial_12_Bold, screen.black, backspaceIcon, backspace, '\b'},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused, 'z'},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused, 'x'},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused, 'c'},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused, 'v'},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused, 'b'},
          },
          {
            {screen.red, "clear", Arial_12_Bold, screen.black, nullptr, clear, 0},
            {screen.green, "", Arial_12_Bold, screen.black, nullptr, unused, 'Z'},
            {screen.green, "", Arial_12_Bold, screen.black, nullptr, unused, 'X'},
            {screen.green, "", Arial_12_Bold, screen.black, nullptr, unused, 'C'},
            {screen.green, "", Arial_12_Bold, screen.black, nullptr, unused, 'V'},
            {screen.yellow, "", Arial_12_Bold, screen.black, nullptr, unused, 'B'},
          },
        }
    };
#endif

void CalculatorApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);
  drawDisplay();
  if (!buttons) {
    buttons = new ButtonMatrix(screen.clipLeft(),(coord_t)(screen.clipTop()+displayHeight),screen.clipWidth(),(coord_t)(screen.clipHeight()-displayHeight),keyRows,keyColumns,keyMaps,(ButtonConfig*)keyConfig);
  }
  buttons->draw();
}

void CalculatorApp::end() {
  delete buttons;
  buttons = nullptr;
  BritepadApp::end();
}

void CalculatorApp::run() {
  Button* hit = buttons->pressedButton();
  if (hit) {
    handleKey(hit->getID());
  }

  KeyEvent* key = getNextEvent();
  if (key) {
    hit = buttons->keyButton(key);
    if (key->pressed()) {
      if (hit) {
        handleKey(hit->getID());
      } else if (key->code(KEY_ESC)) {
        exit();
      } else if (key->code(KEY_SPACE)) {
        handleKey(send);
      } else {
        char c = key->character();
        if (c >= '0' && c <= '9') {
          handleKey(c-'0'+zero);
        }
      }
    }
  }

  if (pad.pressed(SCREEN_PAD) && pad.y() < screen.clipTop()+displayHeight) {
      handleKey(send);
  }
}

void CalculatorApp::handleKey(widgetid_t keyPressed) {

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
      case adds:
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
      case in2mm_f:
        keyIn2mm();
        break;
      case mm2in_f:
        keyMm2In();
        break;
      case g2lb_f:
        keyG2lb();
        break;
      case lb2g_f:
        keyLb2g();
        break;
      case c2f_f:
        keyC2f();
        break;
      case f2c_f:
        keyF2c();
        break;
      case ppi_f:
        keyPpi();
        break;
      default:
        break;
    }
  }

  drawDisplay();
}

void CalculatorApp::acceptText() {
  if (strlen(topText)) {
    pop();
    if (getBase() == 10) {
      push(strtod(topText, nullptr));
    } else {
      int64_t top64 = strtol(topText, nullptr, getBase());
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
  keyMap curMap = (keyMap)buttons->getMap();

  switch (curMap) {
    case sci_map:
      curMap = conv_map;
      break;
    case basic_map:
      curMap = sci_map;
      break;
    default:
      curMap = basic_map;
      break;
  }
  buttons->setMap(curMap);
  buttons->draw();
}

void CalculatorApp::keyBase() {
  if (getBase() == 10) {
    buttons->setMap(hex_map);
    setBase(16);
  } else {
    buttons->setMap(basic_map);
    setBase(10);
  }
  buttons->draw();
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
#endif