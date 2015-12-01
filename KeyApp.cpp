#include "BritepadShared.h"
#include "BritepadApp.h"
#include "KeyApp.h"

#include <usb_keyboard.h>


void KeyApp::run() {

  const char* slowstr = keyStr;

  switch (mediaKey) {
    case 0:
      // type it slower, as some apps can't keep up. TODO: make this configurable
      while (*slowstr != 0) {
        Keyboard.write(*slowstr);
        slowstr++;
//  added delay to try to make the login screen from a cold boot on mac work.  it does not.  todo: fix it
//        delay(50);
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
      break;
    case KEY_MEDIA_EJECT:
      Keyboard.set_media(KEY_MEDIA_EJECT);
      Keyboard.send_now();
      // todo use a timer rather than blocking
      delay(300);  // Mac OS-X will not recognize a very short eject press
      Keyboard.set_media(0);
      Keyboard.send_now();
      break;
    default:
      Keyboard.write(mediaKey);
      break;
    }

  if (!isInvisible()) {
    draw();
  }

  if (!isInvisible()) {
    exit();
  }
}

bool KeyApp::isInvisible() {
  // keyapps that just send one key don't show feedback
  return (mediaKey != 0);
}

color_t KeyApp::buttonColor() {
  return button_color;
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
