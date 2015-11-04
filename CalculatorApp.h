#ifndef _CalculatorApp_
#define _CalculatorApp_
#include "BritepadApp.h"
#include "Button.h"

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

    static const int keyColumns = 6;
    static const int keyRows = 4;
    static const int keys = keyRows*keyColumns;
    static const uint8_t keyMaps = 2;
    uint8_t curKeyMap = 0;

    char* formatText(char* fstring, double value);

    void drawKeys();
    void drawDisplay();
    Button button[keyMaps][keyRows][keyColumns];

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
  };

#endif

