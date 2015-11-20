#ifndef _Preferences_
#define _Preferences_

#include "Types.h"

class Preferences {
  public:
    Preferences();

    void write(appid_t tag, uint8_t size, const uint8_t* data);
    uint8_t read(appid_t tag, uint8_t size, uint8_t* data);

  private:
    static const size_t totalPrefsSize = 2048; // todo - we should be able to get this from EEPROM.h or the like
    uint8_t prefsData[totalPrefsSize];

    void saveOut();
    void resetPrefs();

    uint8_t version = 1;
    static constexpr appid_t versionTag = "vers";
    static constexpr appid_t endTag = "end";

    size_t writeTag(size_t offset, appid_t tag, uint8_t size, const uint8_t* data);
    size_t findTag(appid_t tag);
    size_t tagDataSize(size_t offset);
    void deleteTag(size_t start);
    size_t tagSize(size_t offset);

};

extern Preferences prefs;

#endif
