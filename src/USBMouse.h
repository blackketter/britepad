#ifndef _USBMouse_
#define _USBMouse_

#include "BritepadShared.h"
#include "USBHost.h"

MouseController _mouse(usbHost);
USBHub hub1(usbHost);
USBHub hub2(usbHost);
USBHub hub3(usbHost);

class USBMouse {
  public:
    void begin() {}
    void end() {}
    void run() {
      if(_mouse.available()) {
        usb_mouse_buttons_state = _mouse.getButtons();
        Mouse.move(_mouse.getMouseX(), _mouse.getMouseY(),_mouse.getWheel());
        _mouse.mouseDataClear();
      }
    }

  private:
};

USBMouse usbMouse;
#endif
