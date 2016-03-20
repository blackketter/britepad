#ifndef _TouchKeyboard_
#define _TouchKeyboard_

#include "BritepadShared.h"
#include "Button.h"
#include "Widget.h"

class TouchKeyboard : public Widget {
  public:
    virtual void init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, font_t legendFont = Arial_9_Bold, color_t legendColor = screen.black);

    virtual void track();
    virtual void draw();

    virtual uint8_t down();  // returns value of key pressed down or 0 if none.  as side effect tracks keys and does redraw
    virtual uint8_t up();

  protected:


    virtual uint8_t key(coord_t x, coord_t y);
    virtual uint8_t key(int row, int col);
    virtual int keysInRow(int rowNum);
    virtual void initButtons();


    font_t legendF;
    color_t legendC;

    color_t colored;

    enum KeySet {
      LOWER_SET,
      UPPER_SET,
      NUMBER_SET,
      PUNCT_SET
    };

    void setKeySet(KeySet newSet);
    KeySet currSet = LOWER_SET;

    static const uint8_t keysets = 4;
    static const uint8_t rows = 4;
    static const uint8_t maxKeysPerRow = 10;

    RoundButton buttons[rows][maxKeysPerRow];

// special keys
    static const uint8_t NO_KEY = 0;
    static const uint8_t TAB_KEY = '\t';
    static const uint8_t BS_KEY = '\b';
    static const uint8_t RETURN_KEY = '\n';
    static const uint8_t ESC_KEY = '\x1b';

// modifiers
    static const uint8_t CTRL_KEY = '\x10';
    static const uint8_t CMD_KEY = '\x13';
    static const uint8_t OPT_KEY = '\x14';

// keyboard switching
    static const uint8_t SHIFTDOWN_KEY = '\x0e';
    static const uint8_t SHIFTUP_KEY = '\x0f';
    static const uint8_t PUNCT_KEY = '\x11';
    static const uint8_t NUMBERS_KEY = '\x12';
    static const uint8_t ARROWS_KEY = '\x15';
    static const uint8_t FUNCTION_KEY = '\x16';
    static const uint8_t ALPHA_KEY = '\x17';

    const char* keymap[keysets][rows] = {
      // lower case
      {
        "qwertyuiop",
        "asdfghjkl",
        "\x0f" "\x12" "zxcvbnm" "\b", // shiftup num zxcvbnm backspace
        "\t" "\x1b" "\x10" "\x13" " " "\x14" "\x15" "\x16" "\n" // tab esc ctrl cmd space opt arrows fn return
      },

      // upper case
      {
        "QWERTYUIOP",
        "ASDFGHJKL",
        "\x0e" "\x12" "ZXCVBNM" "\b", // shiftdown num ZXCVBNM backspace
        "\t" "\x1b" "\x10" "\x13" " " "\x14" "\x15" "\x16" "\n" // tab esc ctrl cmd space opt arrows fn return
      },

      // numbers and common punctuation
      {
        "1234567890",
        "-/:;()$&@\"",
        "\x11" "\x17" ".,?!'" "\b", // punct alpha .,?!'" backspace
        "\t" "\x1b" "\x10" "\x13" " " "\x14" "\x15" "\x16" "\n" // tab esc ctrl cmd space opt arrows fn return
      },

      // uncommon punctuation, todo complete
      {
        "[]{}#%^*+=",
        "_\\|~<>   •",
        "\x12" "\x17" ".,?!'" "\b" // num alpha .,?!' backspace
        "\t" "\x1b" "\x10" "\x13" " " "\x14" "\x15" "\x16" "\n" // tab esc ctrl cmd space opt arrows fn return
      },

      // todo: arrows

      // todo: function keys

    };


};

#endif


