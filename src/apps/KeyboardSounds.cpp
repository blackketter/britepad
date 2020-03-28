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
      prefs.get(id(), sizeof(_soundsEnabled), &_soundsEnabled);
    };

    void run() {
      _soundsEnabled = !_soundsEnabled;
      console.debugf("Sounds Enabled: %d\n",_soundsEnabled);
      prefs.set(id(), sizeof(_soundsEnabled), &_soundsEnabled);
   }


    EventPriority eventPriority() { return PRIORITY_FIRST; }

    bool event(KeyEvent* key) {
      if (_soundsEnabled) {

        if (key->pressed() && key->hard() && !key->code(NO_CODE)) {
          sound.click();
        }
      }
      return false;
    }

  private:
    uint8_t _soundsEnabled = 1;
};

KeyboardSoundsApp theKeyboardSoundsApp;
