#include "USBKeyMatrix.h"
#include "USBKeyLayout.h"
#include "Apps/KeyboardViewerApp.h"

USBKeyMatrix usbKeys;

KeyboardViewerApp theUSBKeyboardViewerApp(&usbKeys);

USBKeyMatrix::USBKeyMatrix() {
   _defaultLayout = ANSI104KeyLayout;
   _defaultMap = ANSI104KeyMap;
   setMap();
   setLayout();
}

void USBKeyMatrix::begin() {
  KeyMatrix::begin();
}

keyswitch_t USBKeyMatrix::update() {
  keyswitch_t count = 0;
  return count;
}

bool USBKeyMatrix::switchIsDown(keyswitch_t k) {
  return false;
}

bool USBKeyMatrix::switchIsUp(keyswitch_t k) {
  return true;
}
