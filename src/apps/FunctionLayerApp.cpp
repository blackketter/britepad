#include "KeyMatrix.h"
#include "KeyboardApp.h"
#include "KeyOverlays.h"
#include "BritepadShared.h"

class FunctionLayerApp : public KeyboardApp {

  public:

    appid_t id() { return ID; };
    static constexpr appid_t ID = "funl";
    const char* name() { return "Function Layer"; };

    EventPriority eventPriority() { return PRIORITY_EARLY; }
    bool event(Event* key) {
      if (getEnabled(KEYBOARD_MODE)) {
        // switch to the function layer
        if (key->pressed(KEY_LEFT_FN) || key->pressed(KEY_RIGHT_FN)) {
          if (keys.getOverlay() != functionOverlay) {
            console.debug("setting function layer\n");
            keys.setOverlay(functionOverlay);

            launcher.wakeHost();
          }
        } else if (
                   (key->released(KEY_LEFT_FN) || key->released(KEY_RIGHT_FN))  // released fn key
                  ) {
                console.debug("removing function layer\n");
                keys.setOverlay();
        }
      }
      return false;
    };
};

FunctionLayerApp theFunctionLayerApp;

class MouseLayerApp : public KeyboardApp {

  public:

    appid_t id() { return ID; };
    static constexpr appid_t ID = "moul";
    const char* name() { return "Mouse Layer"; };

    EventPriority eventPriority() { return PRIORITY_EARLY; }
    bool event(Event* key) {
      if (getEnabled(KEYBOARD_MODE)) {
        // switch to the function layer
        if (key->pressed(KEY_MOUSE)) {
          if (keys.getOverlay() != mouseOverlay) {
            console.debug("setting mouse layer\n");
            keys.setOverlay(mouseOverlay);

            launcher.wakeHost();
          }
        } else if (
                   (key->released(KEY_MOUSE))  // released fn key
                  ) {
                console.debug("removing mouse layer\n");
                keys.setOverlay();
        }
      }
      return false;
    };
};

MouseLayerApp theMouseLayerApp;
