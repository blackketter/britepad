#ifndef _CalculatorTestApp_
#define _CalculatorTestApp_
#include "BritepadApp.h"
#include "widgets/Button.h"
#include "widgets/ButtonMatrix.h"

class CalculatorTestApp : public BritepadApp {
  public:
    void begin(AppMode asMode);
    void end();
    void run();

    BritepadApp* exitsTo() { return A_MOUSE_APP; }

    const char* name() { return "CalculatorTest"; }
    appid_t id() { return ID; };
    static constexpr appid_t ID = "clct";

    AppType getAppType() { return KEY_APP; }

  private:

    static const coord_t displayHeight = 48;
    static const int stackDepth = 4;
    double stack[stackDepth] = { 0.0,0.0,0.0,0.0 };

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

    char* formatText(char* fstring, double value);

    void drawDisplay();
//    ButtonMatrix* buttons = nullptr;

    void handleKey(widgetid_t keyPressed);
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
};

#endif

