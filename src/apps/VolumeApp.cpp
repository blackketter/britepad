#include "BritepadApp.h"
#include "BritepadShared.h"
#include "Sound.h"

const float _volumeLevels[] = {1.0, 0.0, 0.25, 0.5, 0.75 };
const char* _volumeNames[] = {"Volume 100%", "Volume Mute", "Volume 25%", "Volume 50%", "Volume 75%"};
const uint8_t _volumeLevelSteps = 5;

class VolumeApp : public BritepadApp {

  public:
    void init() {
      prefs.get(id(), sizeof(_volumeLevel), &_volumeLevel);
      sound.setVolume(_volumeLevels[_volumeLevel]);
    };

    void run() {
      _volumeLevel++;
      if (_volumeLevel >= _volumeLevelSteps) {
        _volumeLevel = 0;
      }

      sound.setVolume(_volumeLevels[_volumeLevel]);

      prefs.set(id(), sizeof(_volumeLevel), &_volumeLevel);

      sound.beep();
    }

    const char* name() { return _volumeNames[_volumeLevel]; }

    bool canBeInvisible() { return true; };

    AppType getAppType() { return SETTINGS_APP; }

    appid_t id() { return ID; };

    static constexpr appid_t ID = "volu";

  private:

    uint8_t _volumeLevel = 0;
};

VolumeApp theVolumeApp;
