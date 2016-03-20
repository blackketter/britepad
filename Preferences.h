#ifndef _Preferences_
#define _Preferences_

typedef const char* tag_t;

class Preferences {
  public:
    Preferences();

    void write(tag_t tag, uint8_t size, const uint8_t* data);
    uint8_t read(tag_t tag, uint8_t size, uint8_t* data);

    void resetPrefs();  // danger: this erases all the prefs

  private:
    static const size_t totalPrefsSize = 2048; // todo - we should be able to get this from EEPROM.h or the like
    uint8_t prefsData[totalPrefsSize];

    void saveOut();

    uint8_t version = 1;
    static constexpr tag_t versionTag = "vers";
    static constexpr tag_t endTag = "end";

    size_t writeTag(size_t offset, tag_t tag, uint8_t size, const uint8_t* data);
    size_t findTag(tag_t tag);
    size_t tagDataSize(size_t offset);
    void deleteTag(size_t start);
    size_t tagSize(size_t offset);

};

extern Preferences prefs;

#endif
