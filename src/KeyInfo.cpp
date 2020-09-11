#include "KeyInfo.h"

int getKeyInfoIndex(keycode_t c) {
  int i = 0;
  while (keyInfo[i].code != NO_CODE) {
    if (keyInfo[i].code == c) {
      return i;
    }
    i++;
  }
  return i;
}

const keyinfo_t* getKeyInfo(keycode_t c) {

  int i = getKeyInfoIndex(c);

  if (keyInfo[i].code == NO_CODE) {
    return nullptr;
  } else {
    return &keyInfo[i];
  }
}

const icon_t getKeyIcon(keycode_t c) {
  const keyinfo_t* info = getKeyInfo(c);
  if (info) {
    return info->icon;
  } else {
    return 0;
  }
}

modifierkey_t getKeyModifier(keycode_t c) {
  const keyinfo_t* info = getKeyInfo(c);
  if (info) {
    return info->modifier;
  } else {
    return false;
  }
}

const char* getKeyLabel(keycode_t c) {
  const keyinfo_t* info = getKeyInfo(c);
  if (info) {
    return info->label;
  } else {
    return 0;
  }
}
