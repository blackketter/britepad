/*
#include "KeyMatrix.h"
#include "KeyboardApp.h"

typedef struct keysound_t {
  keycode_t code,
  float freq
};

keysound_t keySounds[] =
{  MODIFIERKEY_LEFT_SHIFT
{  MODIFIERKEY_LEFT_GUI
{  MODIFIERKEY_LEFT_ALT
{  MODIFIERKEY_LEFT_CTRL
{  KEYLEFT_FN
{  MODIFIERKEY_RIGHT_SHIFT
{  MODIFIERKEY_RIGHT_GUI
{  MODIFIERKEY_RIGHT_ALT
{  MODIFIERKEY_RIGHT_CTRL
{  KEYRIGHT_FN
{  NO_CODE, 0.0 }
}
class KeyboardSoundsApp : public KeyboardApp {
  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "lbar";
    const char* name() { return "Key\nSounds"; };

    void idle() {
      if (getEnabled(KEYBOARD_MODE)) {

        if (keys.keyPressed((keycode_t)KEY_LEFT_FN) ||
            keys.keyPressed((keycode_t)KEY_RIGHT_FN)) {
          if (keys.getMap() != KeyboardSoundsMap) {
            keys.setMap(KeyboardSoundsMap);
          }
        } else if (
                   (keys.keyReleased((keycode_t)KEY_LEFT_FN) || keys.keyReleased((keycode_t)KEY_RIGHT_FN)) &&  // released fn key
                    keys.keyIsUp((keycode_t)KEY_RIGHT_FN) && keys.keyIsUp((keycode_t)KEY_LEFT_FN) &&           // neither key is still held
                    !keys.doubleTapped((keycode_t)KEY_RIGHT_FN) && !keys.doubleTapped((keycode_t)KEY_LEFT_FN)  // and it's not a double-tap
                  ) {
                keys.setMap();
        }
      }
    };
  private:
};

KeyboardSoundsApp theKeyboardSoundsApp;
*/