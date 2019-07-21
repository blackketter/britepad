#ifndef _MacroApp_
#define _MacroApp_

#include "BritepadApp.h"
#include "widgets/AppButton.h"
#include "widgets/TextField.h"

typedef struct macro_t {
  keycode_t code;
  bool pressed;
} macro_t;

class MacroApp : public BritepadApp {
  public:
    MacroApp( uint8_t num ) {
      _macroNum = num;
    }

    uint8_t getNum() { return _macroNum; }
    AppType getAppType() { return MACROS_APP; }
    virtual int32_t getLauncherPosition() { return _macroNum; }
    bool disablesScreensavers() { return isRecording() || _namefield; };
    bool usesKeyboard() { return _namefield != nullptr; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "macr";
    const char* name() {
      String prefID(ID);
      prefID += "n";
      prefID += _macroNum;
      if (prefs.get(prefID, _name) == 0) {
        _name = "Macro ";
        _name += _macroNum;
      }
      return _name.c_str();
    };

    bool canBeInvisible() { return true; }
    bool canBeSetup() { return true; }

    color_t appButtonColor() {
      if (getMacroSize()) {
        return Screen::OrangeRed;
      } else {
        return screen.grey;
      }
    }

    void begin(AppMode asMode) {
      BritepadApp::begin(asMode);
      if (asMode == SETUP_MODE) {
        _message = new TextField(screen.clipLeft(), screen.clipTop() + screen.clipHeight()/6, screen.clipWidth(), screen.clipHeight()/3,
              &Arial_16, screen.white, screen.black, (alignment_t)(ALIGN_CENTER+ALIGN_VCENTER));
        _message->draw("Type new macro now.\nTap screen to finish.");
        startRecording();
      } else {
        cleanup();
      }
    }

    void cleanup() {
      endRecording(false);  // cancel the recording
      if (_namefield) { delete(_namefield); _namefield = nullptr; }
      if (_message) { delete(_message); _message = nullptr; }
    }

    void end() {
      cleanup();
      BritepadApp::end();
    }

    void run() {
      if (getAppMode() == SETUP_MODE) {
        runSetup();
      } else {
        sendMacro();
        exit();
      }
    }

  private:
    void sendMacro() {
      String prefID(ID);
      prefID += _macroNum;
      size_t s = prefs.size(prefID.c_str());

      if (s) {
        int events = s/sizeof(macro_t);
        console.debugf("Sending macro of length %d key events\n", events);
        macro_t macro[events];

        prefs.get(prefID.c_str(), s, (uint8_t*)macro);
        millis_t keytime = Uptime::millis();
        for (int i = 0; i < events; i++) {
          keyEvents.addEvent(nullptr, NO_KEY, macro[i].code, keytime, macro[i].pressed);
          keytime += 25;  // delay each key
        }
        while (keyEvents.peekNextEvent()) {
          britepad.idle();
        }
      } else {
        console.debugln("No macro found");
      }
    }

    void runSetup() {
      if (isRecording()) {
        if (pad.pressed()) {
          endRecording(true);
          _message->draw("Macro name:");
          _namefield = new TextField(screen.clipLeft() + screen.clipWidth()/4, screen.clipTop() + screen.clipHeight()/3*2, screen.clipWidth()/2, screen.clipHeight()/6,
                &Arial_16, screen.white, screen.darkergrey, (alignment_t)(ALIGN_CENTER+ALIGN_VCENTER));
          _namefield->setMaxTextLength(10);
          _namefield->setText(_name);
          _namefield->draw();
        }
      } else if (_namefield) {
          KeyEvent* key = getNextEvent();
          _namefield->key(key);
          if ((key && key->pressed(KEY_RETURN)) || pad.pressed()) {
             String name;
            _namefield->getText(name);
            setName(name);
            exit();
          }
      } else {
         _message->draw("Type the new macro now.\nTap screen to finish.");
        startRecording();
      }
    }

    size_t getMacroSize() {
      String prefID(ID);
      prefID += _macroNum;
      return prefs.size(prefID.c_str());
    }

    void startRecording() {
      endRecording(false);
      _recordingKeys = new macro_t[_maxEvents];
      _recordingEvent = 0;
      console.debugln("start recording");
    }

    EventPriority eventPriority() { return PRIORITY_NORMAL; }

    bool event(KeyEvent* key) {
      bool consume = false;
      if (isCurrentApp() && isRecording()) {
        recordEvent(key);
      } else if (key->code(KEY_FIRST_MACRO + _macroNum)) {
        if (key->pressed()) {
          sendMacro();
          //consume = true;
        }
      }
      return consume;
    }

    bool recordEvent(KeyEvent* key) {
      if (_recordingEvent < _maxEvents-1) {
        _recordingKeys[_recordingEvent].code = key->code();
        _recordingKeys[_recordingEvent].pressed = key->pressed();
        _recordingEvent++;
        console.debugf("Recorded event %d (code:%d,pressed:%d)\n", _recordingEvent, key->code(), key->pressed());
        return true;
      } else {
        return false;
      }
    }

    bool endRecording(bool save) {
      bool success = true;
      if (save && _recordingKeys) {
        String prefID(ID);
        prefID += _macroNum;
        success = prefs.set(prefID.c_str(), _recordingEvent*sizeof(macro_t), (uint8_t*)_recordingKeys);
        console.debugf("end recording %d events (save:%d) = success:%d\n", _recordingEvent, save,success);
      }
      if (_recordingKeys) {
        delete _recordingKeys;
        _recordingKeys = nullptr;
      }
      return success;
    }

    bool isRecording() { return _recordingKeys; }

    void setName(String& name) {
      _name = name;
      String prefID(ID);
      prefID += "n";
      prefID += _macroNum;
      prefs.set(prefID, _name);
    }

    uint8_t _macroNum;
    String _name = "Macro ";

    // these are static because only one macro can be recording at a time
    static constexpr int _maxEvents = Dictionary::MAX_ENTRY_SIZE/sizeof(macro_t);
    static TextField* _message;
    static TextField* _namefield;
    static macro_t* _recordingKeys;
    static int _recordingEvent;

};

#endif
