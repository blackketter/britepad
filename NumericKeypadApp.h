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
    BritepadApp* exitsTo() { return DEFAULT_APP; }

    const char* name() { return "Keypad"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "keyp";

  private:
    ButtonMatrix* buttons = nullptr;

    static const int keyMaps = 1;
    static const int keyColumns = 5;
    static const int keyRows = 4;

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
      zerozero,
      period,
      enter,
      plus,
      minus,
      times,
      divide,
      equals,
      tab,
      backspace
    };
//
//  7 8 9 / BS
//  4 5 6 * TAB
//  1 2 3 - =
//  0 00. + ENTER
//
    ButtonConfig keyConfig[keyMaps][keyRows][keyColumns] =
    {
      { // Basic numeric keypad map
        {
          {screen.grey, "7", Arial_16_Bold, screen.white, nullptr, seven},
          {screen.grey, "8", Arial_16_Bold, screen.white, nullptr, eight},
          {screen.grey, "9", Arial_16_Bold, screen.white, nullptr, nine},
          {screen.grey, "/", Arial_16_Bold, screen.white, nullptr, divide},
          {screen.grey, nullptr, Arial_16_Bold, screen.white, backspaceIcon, backspace},
         },
        {
          {screen.grey, "4", Arial_16_Bold, screen.white, nullptr, four},
          {screen.grey, "5", Arial_16_Bold, screen.white, nullptr, five},
          {screen.grey, "6", Arial_16_Bold, screen.white, nullptr, six},
          {screen.grey, "*", Arial_16_Bold, screen.white, nullptr, times},
          {screen.grey, nullptr, Arial_16_Bold, screen.white, tabIcon16, tab},
         },
        {
          {screen.grey, "1", Arial_16_Bold, screen.white, nullptr, one},
          {screen.grey, "2", Arial_16_Bold, screen.white, nullptr, two},
          {screen.grey, "3", Arial_16_Bold, screen.white, nullptr, three},
          {screen.grey, "-", Arial_16_Bold, screen.white, nullptr, minus},
          {screen.grey, "=", Arial_16_Bold, screen.white, nullptr, equals},
         },
        {
          {screen.grey, "0",     Arial_16_Bold, screen.white, nullptr, zero},
          {screen.grey, "00",    Arial_16_Bold, screen.white, nullptr, zerozero},
          {screen.grey, ".",     Arial_16_Bold, screen.white, nullptr, period},
          {screen.grey, "+",     Arial_16_Bold, screen.white, nullptr, plus},
          {screen.grey, "enter", Arial_12_Bold, screen.white, nullptr, enter},
         },
      }
    };
  };

#endif
