#include <Arduino.h>
#include <EEPROM.h>
#include "Preferences.h"

#include "Debug.h"

// todo: handle error condition when prefs are too big

// preferences are saved:
//   tag string (zero terminated), length byte, data bytes (of length given before)

Preferences::Preferences() {
  // todo - only read in the bytes that have been used
  for (size_t i = 0; i < sizeof(prefsData); i++) {
    prefsData[i] = EEPROM.read(i);
  }

  // check if prefs have been initialized
  if (strcmp((const char*)versionTag, (const char*)prefsData) != 0 ||
      prefsData[strlen(versionTag) + 1] != sizeof(version) ||
      prefsData[strlen(versionTag) + 2] != version) {
    resetPrefs();
  }
}

void Preferences::write(tag_t tag, uint8_t size, const uint8_t* data) {

  bool changed = false;
  size_t offset = findTag(tag);

  if (offset) {
    // found an old tag
    if (tagDataSize(offset) == size) {
      // same size so we can write in place
      writeTag(offset, tag, size, data);
      changed = true;
    } else {
      // delete pref if the new size is not the same as the existing one
      deleteTag(offset);
      offset = 0;
      changed = true;
    }
  }

  // if it isn't already written and it has a size, we'll tack it on to the end
  if (!offset && size) {
    // append to the end
    offset = findTag(endTag);
    offset = writeTag(offset, tag, size, data);
    offset = writeTag(offset, endTag, sizeof(version), &version);
    changed = true;
  }

  // save it out if something's been written
  if (changed) {
    saveOut();
  }
}

uint8_t Preferences::read(tag_t tag, uint8_t size, uint8_t* data) {

  size_t offset = findTag(tag);

  if (offset) {
    offset += strlen(tag) + 1;
    uint8_t readSize = prefsData[offset];
    offset++;
    readSize = min(readSize, size);  // if the sizes don't match, choose the smaller
    memcpy(data, &prefsData[offset], readSize);

    return readSize;
  } else {
    return 0;  // not found
  }
}


void Preferences::deleteTag(size_t start) {
  // delete chosen tag and close the gap

  // don't delete the version tag
  if (start == 0) {
    return;
  }

  // close the gap
  size_t end = findTag(endTag);

  size_t nextTag = tagSize(start) + start;

  if (nextTag < end) {
    memcpy(&prefsData[start], &prefsData[nextTag], end - nextTag + tagSize(end));
  }

}

size_t Preferences::tagSize(size_t offset) {
  size_t endoffset = offset;
  endoffset += strlen((const char*)&prefsData[endoffset]) + 1;
  endoffset += prefsData[endoffset];
  endoffset++;

  return endoffset - offset;
}

size_t Preferences::tagDataSize(size_t offset) {
  size_t endoffset = offset;
  endoffset += strlen((const char*)&prefsData[endoffset]) + 1;
  return prefsData[endoffset];
}

size_t Preferences::findTag(tag_t tag) {
  size_t offset = 0;
  while (offset < totalPrefsSize) {
    if (strcmp(tag, (const char*)&prefsData[offset]) == 0) {
      return offset;
    } else if (strcmp(endTag, (const char*)&prefsData[offset]) == 0) {
      return 0;  // hit the end tag, so it's not found
    } else {
      offset += strlen((const char*)&prefsData[offset]) + 1;
      offset += prefsData[offset] + 1;
    }
  }

  // went past the end of the data structure, we are corrupted.
  resetPrefs();
  return 0;
}

void Preferences::saveOut() {
  // this is brute force:
  // todo - only write out the bytes that have changed
  for (size_t i = 0; i < sizeof(prefsData); i++) {
    EEPROM.write(i, prefsData[i]);
#if 0
    char dumper[100];
    sprintf(dumper, "%4d - %c (%2x)", i, prefsData[i], prefsData[i]);
    DEBUG_LN(dumper);
#endif
  }
  DEBUGF("Wrote out prefs of size: %d\n", findTag(endTag) + tagSize(findTag(endTag)));
}

size_t Preferences::writeTag(size_t offset, tag_t tag, uint8_t size, const uint8_t* data) {
    strcpy((char*)&prefsData[offset], tag);
    offset += strlen(tag) + 1;

    prefsData[offset] = size;
    offset++;

    memcpy(&prefsData[offset], data, size);
    offset+= size;

    return offset;
}

void Preferences::resetPrefs() {
    // initial version tag is missing, clear out prefs
    size_t offset = 0;
    // write a version tag & 1 byte version
    offset = writeTag(offset, versionTag, sizeof(version), &version);

    // write an end tag with the same 1 byte version
    offset = writeTag(offset, endTag, sizeof(version), &version);
    saveOut();
}
