#ifndef _ChimeApp_
#define _ChimeApp_

#include "MuteApp.h"

class ChimeApp : public MuteApp {

  public:
    void init() {
      prefs.read(id(), sizeof(_chimeEnabled), &_chimeEnabled);
      resetChime();
    };

    void run() {
      _chimeEnabled = !_chimeEnabled;
      resetChime();
      prefs.write(id(), sizeof(_chimeEnabled), &_chimeEnabled);
   }

    const char* name() { return _chimeEnabled ? "Chime\nOn" : "Chime\nOff"; };

    virtual bool highlighted() { return !_chimeEnabled; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "chim";

    void resetChime();
    void enableChime(bool enabled) { enabled ? resetChime() : _chimeTimer.cancel(); };
    void chimerCallback();

  private:

    uint8_t _chimeEnabled;
    Timer _chimeTimer;
    int _chimesRemaining = 0;
    static const millis_t _chimeInterval = 500;
};

#endif

