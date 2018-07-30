#include "BritepadShared.h"
#include "BritepadApp.h"
#include "KeyApp.h"
#include "widgets/Icon.h"

#include <usb_keyboard.h>

static const color_t defaultColor = Screen::Orange;
static const color_t volColor = Screen::Olive;

// default screen has quick buttons
KeyApp volUpKey(volPlusIcon, KEY_MEDIA_VOLUME_INC, volColor, 0);
KeyApp volDownKey(volMinusIcon, KEY_MEDIA_VOLUME_DEC, volColor, 4);
KeyApp muteKey(muteIcon, KEY_MEDIA_MUTE, volColor, 8);

KeyApp rewKey(rewIcon, KEY_MEDIA_PREV_TRACK, defaultColor, 1);
KeyApp pauseKey(pauseIcon, KEY_MEDIA_PLAY_PAUSE, defaultColor, 2);
KeyApp ffKey(ffIcon, KEY_MEDIA_NEXT_TRACK, defaultColor, 3);

// just for testing
//  KeyApp myNameKey("My Name", "Dean\nBlackketter");

// i never use this
 KeyApp sleepKey("Sleep", KEY_SYSTEM_SLEEP,defaultColor,11);

void KeyApp::run() {

  millis_t now = Uptime::millis();
  millis_t since = now - lastKey;
  if (since < (repeating ? keyRepeat : keyStartRepeat)) {
    return;
  }

  if (lastKey != 0) {
    repeating = true;
  }

  lastKey = Uptime::millis();

  const char* slowstr = keyStr;

  switch (mediaKey) {
    case 0:
      // type it slower, as some apps can't keep up. TODO: make this configurable
      while (*slowstr != 0) {
        Keyboard.write(*slowstr);
        sound.click();
        slowstr++;
        delay(50);
      }
      break;
    case KEY_MEDIA_VOLUME_INC:
    case KEY_MEDIA_VOLUME_DEC:
    case KEY_MEDIA_PREV_TRACK:
    case KEY_MEDIA_NEXT_TRACK:
    case KEY_MEDIA_PLAY_PAUSE:
    case KEY_MEDIA_STOP:
    case KEY_MEDIA_MUTE:
      Keyboard.set_media(mediaKey);
      Keyboard.send_now();
      Keyboard.set_media(0);
      Keyboard.send_now();
      console.debugf("sent media key: %d\n", mediaKey);
      break;
    case KEY_SYSTEM_SLEEP:
      Keyboard.press(MODIFIERKEY_ALT);
      Keyboard.press(MODIFIERKEY_GUI);
    case KEY_MEDIA_EJECT:
      Keyboard.set_media(KEY_MEDIA_EJECT);
      Keyboard.send_now();
      // todo use a timer rather than blocking
      delay(300);  // macOS will not recognize a very short eject press
      Keyboard.set_media(0);
      Keyboard.send_now();
      Keyboard.release(MODIFIERKEY_ALT);
      Keyboard.release(MODIFIERKEY_GUI);
      break;
    default:
      Keyboard.write(mediaKey);
      break;
    }

  if (!canBeInvisible()) {
    draw();
  }

  if (!canBeInvisible()) {
    exit();
  }
}

bool KeyApp::canBeInvisible() {
  // keyapps that just send one key don't show feedback
  return (mediaKey != 0);
}

void KeyApp::draw() {
  clearScreen();
  screen.setFont(Arial_20_Bold);
  screen.setTextColor(screen.yellow);
  int x = screen.clipMidWidth() - (screen.measureTextWidth(nameStr) / 2);
  int y = screen.clipMidHeight() - (screen.measureTextHeight(nameStr) / 2);
  screen.setCursor( x, y);

  screen.drawText(nameStr);

  // todo use a timer to defer exiting rather than blocking
  delay(200);
}
