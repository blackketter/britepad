#ifndef _MacroApp_
#define _MacroApp_

#include "BritepadApp.h"

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

    appid_t id() { return ID; };
    static constexpr appid_t ID = "macr";
    const char* name() {
      String prefID(ID);
      prefID += "n";
      prefID += _macroNum;
      if (prefs.read(prefID, _name) == 0) {
        _name = "Macro ";
        _name += _macroNum;
      }
      return _name.c_str();
    };

    bool canBeInvisible() { return true; }
    BritepadApp* exitsTo() { return A_SCREENSAVER_APP; }

    color_t appButtonColor() {
      if (getMacroSize()) {
        return Screen::OrangeRed;
      } else {
        return screen.grey;
      }
    }

    void run() {
      String prefID(ID);
      prefID += _macroNum;
      size_t s = prefs.size(prefID.c_str());

      if (s) {
        int events = s/sizeof(macro_t);
        console.debugf("Sending macro of length %d key events\n", events);
        macro_t macro[events];

        prefs.read(prefID.c_str(), s, (uint8_t*)macro);

        for (int i = 0; i < events; i++) {
          keys.sendKey(macro[i].code, macro[i].pressed);
        }

      } else {
        console.debugln("No macro found");
      }
      exit();
    }

    size_t getMacroSize() {
      String prefID(ID);
      prefID += _macroNum;
      return prefs.size(prefID.c_str());
    }

    void startRecording() {
      if (_recording) {
        endRecording(false);
      }
      _recording = new macro_t[_maxEvents];
      _recordingEvent = 0;
      console.debugln("start recording");
    }

    bool recordEvent(KeyEvent* key) {
      if (_recordingEvent < _maxEvents-1) {
        _recording[_recordingEvent].code = key->code();
        _recording[_recordingEvent].pressed = key->pressed();
        _recordingEvent++;
        console.debugf("Recorded event %d (code:%d,pressed:%d)\n", _recordingEvent, key->code(), key->pressed());
        return true;
      } else {
        return false;
      }
    }

    void setName(String& name) {
      _name = name;
      String prefID(ID);
      prefID += "n";
      prefID += _macroNum;
      prefs.write(prefID, _name);
    }

    bool endRecording(bool save) {
      bool success = true;
      if (save && _recording) {
        String prefID(ID);
        prefID += _macroNum;
        success = prefs.write(prefID.c_str(), _recordingEvent*sizeof(macro_t), (uint8_t*)_recording);
      }
      if (_recording) {
        delete _recording;
        _recording = nullptr;
      }
      console.debugf("end recording %d events (save:%d) = success:%d\n", _recordingEvent, save,success);
      return success;
    }

  private:
    uint8_t _macroNum;
    macro_t* _recording = nullptr;
    int _recordingEvent = 0;
    String _name = "Macro ";
    static constexpr int _maxEvents = Preferences::MAX_TAG_SIZE/sizeof(macro_t);
};

#endif