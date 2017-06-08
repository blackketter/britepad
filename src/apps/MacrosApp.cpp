#include "KeyMatrix.h"
#include "BritepadApp.h"
#include "MacroApp.h"
#include "widgets/ButtonMatrix.h"
#include "widgets/AppButton.h"
#include "widgets/TextField.h"

class MacrosApp : public BritepadApp {
  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "mcor";
    const char* name() { return "Macros"; };
    AppType getAppType() { return KEY_APP; }
    bool disablesScreensavers() { return true; };
    const char* infoBarText() { return "Press and hold to edit"; }

    void init() {
      for (int i = 0; i < 10; i++) {
        new MacroApp(i);  // 10 new macro apps get spawned off by the MacrosApp.
      }
    }

    void begin(AppMode asMode) {
      _buttons = new ButtonMatrix(screen.clipLeft(),screen.clipTop(),screen.clipWidth(),screen.clipHeight(),v_buttons,h_buttons,1);

      BritepadApp* currApp = nullptr;

      for (int i = 0; i < (v_buttons*h_buttons); i++) {

        do {
          currApp = britepad.getNextApp(currApp);
        } while (currApp && !currApp->isID(MacroApp::ID));

        if (currApp == nullptr) {
          break;
        }
        Button* b = new AppButton(currApp);
        _buttons->setButton(b,i);
      }

      BritepadApp::begin(asMode);
      drawButtons();
    }

    void end() {
      if (_recording) {
        _recording->endRecording(false);  // cancel the recording
        _recording = nullptr;
      }
      delete(_buttons);
      BritepadApp::end();
    }

    void idle(KeyEvent* key) {
      if (_recording) {
        _recording->recordEvent(key);
      }
    }

    void run() {
      if (_recording) {
        if (pad.pressed()) {
          _recording->endRecording(true);
          _recording = nullptr;
          sound.beep();
          drawButtons();
        }
      } else {
        AppButton* b = (AppButton*)(_buttons->releasedButton());
        if (b) {
          if (((MacroApp*)b->getApp())->getMacroSize()) {
            launchApp(b->getApp());
          }
        } else {
          b =  (AppButton*)(_buttons->heldButton());
          if (b) {
            clearScreen();
            _recording = ((MacroApp*)b->getApp());
            _recording->startRecording();
          }
        }
      }
    }

  private:
    void drawButtons() {
      _buttons->setBounds(screen.clipLeft(), screen.clipTop(), screen.clipWidth(), screen.clipHeight());
      _buttons->draw();
    }

    ButtonMatrix* _buttons = nullptr;
    TextField* _message = nullptr;
    TextField* _namefield = nullptr;
    MacroApp* _recording = nullptr;
    const int v_buttons = 3;
    const int h_buttons = 4;


};

MacrosApp theMacrosApp;
