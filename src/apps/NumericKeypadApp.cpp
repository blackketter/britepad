#include "NumericKeypadApp.h"
#include "Sound.h"
#include <usb_keyboard.h>

void NumericKeypadApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);
  if (!buttons) {
    buttons = new ButtonMatrix(screen.clipLeft(),(coord_t)(screen.clipTop()),screen.clipWidth(),(coord_t)(screen.clipHeight()),keyRows,keyColumns,keyMaps,(ButtonConfig*)keyConfig);
  }
  buttons->draw();
}


void NumericKeypadApp::end() {
  delete buttons;
  buttons = nullptr;
  BritepadApp::end();
}

void NumericKeypadApp::run() {
  Button* b = nullptr;

  if (pad.isGesturing()) {
    // todo: maybe draw the gesture
    DEBUGF("Is gesturing\n");
    buttons->setHighlighted(false);
  } else if (pad.didGesture()) {
    if (pad.getGesture() == GESTURE_SWIPE_UP) {
      exit();
    } else {
      b = buttons->getButton(pad.getGesture());
      if (b) {
        DEBUGF("Found button for gesture 0x%02x\n", pad.getGesture());
      } else {
        DEBUGF("NO Found button for gesture 0x%02x\n", pad.getGesture());
      }
    }
  } else {
    b = buttons->up();
  }

  if (b) {
    switch (b->getID()) {
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
        Keyboard.write(b->getID());
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
