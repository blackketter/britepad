#ifndef _NumericKeypadApp_
#define _NumericKeypadApp_
#include "BritepadApp.h"
#include "Button.h"
#include "ButtonMatrix.h"
#include "Icons.h"


class NumericKeypadApp : public BritepadApp {
  public:
    void begin();
    void end();
    void run();

    virtual bool disablesScreensavers() { return true; }
    BritepadApp* exitsTo() { return MOUSE_APP; }

    const char* name() { return "Keypad"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "keyp";

  private:
    ButtonMatrix* buttons = nullptr;

    static const int keyMaps = 1;
    static const int keyColumns = 5;
    static const int keyRows = 4;

    enum keys {
      zero = '0',
      one = '1',
      two = '2',
      three = '3',
      four = '4',
      five = '5',
      six = '6',
      seven = '7',
      eight = '8',
      nine = '9',
      zerozero = 'z',
      period = '.',
      enter = '\n',
      plus = '+',
      minus = '-',
      times = 'x',
      divide = '/',
      equals = '=',
      tab = ' ',
      backspace = 0x08
    };
//
//  7 8 9 / B
//  4 5 6 * TAB
//  1 2 3 - =
//  0 00. + ENTER
//
    ButtonConfig keyConfig[keyMaps][keyRows][keyColumns] =
    {
      { // Basic numeric keypad map
        {
          {screen.grey30, "7", Arial_16_Bold, screen.white, nullptr, seven},
          {screen.grey30, "8", Arial_16_Bold, screen.white, nullptr, eight},
          {screen.grey30, "9", Arial_16_Bold, screen.white, nullptr, nine},
          {screen.grey40, "/", Arial_16_Bold, screen.white, nullptr, divide},
          {screen.grey20, nullptr, Arial_16_Bold, screen.white, backspaceIcon, backspace},
         },
        {
          {screen.grey30, "4", Arial_16_Bold, screen.white, nullptr, four},
          {screen.grey30, "5", Arial_16_Bold, screen.white, nullptr, five},
          {screen.grey30, "6", Arial_16_Bold, screen.white, nullptr, six},
          {screen.grey40, "*", Arial_16_Bold, screen.white, nullptr, times},
          {screen.grey20, nullptr, Arial_16_Bold, screen.white, tabIcon16, tab},
         },
        {
          {screen.grey30, "1", Arial_16_Bold, screen.white, nullptr, one},
          {screen.grey30, "2", Arial_16_Bold, screen.white, nullptr, two},
          {screen.grey30, "3", Arial_16_Bold, screen.white, nullptr, three},
          {screen.grey40, "-", Arial_16_Bold, screen.white, nullptr, minus},
          {screen.grey20, "=", Arial_16_Bold, screen.white, nullptr, equals},
         },
        {
          {screen.grey30, "0",     Arial_16_Bold, screen.white, nullptr, zero},
          {screen.grey30, "00",    Arial_16_Bold, screen.white, nullptr, zerozero},
          {screen.grey30, ".",     Arial_16_Bold, screen.white, nullptr, period},
          {screen.grey40, "+",     Arial_16_Bold, screen.white, nullptr, plus},
          {screen.grey20, "enter", Arial_12_Bold, screen.white, nullptr, enter},
         },
      }
    };
  };

#endif
