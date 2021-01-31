#include "KeyMatrix.h"
#include "KeyboardApp.h"
#include "BritepadShared.h"

// map for BB1 keys to Atreus
static const keymap_t atreusMap[] = {
// row 0

// row 1
    {  0+1, KEY_TAB },
    {  5+1, KEY_Q },
    { 10+1, KEY_W },
    { 15+1, KEY_E },
    { 20+1, KEY_R },
    { 25+1, KEY_T },

    { 55+1, KEY_Y },
    { 60+1, KEY_U },
    { 65+1, KEY_I },
    { 70+1, KEY_O },
    { 75+1, KEY_P },
    { 80+1, KEY_BACKSLASH },

// row 2
    {  0+2, MODIFIERKEY_LEFT_CTRL },
    {  5+2, KEY_A },
    { 10+2, KEY_S },
    { 15+2, KEY_D },
    { 20+2, KEY_F },
    { 25+2, KEY_G },

    { 55+2, KEY_H },
    { 60+2, KEY_J },
    { 65+2, KEY_K },
    { 70+2, KEY_L },
    { 75+2, KEY_SEMICOLON },
    { 80+2, KEY_QUOTE },

// row 3
    {  0+3, MODIFIERKEY_LEFT_SHIFT },
    {  5+3, KEY_Z },
    { 10+3, KEY_X },
    { 15+3, KEY_C },
    { 20+3, KEY_V },
    { 25+3, KEY_B },
    { 30+3, MODIFIERKEY_LEFT_ALT },

    { 50+3, NO_CODE },
    { 55+3, KEY_N },
    { 60+3, KEY_M },
    { 65+3, KEY_COMMA },
    { 70+3, KEY_PERIOD },
    { 75+3, KEY_SLASH },
    { 80+3, MODIFIERKEY_RIGHT_SHIFT },

// row 4
    { 20+4, KEY_LEFT_FN },
    { 25+4, KEY_BACKSPACE },
    { 30+4, MODIFIERKEY_LEFT_GUI },

    { 50+4, KEY_ENTER },
    { 55+4, KEY_SPACE },
    { 60+4, KEY_RIGHT_FN },

 // end of table marker
    { NO_KEY, NO_CODE }
};

const keyoverlay_t atreusOverlay[] = {
// row 0
    { KEY_TAB, KEY_EQUAL },
    { KEY_Q, KEY_1 },
    { KEY_W, KEY_2 },
    { KEY_E, KEY_3 },
    { KEY_R, KEY_4 },
    { KEY_T, KEY_5 },

    { KEY_Y, KEY_6 },
    { KEY_U, KEY_7 },
    { KEY_I, KEY_8 },
    { KEY_O, KEY_9 },
    { KEY_P, KEY_0 },
    { KEY_BACKSLASH, KEY_MINUS },

    { KEY_A, KEY_TILDE },
    { KEY_S, KEY_MEDIA_PREV_TRACK },
    { KEY_D, KEY_MEDIA_PLAY_PAUSE },
    { KEY_F, KEY_MEDIA_NEXT_TRACK },
    { KEY_G, KEY_MEDIA_VOLUME_INC },
    { KEY_B, KEY_MEDIA_VOLUME_DEC },
    { KEY_BACKSPACE, KEY_MEDIA_MUTE },
    { KEY_Z, KEY_FIRST_MACRO + 8 },
    { KEY_X, KEY_FIRST_MACRO + 9 },
    { KEY_C, KEY_FIRST_MACRO + 10 },
    { KEY_V, KEY_FIRST_MACRO + 11 },


    { KEY_M, KEY_PAGE_UP },
    { KEY_COMMA, KEY_PAGE_DOWN },
    { KEY_PERIOD, KEY_LEFT_BRACE },
    { KEY_SLASH, KEY_RIGHT_BRACE },

    // don't overlay these
    { KEY_ESC, KEY_ESC },
    { MODIFIERKEY_LEFT_CTRL, MODIFIERKEY_LEFT_CTRL },
    { MODIFIERKEY_RIGHT_CTRL, MODIFIERKEY_RIGHT_CTRL },
    { KEY_LEFT_FN, KEY_LEFT_FN },
    { KEY_RIGHT_FN, KEY_RIGHT_FN },
    { MODIFIERKEY_RIGHT_SHIFT, MODIFIERKEY_RIGHT_SHIFT },
    { MODIFIERKEY_LEFT_SHIFT, MODIFIERKEY_LEFT_SHIFT },
    { MODIFIERKEY_LEFT_GUI, MODIFIERKEY_LEFT_GUI },
    { MODIFIERKEY_RIGHT_GUI, MODIFIERKEY_RIGHT_GUI },
    { MODIFIERKEY_LEFT_ALT, MODIFIERKEY_LEFT_ALT },
    { MODIFIERKEY_RIGHT_ALT, MODIFIERKEY_RIGHT_ALT },

    { KEY_J, KEY_UP },
    { KEY_K, KEY_DOWN },
    { KEY_H, KEY_LEFT },
    { KEY_L, KEY_RIGHT },

    // end of table marker
    { NO_CODE, NO_CODE }  // replace the second value with ANY_CODE to make overlay transparent
};

class AtreusApp : public KeyboardApp {

  public:

    appid_t id() { return ID; };
    static constexpr appid_t ID = "atre";

    const char* name() { return "Atreus Layout"; };

    EventPriority eventPriority() { return PRIORITY_EARLY; }

    virtual void init() override {
      updateMap();
    }

    virtual void setEnabled(bool e, AppMode asMode ) override {
      BritepadApp::setEnabled(e,asMode);
      updateMap();
    }

    bool event(KeyEvent* key) {
      if (getEnabled(KEYBOARD_MODE)) {
        // switch to the atreus overlay layer
        if (key->pressed(KEY_LEFT_FN) || key->pressed(KEY_RIGHT_FN)) {
          if (keys.getOverlay() != atreusOverlay) {
            keys.setOverlay(atreusOverlay);

            launcher.wakeHost();
            // jiggle the mouse to make cursor show up
            Mouse.move(-1, 0);
            Mouse.move(1, 0);
          }
        } else if (
                   (key->released(KEY_LEFT_FN) || key->released(KEY_RIGHT_FN))  // released fn key
                  ) {
                keys.setOverlay();
        }
      }
      return false;
    };
  protected:
    AppMode defaultEnabled() override { return NO_MODE; }

  private:
    void updateMap() {
      keys.setMap( getEnabled(KEYBOARD_MODE) ? atreusMap : nullptr);
    }
};

AtreusApp theAtreusApp;
