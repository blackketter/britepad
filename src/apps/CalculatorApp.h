#ifndef _CalculatorApp_
#define _CalculatorApp_
#include "BritepadApp.h"
#include "widgets/Button.h"
#include "widgets/ButtonMatrix.h"
#include "widgets/Icons.h"


class CalculatorApp : public BritepadApp {
  public:
    void begin();
    void end();
    void run();

    BritepadApp* exitsTo() { return MOUSE_APP; }

    const char* name() { return "Calculator"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "calc";

  private:
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
      g2lb_f,
      lb2g_f,
      c2f_f,
      f2c_f,

      ppi_f,  // pixels per inch

      unused
    };

    enum keyMap {
      basic_map,
      hex_map,
      sci_map,
      conv_map,
      keyMaps
    };

    static const coord_t displayHeight = 48;
    static const int stackDepth = 4;
    double stack[stackDepth] = { 0,0,0,0 };

    static const int topTextSize = 13;
    char topText[topTextSize] = "";
    int maxTopText() {if (base == 16) { return 10; } else { return topTextSize-1; } };

    void push(double x) { for (int i = stackDepth - 1; i > 0; i--) { stack[i] = stack[i-1];} stack[0] = x; }
    double pop() { double popped = stack[0]; for (int i = 0; i < stackDepth-1; i++) { stack[i]=stack[i+1]; } stack[stackDepth-1] = 0; return popped; }
    double top(int depth = 0) { return stack[depth]; }
    void clearStack() { for (int i = 0; i < stackDepth; i++) { stack[i] = 0; }}

    uint8_t base = 10;
    uint8_t getBase() { return base; };
    void setBase(uint8_t newBase) { base = newBase; };

//    static const int keyMaps = 3;
    static const int keyColumns = 6;
    static const int keyRows = 4;

    char* formatText(char* fstring, double value);

    void drawDisplay();
    ButtonMatrix* buttons = nullptr;

    void handleKey(keys keyPressed);
    void acceptText();

    void keyDigit(uint8_t digit);
    void keyClear();
    void keyEnter();
    void keyDecimal();
    void keyEE();
    void keyBackspace();
    void keyShift();

    void keyDivide();
    void keyMultiply();
    void keySubtract();
    void keyAdd();
    void keyInverse();
    void keySwap();
    void keyPercent();
    void keyBase();
    void keySend();
    void keySquare() { double x = pop(); push(x*x); };
    void keyCube() { double x = pop(); push(x*x*x); };
    void keyPow()  { keySwap(); push(pow(pop(),pop())); };
    void keyExp()  { push(pow(pop(),M_E)); };
    void keyPow10(){ push(pow(10,pop())); };
    void keySqrt() { push(sqrt(pop())); };
    void keyCubert(){push(pow(pop(),1.0/3.0)); };
    void keyXroot(){ keySwap(); push(pow(pop(),1.0/pop())); };
    void keyLn()   { push(log(pop())/log(M_E)); };
    void keyLog10(){ push(log(pop())/log(10.0)); };
    void keySin()  { push(sin(pop())); };
    void keyCos()  { push(cos(pop())); };
    void keyTan()  { push(tan(pop())); };
    void keyPi()   { push(M_PI); };
    void keyE()    { push(M_E); };
    void keyAsin() { push(asin(pop())); };
    void keyAcos() { push(acos(pop())); };
    void keyAtan() { push(atan(pop())); };
    void keyLogy() { keySwap(); push(log(pop())/log(pop())); };
    void keyLog2() { push(log(pop())/log(2.0)); };
    void keyIn2mm() { push(pop()*25.4); };
    void keyMm2In() { push(pop()/25.4); };
    void keyG2lb()  { push(pop()/0.0022046); };
    void keyLb2g()  { push(pop()*0.0022046); };
    void keyC2f()   { push(pop()*9.0/5.0+32.0); };
    void keyF2c()   { push((pop()-32.0)*5.0/9.0); };
    void keyPpi()   { double x,y,d; d = pop(); x = pop(); y = pop(); push(sqrt(x*x + y*y)/d); };

    ButtonConfig keyConfig[keyMaps][keyRows][keyColumns] = {
        { // Basic calc map
          {
            {screen.yellow, "...", Arial_12_Bold, screen.black, nullptr, shift},
            {screen.green, "7", Arial_12_Bold, screen.black, nullptr, seven},
            {screen.green, "8", Arial_12_Bold, screen.black, nullptr, eight},
            {screen.green, "9", Arial_12_Bold, screen.black, nullptr, nine},
            {screen.cyan, "/", Arial_12_Bold, screen.black, nullptr, divide},
            {screen.cyan, "1/x", Arial_12_Bold, screen.black, nullptr, inverse},
          },
          {
            {screen.yellow, "hex", Arial_12_Bold, screen.black, nullptr, changeBase},
            {screen.green, "4", Arial_12_Bold, screen.black, nullptr, four},
            {screen.green, "5", Arial_12_Bold, screen.black, nullptr, five},
            {screen.green, "6", Arial_12_Bold, screen.black, nullptr, six},
            {screen.cyan, "x", Arial_12_Bold, screen.black, nullptr, multiply},
            {screen.cyan, "%", Arial_12_Bold, screen.black, nullptr, percent},
          },
          {
            {screen.red, nullptr, Arial_12_Bold, screen.black, backspaceIcon, backspace},
            {screen.green, "1", Arial_12_Bold, screen.black, nullptr, one},
            {screen.green, "2", Arial_12_Bold, screen.black, nullptr, two},
            {screen.green, "3", Arial_12_Bold, screen.black, nullptr, three},
            {screen.cyan, "+", Arial_12_Bold, screen.black, nullptr, add},
            {screen.yellow, "x<>y", Arial_12_Bold, screen.black, nullptr, swap},
          },
          {
            {screen.red, "clear", Arial_12_Bold, screen.black, nullptr, clear},
            {screen.green, "0", Arial_12_Bold, screen.black, nullptr, zero},
            {screen.green, ".", Arial_12_Bold, screen.black, nullptr, decimal},
            {screen.green, "E", Arial_12_Bold, screen.black, nullptr, ee},
            {screen.cyan, "-", Arial_12_Bold, screen.black, nullptr, subtract},
            {screen.yellow, "enter", Arial_12_Bold, screen.black, nullptr, enter},
          },
        },
        { // Hex calc map
          {
            {screen.yellow, "...", Arial_12_Bold, screen.black, nullptr, shift},
            {screen.green, "7", Arial_12_Bold, screen.black, nullptr, seven},
            {screen.green, "8", Arial_12_Bold, screen.black, nullptr, eight},
            {screen.green, "9", Arial_12_Bold, screen.black, nullptr, nine},
            {screen.green, "a", Arial_12_Bold, screen.black, nullptr, hex_a},
            {screen.cyan, "+", Arial_12_Bold, screen.black, nullptr, add},
          },
          {
            {screen.yellow, "dec", Arial_12_Bold, screen.black, nullptr, changeBase},
            {screen.green, "4", Arial_12_Bold, screen.black, nullptr, four},
            {screen.green, "5", Arial_12_Bold, screen.black, nullptr, five},
            {screen.green, "6", Arial_12_Bold, screen.black, nullptr, six},
            {screen.green, "b", Arial_12_Bold, screen.black, nullptr, hex_b},
            {screen.cyan, "-", Arial_12_Bold, screen.black, nullptr, subtract},
          },
          {
            {screen.red, nullptr, Arial_12_Bold, screen.black, backspaceIcon, backspace},
            {screen.green, "1", Arial_12_Bold, screen.black, nullptr, one},
            {screen.green, "2", Arial_12_Bold, screen.black, nullptr, two},
            {screen.green, "3", Arial_12_Bold, screen.black, nullptr, three},
            {screen.green, "c", Arial_12_Bold, screen.black, nullptr, hex_c},
            {screen.cyan, "x", Arial_12_Bold, screen.black, nullptr, multiply},
          },
          {
            {screen.red, "clear", Arial_12_Bold, screen.black, nullptr, clear},
            {screen.green, "0", Arial_12_Bold, screen.black, nullptr, zero},
            {screen.green, "f", Arial_12_Bold, screen.black, nullptr, hex_f},
            {screen.green, "e", Arial_12_Bold, screen.black, nullptr, hex_e},
            {screen.green, "d", Arial_12_Bold, screen.black, nullptr, hex_d},
            {screen.yellow, "enter", Arial_12_Bold, screen.black, nullptr, enter},
          },
        },
        { // Sci calc map
          {
            {screen.yellow, "...", Arial_12_Bold, screen.black, nullptr, shift},
            {screen.cyan, "x^2", Arial_12_Bold, screen.black, nullptr, square_f},
            {screen.cyan, "x^3", Arial_12_Bold, screen.black, nullptr, cube_f},
            {screen.cyan, "x^y", Arial_12_Bold, screen.black, nullptr, pow_f},
            {screen.cyan, "e^x", Arial_12_Bold, screen.black, nullptr, exp_f},
            {screen.cyan, "10^x", Arial_12_Bold, screen.black, nullptr, pow10_f},
          },
          {
            {screen.yellow, "hex", Arial_12_Bold, screen.black, nullptr, changeBase},
            {screen.cyan, "sqrt", Arial_12_Bold, screen.black, nullptr, sqrt_f},
            {screen.cyan, "cubert", Arial_12_Bold, screen.black, nullptr, cubert_f},
            {screen.cyan, "xroot", Arial_12_Bold, screen.black, nullptr, xroot_f},
            {screen.cyan, "ln", Arial_12_Bold, screen.black, nullptr, ln_f},
            {screen.cyan, "log10", Arial_12_Bold, screen.black, nullptr, log10_f},
          },
          {
            {screen.red, nullptr, Arial_12_Bold, screen.black, backspaceIcon, backspace},
            {screen.cyan, "sin", Arial_12_Bold, screen.black, nullptr, sin_f},
            {screen.cyan, "cos", Arial_12_Bold, screen.black, nullptr, cos_f},
            {screen.cyan, "tan", Arial_12_Bold, screen.black, nullptr, tan_f},
            {screen.cyan, "pi", Arial_12_Bold, screen.black, nullptr, pi_f},
            {screen.cyan, "e", Arial_12_Bold, screen.black, nullptr, e_f},
          },
          {
            {screen.red, "clear", Arial_12_Bold, screen.black, nullptr, clear},
            {screen.green, "asin", Arial_12_Bold, screen.black, nullptr, asin_f},
            {screen.green, "acos", Arial_12_Bold, screen.black, nullptr, acos_f},
            {screen.green, "atan", Arial_12_Bold, screen.black, nullptr, atan_f},
            {screen.green, "logy", Arial_12_Bold, screen.black, nullptr, logy_f},
            {screen.yellow, "log2", Arial_12_Bold, screen.black, nullptr, log2_f},
          },
        },
        { // Conversions map
          {
            {screen.yellow, "...", Arial_12_Bold, screen.black, nullptr, shift},
            {screen.cyan, "in > mm", Arial_9_Bold, screen.black, nullptr, in2mm_f},
            {screen.cyan, "mm > in", Arial_9_Bold, screen.black, nullptr, mm2in_f},
            {screen.cyan, "g > lb", Arial_9_Bold, screen.black, nullptr, g2lb_f},
            {screen.cyan, "lb > g", Arial_9_Bold, screen.black, nullptr, lb2g_f},
            {screen.cyan, "", Arial_9_Bold, screen.black, nullptr, unused},
          },
          {
            {screen.yellow, "hex", Arial_12_Bold, screen.black, nullptr, changeBase},
            {screen.cyan, "c > f", Arial_9_Bold, screen.black, nullptr, c2f_f},
            {screen.cyan, "f > c", Arial_9_Bold, screen.black, nullptr, f2c_f},
            {screen.cyan, "ppi", Arial_12_Bold, screen.black, nullptr, ppi_f},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused},
          },
          {
            {screen.red, nullptr, Arial_12_Bold, screen.black, backspaceIcon, backspace},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused},
            {screen.cyan, "", Arial_12_Bold, screen.black, nullptr, unused},
          },
          {
            {screen.red, "clear", Arial_12_Bold, screen.black, nullptr, clear},
            {screen.green, "", Arial_12_Bold, screen.black, nullptr, unused},
            {screen.green, "", Arial_12_Bold, screen.black, nullptr, unused},
            {screen.green, "", Arial_12_Bold, screen.black, nullptr, unused},
            {screen.green, "", Arial_12_Bold, screen.black, nullptr, unused},
            {screen.yellow, "", Arial_12_Bold, screen.black, nullptr, unused},
          },
        }
    };
};

#endif

