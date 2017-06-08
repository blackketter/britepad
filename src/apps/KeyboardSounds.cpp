#include "KeyMatrix.h"
#include "KeyboardApp.h"

/*
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
*/
class KeyboardSoundsApp : public BritepadApp {
  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "ksnd";
    const char* name() { return _soundsEnabled ? "Key Sounds On" : "Key Sounds Off"; };
    AppType getAppType() { return SETTINGS_APP; }
//    virtual bool highlighted() { return !_soundsEnabled; }
    bool canBeInvisible() { return true; };

    void init() {
      prefs.read(id(), sizeof(_soundsEnabled), &_soundsEnabled);
    };

    void run() {
      _soundsEnabled = !_soundsEnabled;
      console.debugf("Sounds Enabled: %d\n",_soundsEnabled);
      prefs.write(id(), sizeof(_soundsEnabled), &_soundsEnabled);
   }


    void idle(KeyEvent* key) {
      if (_soundsEnabled) {

        if (key->pressed()) {
          sound.click();
        }
      }
    }

  private:
    uint8_t _soundsEnabled = 1;
};

KeyboardSoundsApp theKeyboardSoundsApp;
