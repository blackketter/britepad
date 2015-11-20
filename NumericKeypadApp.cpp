#include "NumericKeypadApp.h"
#include "Sound.h"
#include <usb_keyboard.h>

void NumericKeypadApp::begin() {
  BritepadApp::begin();
  if (!buttons) {
    buttons = new ButtonMatrix(screen.clipLeft(),(coord_t)(screen.clipTop()),screen.clipWidth(),(coord_t)(screen.clipHeight()),keyRows,keyColumns,keyMaps,(ButtonConfig*)keyConfig);
  }
  buttons->draw();
}


void NumericKeypadApp::end() {
  BritepadApp::end();
  delete buttons;
  buttons = nullptr;
}

void NumericKeypadApp::run() {
  Button* hit = buttons->down();
  if (hit) {
    switch (hit->getID()) {
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
        Keyboard.write(hit->getID()+'0');
        break;
      case zerozero:
        Keyboard.print("00");
        break;
      case period:
        Keyboard.print(".");
        break;
      case enter:
        Keyboard.print("\n");
        break;
      case plus:
        Keyboard.print("+");
        break;
      case minus:
        Keyboard.print("-");
        break;
      case times:
        Keyboard.print("*");
        break;
      case divide:
        Keyboard.print("/");
        break;
      case equals:
        Keyboard.print("=");
        break;
      case tab:
        Keyboard.press(KEY_TAB);
        Keyboard.release(KEY_TAB);
        break;
      case backspace:
        Keyboard.press(KEY_BACKSPACE);
        Keyboard.release(KEY_BACKSPACE);
        break;
      default:
        break;
    }
    sound.click();
  }
}
