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
    bool disablesScreensavers() { return _recording || _namefield; };
    const char* infoBarText() { return _infobartext; }
    bool usesKeyboard() { return !_recording; }
    App* exitsTo() override { return A_SCREENSAVER_APP; }


    void init() {
      for (int i = 0; i < _totalMacros; i++) {
        new MacroApp(i);  // 10 new macro apps get spawned off by the MacrosApp.
      }
    }

    void begin(AppMode asMode) {
      _message = new TextField(screen.clipLeft(), screen.clipTop() + screen.clipHeight()/6, screen.clipWidth(), screen.clipHeight()/3,
            &Arial_16, screen.white, screen.black, (alignment_t)(ALIGN_CENTER+ALIGN_VCENTER));
      BritepadApp::begin(asMode);
      loadButtons();
    }

    void end() {
      if (_recording) {
        _macroapp->endRecording(false);  // cancel the recording
        _recording = false;
      }
      if (_buttons) { delete(_buttons); _buttons = nullptr; }
      if (_namefield) { delete(_namefield); _namefield = nullptr; }
      if (_message) { delete(_message); _message = nullptr; }
      BritepadApp::end();
    }

    void idle(KeyEvent* key) {
      if (_recording) {
        _macroapp->recordEvent(key);
      } else if (key->pressed(_launchkey)) {
        launch();
      }
    }

    void run() {
      if (_recording) {
        if (pad.pressed()) {
          _macroapp->endRecording(true);
          _recording = false;
          _message->draw("Macro name:");
          _namefield = new TextField(screen.clipLeft() + screen.clipWidth()/4, screen.clipTop() + screen.clipHeight()/3*2, screen.clipWidth()/2, screen.clipHeight()/6,
                &Arial_16, screen.white, screen.darkergrey, (alignment_t)(ALIGN_CENTER+ALIGN_VCENTER));
          _namefield->setMaxTextLength(10);
          _namefield->draw();
        }
      } else if (_namefield) {
          KeyEvent* key = getNextEvent();
          _namefield->key(key);
          if ((key && key->pressed(KEY_RETURN)) || pad.pressed()) {
             String name;
            _namefield->getText(name);
            _macroapp->setName(name);
            if (_namefield) { delete(_namefield); _namefield = nullptr; }
            _message->draw("");
            loadButtons();
          }
      } else {

        KeyEvent* key = getNextEvent();
        if (key) {
          if (key->pressed(_launchkey)) {
            exit();
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
              b->setHighlighted(false);

              clearScreen();
              setInfoBarText(" ");
              _message->draw("Type new macro now.\nTap screen to finish.");
              _recording = true;
              _macroapp = ((MacroApp*)b->getApp());
              _macroapp->startRecording();
            }
          }
        }
      }
    }

  private:
    void drawButtons() {
      clearScreen();
      setInfoBarText("Press and hold to record");
      _buttons->setBounds(screen.clipLeft(), screen.clipTop(), screen.clipWidth(), screen.clipHeight());
      _buttons->draw();
    }

    void loadButtons() {
      if (_buttons) { delete(_buttons); _buttons = nullptr; }
      _buttons = new ButtonMatrix(screen.clipLeft(),screen.clipTop(),screen.clipWidth(),screen.clipHeight(),_v_buttons,_h_buttons,1);

      BritepadApp* currApp = nullptr;

      for (int i = 0; i < (_totalMacros); i++) {

        do {
          currApp = currApp->getNextApp();
        } while (currApp && !currApp->isID(MacroApp::ID));

        if (currApp == nullptr) {
          break;
        }
        Button* b = new AppButton(currApp);
        _buttons->setButton(b,((MacroApp*)currApp)->getNum());
      }
      drawButtons();
    }

    void setInfoBarText(const char* msg) {
      _infobartext = msg;
      drawInfoBar();
    }

    ButtonMatrix* _buttons = nullptr;
    TextField* _message = nullptr;
    TextField* _namefield = nullptr;
    const char* _infobartext = "";
    bool _recording = false;
    MacroApp* _macroapp;
    const int _v_buttons = 3;
    const int _h_buttons = 4;
    const uint8_t _totalMacros = _v_buttons * _h_buttons;
    const keycode_t _launchkey = KEY_RIGHT_FN;


};

MacrosApp theMacrosApp;
