#include "USBKeyMatrix.h"
#include "USBKeyLayout.h"
#include "Apps/KeyboardViewerApp.h"

USBKeyMatrix usbKeys;
KeyboardViewerApp theUSBKeyboardViewerApp(&usbKeys);

class RawKeyboardController : public KeyboardController {
  protected:
    void new_data(const Transfer_t *transfer);
  private:
    uint8_t lastBuf[8];
};

RawKeyboardController _keyboard;

static keycode_t keycode(uint8_t usbCode) {
  if (usbCode < 4) {
    return NO_KEY;
  } else {
    return usbCode | 0xF000;
  }
}

static bool contains(uint8_t b, const uint8_t *data)
{
	if (data[2] == b || data[3] == b || data[4] == b) return true;
	if (data[5] == b || data[6] == b || data[7] == b) return true;
	return false;
}

void RawKeyboardController::new_data(const Transfer_t *transfer)
{
  uint8_t* buf = (uint8_t*)transfer->buffer;
  uint32_t len = transfer->length;
  if (len != 8) {
    console.debug("bad usb transfer length\n");
  } else {
    uint8_t mods = buf[0];
    uint8_t oldMods = lastBuf[0];
    if (mods != oldMods) {
      const uint8_t LEFT_CTRL = 0x01;
      const uint8_t RIGHT_CTRL = 0x10;
      const uint8_t LEFT_SHIFT = 0x02;
      const uint8_t RIGHT_SHIFT = 0x20;
      const uint8_t LEFT_ALT = 0x04;
      const uint8_t RIGHT_ALT = 0x40;
      const uint8_t LEFT_GUI = 0x08;
      const uint8_t RIGHT_GUI = 0x80;

      // modifier changed
      if ( (mods & LEFT_CTRL) != (oldMods & LEFT_CTRL) ) { usbKeys.usbKeyEvent(MODIFIERKEY_LEFT_CTRL, mods & LEFT_CTRL); }
      if ( (mods & RIGHT_CTRL) != (oldMods & RIGHT_CTRL) ) { usbKeys.usbKeyEvent(MODIFIERKEY_RIGHT_CTRL, mods & RIGHT_CTRL); }
      if ( (mods & LEFT_SHIFT) != (oldMods & LEFT_SHIFT) ) { usbKeys.usbKeyEvent(MODIFIERKEY_LEFT_SHIFT, mods & LEFT_SHIFT); }
      if ( (mods & RIGHT_SHIFT) != (oldMods & RIGHT_SHIFT) ) { usbKeys.usbKeyEvent(MODIFIERKEY_RIGHT_SHIFT, mods & RIGHT_SHIFT); }
      if ( (mods & LEFT_ALT) != (oldMods & LEFT_ALT) ) { usbKeys.usbKeyEvent(MODIFIERKEY_LEFT_ALT, mods & LEFT_ALT); }
      if ( (mods & RIGHT_ALT) != (oldMods & RIGHT_ALT) ) { usbKeys.usbKeyEvent(MODIFIERKEY_RIGHT_ALT, mods & RIGHT_ALT); }
      if ( (mods & LEFT_GUI) != (oldMods & LEFT_GUI) ) { usbKeys.usbKeyEvent(MODIFIERKEY_LEFT_GUI, mods & LEFT_GUI); }
      if ( (mods & RIGHT_GUI) != (oldMods & RIGHT_GUI) ) { usbKeys.usbKeyEvent(MODIFIERKEY_RIGHT_GUI, mods & RIGHT_GUI); }
    }

    for (int i=2; i < 8; i++) {
      uint32_t key = lastBuf[i];
      if (key >= 4 && !contains(key, buf)) {
        usbKeys.usbKeyEvent(keycode(key), false);
      }
    }
    for (int i=2; i < 8; i++) {
      uint32_t key = buf[i];
      if (key >= 4 && !contains(key, lastBuf)) {
        usbKeys.usbKeyEvent(keycode(key), true);
      }
    }

    memcpy(lastBuf,buf,len);

  }
  KeyboardController::new_data(transfer);
}


USBKeyMatrix::USBKeyMatrix() {
   _defaultLayout = ANSI104KeyLayout;
   _defaultMap = ANSI104KeyMap;
   setMap();
   setLayout();
}

void USBKeyMatrix::begin() {
  KeyMatrix::begin();
}

void USBKeyMatrix::update() {
}

void USBKeyMatrix::usbKeyEvent(keyswitch_t k, bool down) {
  keyEvents.addEvent(this, k, getCode(k), Uptime::millis(), down);
}

