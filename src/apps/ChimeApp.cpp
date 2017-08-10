 #include "BritepadApp.h"

class ChimeApp : public BritepadApp {

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

    const char* name() { return _chimeEnabled ? "Chime On" : "Chime Off"; };

    virtual bool highlighted() { return !_chimeEnabled; }
    bool canBeInvisible() { return true; };
    AppType getAppType() { return SETTINGS_APP; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "chim";

    void timeAdjusted() { resetChime(); }
    void resetChime();
    void enableChime(bool enabled) { enabled ? resetChime() : _chimeTimer.cancel(); };
    void chimerCallback();

  private:

    uint8_t _chimeEnabled;
    CallbackTimer _chimeTimer;
    int _chimesRemaining = 0;
    static const millis_t _chimeInterval = 500;
};

ChimeApp theChimeApp;

void chimeCallback(void* data) {
  ((ChimeApp*)data)->chimerCallback();
}

void ChimeApp::resetChime() {
  if (clock.hasBeenSet() && _chimeEnabled) {

    time_t nextChime = ((clock.now() / Time::secsPerHour)+1) * Time::secsPerHour;
    _chimeTimer.setClockTime( nextChime, chimeCallback, (void*)this);

    // how many chimes at the next hour
    _chimesRemaining = clock.hourFormat12() + 1;
    if (_chimesRemaining == 13) { _chimesRemaining = 1; }
    console.debugf("%d chimes set up\n", _chimesRemaining);
  } else {
    console.debugln("chime not set up");
  }
}


void ChimeApp::chimerCallback() {
  console.debugf("Chimes: %d\n", _chimesRemaining);
  if (_chimesRemaining == 0) {
    resetChime();
  } else {
    _chimeTimer.setMillis(_chimeInterval, chimeCallback, (void*)this);
    sound.beep();
    _chimesRemaining--;
  }
}
