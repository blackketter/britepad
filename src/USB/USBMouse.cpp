#include "USBMouse.h"
#include "USBHost.h"

USBHIDParser hid1(usbHost);
USBHIDParser hid2(usbHost);
USBHIDParser hid3(usbHost);
USBHIDParser hid4(usbHost);
USBHIDParser hid5(usbHost);
MouseController _mouse(usbHost);

USBMouse usbMouse;

void USBMouse::run() {
  if(_mouse.available()) {
    _buttons = _mouse.getButtons();
    _dx = _mouse.getMouseX();
    _dy = _mouse.getMouseY();
    _dwheel = _mouse.getWheel();

    _x += _dx;
    _y += _dy;
    _wheel += _dwheel;

    checkBounds();

    usb_mouse_buttons_state = _buttons;
    Mouse.move(_dx, _dy, _dwheel);
//    if (_dwheel) console.debugf("scrolling %d\n", _dwheel);
    _mouse.mouseDataClear();
    _lastmove = Uptime::millis();
  }
}

void USBMouse::setBounds(coord_t x, coord_t y, coord_t w, coord_t h) {
  _bounds.x = x;
  _bounds.y = y;
  _bounds.w = w;
  _bounds.h = h;
  checkBounds();
}

void USBMouse::checkBounds() {
  if (_x < _bounds.x) { _x = _bounds.x; }
  if (_y < _bounds.y) { _y = _bounds.y; }
  if (_x >= _bounds.x+_bounds.w) { _x = _bounds.x+_bounds.w-1; }
  if (_y >= _bounds.y+_bounds.h) { _y = _bounds.y+_bounds.h-1; }
}