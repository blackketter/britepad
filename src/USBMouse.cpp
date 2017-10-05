#include "USBMouse.h"

USBHIDParser hid1(usbHost);
USBHIDParser hid2(usbHost);
USBHIDParser hid3(usbHost);
USBHIDParser hid4(usbHost);
USBHIDParser hid5(usbHost);
MouseController _mouse(usbHost);

USBMouse usbMouse;

void USBMouse::run() {
  if(_mouse.available()) {
    usb_mouse_buttons_state = _mouse.getButtons();
    Mouse.move(_mouse.getMouseX(), _mouse.getMouseY(),_mouse.getWheel());
    _mouse.mouseDataClear();
  }
}
