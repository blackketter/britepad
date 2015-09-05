#include "BritepadShared.h"
#include "BritepadApp.h"
#include "KeyApp.h"

#include <usb_keyboard.h>


BritepadApp* KeyApp::run(void) {

  switch (mediaKey) {
    case 0:
      Keyboard.print(keyStr);
      break;
    case KEY_MEDIA_VOLUME_INC:
    case KEY_MEDIA_VOLUME_DEC:
    case KEY_MEDIA_PREV_TRACK:
    case KEY_MEDIA_NEXT_TRACK:
    case KEY_MEDIA_PLAY_PAUSE:
    case KEY_MEDIA_STOP:
      Keyboard.set_media(mediaKey);
      Keyboard.send_now();
      Keyboard.set_media(0);
      Keyboard.send_now();
      break;
    case KEY_MEDIA_EJECT:
      Keyboard.set_media(KEY_MEDIA_EJECT);
      Keyboard.send_now();
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

  if (isInvisible())
    return STAY_IN_APP;
  else
    return DEFAULT_APP;
}

bool KeyApp::isInvisible(void) {
  // keyapps that just send one key don't show feedback
  return (mediaKey != 0);
}

color_t KeyApp::buttonColor(void) {
  return button_color;
}

void KeyApp::draw(void) {
    screen.fillScreen(bgColor());
    screen.setTextSize(3);
    screen.setTextColor(screen.yellow);
    int x = screen.width()/2 - (screen.measureTextH(nameStr) / 2);
    int y =  screen.height()/2 - (screen.measureTextV(nameStr) / 2);
    screen.setCursor( x, y);

    screen.drawText(nameStr);
    delay(200);
}
