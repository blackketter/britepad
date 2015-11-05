#ifndef _CalculatorApp_
#define _CalculatorApp_
#include "BritepadApp.h"
#include "Button.h"

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
  hidden,

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
  log2_f
};

enum keyMaps {
  basic_map,
  hex_map,
  sci_map
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

class CalculatorApp : public BritepadApp {
  public:
    virtual void begin();
    virtual void run();
    virtual bool disablesScreensavers() { return true; }
    BritepadApp* exitsTo() { return DEFAULT_APP; }

    const char* name() { return "Calculator"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "calc";

  private:
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

    void setKeyMap(uint8_t newMap) { curKeyMap = newMap; drawKeys(); }

    static const uint8_t keyMaps = 3;
    static const int keyColumns = 6;
    static const int keyRows = 4;
    static const int keys = keyRows*keyColumns;

    uint8_t curKeyMap = 0;

    char* formatText(char* fstring, double value);

    void drawKeys();
    void drawDisplay();
//    Button button[keyMaps][keyRows][keyColumns];

    Button button[keyMaps][keyRows][keyColumns] =
      {
        { // Basic calc map
          {
            Button(screen.yellow, "...", Arial_12_Bold, screen.black, nullptr, shift),
            Button(screen.green, "7", Arial_12_Bold, screen.black, nullptr, seven),
            Button(screen.green, "8", Arial_12_Bold, screen.black, nullptr, eight),
            Button(screen.green, "9", Arial_12_Bold, screen.black, nullptr, nine),
            Button(screen.cyan, "/", Arial_12_Bold, screen.black, nullptr, divide),
            Button(screen.cyan, "1/x", Arial_12_Bold, screen.black, nullptr, inverse),
          },
          {
            Button(screen.yellow, "hex", Arial_12_Bold, screen.black, nullptr, changeBase),
            Button(screen.green, "4", Arial_12_Bold, screen.black, nullptr, four),
            Button(screen.green, "5", Arial_12_Bold, screen.black, nullptr, five),
            Button(screen.green, "6", Arial_12_Bold, screen.black, nullptr, six),
            Button(screen.cyan, "x", Arial_12_Bold, screen.black, nullptr, multiply),
            Button(screen.cyan, "%", Arial_12_Bold, screen.black, nullptr, percent),
          },
          {
            Button(screen.red, nullptr, Arial_12_Bold, screen.black, backspaceIcon, backspace),
            Button(screen.green, "1", Arial_12_Bold, screen.black, nullptr, one),
            Button(screen.green, "2", Arial_12_Bold, screen.black, nullptr, two),
            Button(screen.green, "3", Arial_12_Bold, screen.black, nullptr, three),
            Button(screen.cyan, "+", Arial_12_Bold, screen.black, nullptr, add),
            Button(screen.yellow, "x<>y", Arial_12_Bold, screen.black, nullptr, swap),
          },
          {
            Button(screen.red, "clear", Arial_12_Bold, screen.black, nullptr, clear),
            Button(screen.green, "0", Arial_12_Bold, screen.black, nullptr, zero),
            Button(screen.green, ".", Arial_12_Bold, screen.black, nullptr, decimal),
            Button(screen.green, "E", Arial_12_Bold, screen.black, nullptr, ee),
            Button(screen.cyan, "-", Arial_12_Bold, screen.black, nullptr, subtract),
            Button(screen.yellow, "enter", Arial_12_Bold, screen.black, nullptr, enter),
          },
        },
        { // Hex calc map
          {
            Button(screen.yellow, "...", Arial_12_Bold, screen.black, nullptr, shift),
            Button(screen.green, "7", Arial_12_Bold, screen.black, nullptr, seven),
            Button(screen.green, "8", Arial_12_Bold, screen.black, nullptr, eight),
            Button(screen.green, "9", Arial_12_Bold, screen.black, nullptr, nine),
            Button(screen.green, "a", Arial_12_Bold, screen.black, nullptr, hex_a),
            Button(screen.cyan, "+", Arial_12_Bold, screen.black, nullptr, add),
          },
          {
            Button(screen.yellow, "dec", Arial_12_Bold, screen.black, nullptr, changeBase),
            Button(screen.green, "4", Arial_12_Bold, screen.black, nullptr, four),
            Button(screen.green, "5", Arial_12_Bold, screen.black, nullptr, five),
            Button(screen.green, "6", Arial_12_Bold, screen.black, nullptr, six),
            Button(screen.green, "b", Arial_12_Bold, screen.black, nullptr, hex_b),
            Button(screen.cyan, "-", Arial_12_Bold, screen.black, nullptr, subtract),
          },
          {
            Button(screen.red, nullptr, Arial_12_Bold, screen.black, backspaceIcon, backspace),
            Button(screen.green, "1", Arial_12_Bold, screen.black, nullptr, one),
            Button(screen.green, "2", Arial_12_Bold, screen.black, nullptr, two),
            Button(screen.green, "3", Arial_12_Bold, screen.black, nullptr, three),
            Button(screen.green, "c", Arial_12_Bold, screen.black, nullptr, hex_c),
            Button(screen.cyan, "x", Arial_12_Bold, screen.black, nullptr, multiply),
          },
          {
            Button(screen.red, "clear", Arial_12_Bold, screen.black, nullptr, clear),
            Button(screen.green, "0", Arial_12_Bold, screen.black, nullptr, zero),
            Button(screen.green, "f", Arial_12_Bold, screen.black, nullptr, hex_f),
            Button(screen.green, "e", Arial_12_Bold, screen.black, nullptr, hex_e),
            Button(screen.green, "d", Arial_12_Bold, screen.black, nullptr, hex_d),
            Button(screen.yellow, "enter", Arial_12_Bold, screen.black, nullptr, enter),
          },
        },
        { // Sci calc map
          {
            Button(screen.yellow, "...", Arial_12_Bold, screen.black, nullptr, shift),
            Button(screen.cyan, "x^2", Arial_12_Bold, screen.black, nullptr, square_f),
            Button(screen.cyan, "x^3", Arial_12_Bold, screen.black, nullptr, cube_f),
            Button(screen.cyan, "x^y", Arial_12_Bold, screen.black, nullptr, pow_f),
            Button(screen.cyan, "e^x", Arial_12_Bold, screen.black, nullptr, exp_f),
            Button(screen.cyan, "10^x", Arial_12_Bold, screen.black, nullptr, pow10_f),
          },
          {
            Button(screen.yellow, "hex", Arial_12_Bold, screen.black, nullptr, changeBase),
            Button(screen.cyan, "sqrt", Arial_12_Bold, screen.black, nullptr, sqrt_f),
            Button(screen.cyan, "cubert", Arial_12_Bold, screen.black, nullptr, cubert_f),
            Button(screen.cyan, "xroot", Arial_12_Bold, screen.black, nullptr, xroot_f),
            Button(screen.cyan, "ln", Arial_12_Bold, screen.black, nullptr, ln_f),
            Button(screen.cyan, "log10", Arial_12_Bold, screen.black, nullptr, log10_f),
          },
          {
            Button(screen.red, nullptr, Arial_12_Bold, screen.black, backspaceIcon, backspace),
            Button(screen.cyan, "sin", Arial_12_Bold, screen.black, nullptr, sin_f),
            Button(screen.cyan, "cos", Arial_12_Bold, screen.black, nullptr, cos_f),
            Button(screen.cyan, "tan", Arial_12_Bold, screen.black, nullptr, tan_f),
            Button(screen.cyan, "pi", Arial_12_Bold, screen.black, nullptr, pi_f),
            Button(screen.cyan, "e", Arial_12_Bold, screen.black, nullptr, e_f),
          },
          {
            Button(screen.red, "clear", Arial_12_Bold, screen.black, nullptr, clear),
            Button(screen.green, "asin", Arial_12_Bold, screen.black, nullptr, asin_f),
            Button(screen.green, "acos", Arial_12_Bold, screen.black, nullptr, acos_f),
            Button(screen.green, "atan", Arial_12_Bold, screen.black, nullptr, atan_f),
            Button(screen.green, "logy", Arial_12_Bold, screen.black, nullptr, logy_f),
            Button(screen.yellow, "log2", Arial_12_Bold, screen.black, nullptr, log2_f),
          },
        }
      };

    void handleKey(uint8_t keyPressed);
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

    void keySquare() { acceptText(); double x = pop(); push(x*x); };
    void keyCube() { acceptText(); double x = pop(); push(x*x*x); };
    void keyPow()  { acceptText(); keySwap(); push(pow(pop(),pop())); };
    void keyExp()  { acceptText(); push(pow(pop(),M_E)); };
    void keyPow10(){ acceptText(); push(pow(10,pop())); };
    void keySqrt() { acceptText(); push(sqrt(pop())); };
    void keyCubert(){acceptText(); push(pow(pop(),1.0/3.0)); };
    void keyXroot(){ acceptText(); keySwap(); push(pow(pop(),1.0/pop())); };
    void keyLn()   { acceptText(); push(log(pop())/log(M_E)); };
    void keyLog10(){ acceptText(); push(log(pop())/log(10.0)); };
    void keySin()  { acceptText(); push(sin(pop())); };
    void keyCos()  { acceptText(); push(cos(pop())); };
    void keyTan()  { acceptText(); push(tan(pop())); };
    void keyPi()   { acceptText(); push(M_PI); };
    void keyE()    { acceptText(); push(M_E); };
    void keyAsin() { acceptText(); push(asin(pop())); };
    void keyAcos() { acceptText(); push(acos(pop())); };
    void keyAtan() { acceptText(); push(atan(pop())); };
    void keyLogy() { acceptText(); keySwap(); push(log(pop())/log(pop())); };
    void keyLog2() { acceptText(); push(log(pop())/log(2.0)); };
  };

#endif

