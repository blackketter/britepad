#ifndef _TouchKeyboard_
#define _TouchKeyboard_

#include "BritepadShared.h"
#include "Button.h"

class TouchKeyboard {
  public:
    virtual void init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, font_t legendFont = Arial_9_Bold, color_t legendColor = screen.black);

    virtual void track(void);
    virtual void draw(void);

    virtual uint8_t down(void);  // returns value of key pressed down or 0 if none.  as side effect tracks keys and does redraw
    virtual uint8_t up(void);

  protected:


    virtual uint8_t hit(coord_t x, coord_t y);
    virtual uint8_t key(int row, int col);
    virtual int keysInRow(int rowNum);
    virtual void initButtons();


    font_t legendF;
    color_t legendC;
    coord_t xpos;
    coord_t ypos;
    coord_t width;
    coord_t height;

    color_t colored;

    uint8_t currSet = 0;

    static const uint8_t keysets = 4;
    static const uint8_t rows = 4;
    static const uint8_t maxKeysPerRow = 10;
    static const uint8_t NO_KEY = 0;

    RoundButton buttons[rows][maxKeysPerRow];

    const char* keymap[keysets][rows] = {
      // lower case
      {
        "qwertyuiop",
        "asdfghjkl",
        "A#zxcvbnm<", //"\\shift" "\\num" "zxcvbnm" "\\backspace",
        ">~^C O<Fn" // "\\tab" "\\esc" "\\ctrl" "\\cmd" "\\space" "\\opt" "\\arrows" "\\fn" "\\return"
      },

      // upper case
      {
        "QWERTYUIOP",
        "ASDFGHJKL",
        "\\shift" "\\num" "ZXCVBNM" "\\backspace"
        "\\tab" "\\esc" "\\ctrl" "\\cmd" "\\space" "\\opt" "\\arrows" "\\fn" "\\return"
      },

      // numbers and common punctuation
      {
        "1234567890",
        "-/:;()$&@\"",
        "\\punct" "\\alpha" ".,?!'" "\\backspace",
        "\\tab" "\\esc" "\\ctrl" "\\cmd" "\\space" "\\opt" "\\arrows" "\\fn" "\\return"
      },

      // uncommon punctuation
      {
        "[]{}#%^*+=",
        "_\\|~<>   •",
        "\\num" "\\alpha" ".,?!'" "\\backspace",
        "\\tab" "\\esc" "\\ctrl" "\\cmd" "\\space" "\\opt" "\\arrows" "\\fn" "\\return"
      },

      // todo: arrows

      // todo: function keys

    };


};

class CalculatorKeyboard : public TouchKeyboard {

  protected:
    static const uint8_t keysets = 4;
    static const uint8_t rows = 4;
    static const uint8_t maxKeysPerRow = 6;

    point_t keycoord[rows][maxKeysPerRow];
    uint8_t keyStatus[rows][maxKeysPerRow];

    const char* keys[keysets][rows] = {
      // basic 4-function
      {
      },

      // scientific
      {
      },

      // inv scientific
      {
      },

      // computer
      {
      }
    };
};

#endif


