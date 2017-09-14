#include "BritepadShared.h"
#include "KeyMatrix.h"
#include "KeyInfo.h"

void KeyMatrix::begin() {
  setMap();  // set to default map
  setLayout(); // set to default layout
}

keycode_t KeyMatrix::lookupOverlay(keycode_t c) {
  keycode_t to = ANY_CODE;
  if (_currentOverlay) {
    int i = 0;
    to = _currentOverlay[i].to;
    while (_currentOverlay[i].from != NO_CODE) {
      if (_currentOverlay[i].from == c) {
        break;
      }
      i++;
      to = _currentOverlay[i].to;
    }
  }
  if (to == ANY_CODE) { to = c; }
  return to;
}

keyswitch_t KeyMatrix::getSwitch(keycode_t c) {
  keyswitch_t i = 0;
  while (_currentMap[i].key != NO_KEY) {
    if (_currentMap[i].code == c) {
      return _currentMap[i].key;
    } else {
      i++;
    }
  }
  return NO_KEY;
}

keycode_t KeyMatrix::getCode(keyswitch_t k) {
  keyswitch_t i = 0;
  while (_currentMap[i].key != NO_KEY) {
    if (_currentMap[i].key == k) {
      return lookupOverlay(_currentMap[i].code);
    } else {
      i++;
    }
  }
  return NO_CODE;
}

const keylayout_t* KeyMatrix::getKeyLayout(keyswitch_t k) {
  keyswitch_t i = 0;
  while (_currentLayout[i].key != NO_KEY) {
    if (_currentLayout[i].key == k) {
      return (&_currentLayout[i]);
    }
    i++;
  }
  return nullptr;
}

uint8_t KeyMatrix::getWidth() {
  uint8_t maxWidth = 0;
  keyswitch_t i = 0;
  const keylayout_t* l = getDefaultLayout();
  while (l[i].key != NO_KEY) {
    uint8_t w = l[i].x+l[i].w;
    if (w > maxWidth) { maxWidth = w; }
    i++;
  }
  return maxWidth;
}

uint8_t KeyMatrix::getHeight() {
  uint8_t maxHeight = 0;
  keyswitch_t i = 0;
  const keylayout_t* l = getDefaultLayout();
  while (l[i].key != NO_KEY) {
    uint8_t h = l[i].y+l[i].h;
    if (h > maxHeight) { maxHeight = h; }
    i++;
  }
  return maxHeight;
}

uint8_t KeyMatrix::getKeyX(keyswitch_t k) {
  const keylayout_t* l = getKeyLayout(k);
  if (l) {
    return l->x;
  }
  return 0;
}

uint8_t KeyMatrix::getKeyY(keyswitch_t k) {
  const keylayout_t* l = getKeyLayout(k);
  if (l) {
    return l->y;
  }
  return 0;
}

uint8_t KeyMatrix::getKeyWidth(keyswitch_t k) {
  const keylayout_t* l = getKeyLayout(k);
  if (l) {
    return l->w;
  }
  return 0;
}
uint8_t KeyMatrix::getKeyHeight(keyswitch_t k) {
  const keylayout_t* l = getKeyLayout(k);
  if (l) {
    return l->h;
  }
  return 0;
}

int KeyMatrix::getKeyInfoIndex(keycode_t c) {
  int i = 0;
  while (keyInfo[i].code != NO_CODE) {
    if (keyInfo[i].code == c) {
      return i;
    }
    i++;
  }
  return i;
}

const keyinfo_t* KeyMatrix::getKeyInfo(keycode_t c) {

  int i = getKeyInfoIndex(c);

  if (keyInfo[i].code == NO_CODE) {
    return nullptr;
  } else {
    return &keyInfo[i];
  }
}

const icon_t KeyMatrix::getKeyIcon(keycode_t c) {
  const keyinfo_t* info = getKeyInfo(c);
  if (info) {
    return info->icon;
  } else {
    return 0;
  }
}

modifierkey_t KeyMatrix::getKeyModifier(keycode_t c) {
  const keyinfo_t* info = getKeyInfo(c);
  if (info) {
    return info->modifier;
  } else {
    return false;
  }
}

const char* KeyMatrix::getKeyLabel(keycode_t c) {
  const keyinfo_t* info = getKeyInfo(c);
  if (info) {
    return info->label;
  } else {
    return 0;
  }
}

void KeyMatrix::setLayout(const keylayout_t* l) {
  _currentLayout = l ? l : getDefaultLayout();
}

void KeyMatrix::setMap(const keymap_t* l) {
  _currentMap = l ? l : getDefaultMap();
}

class KeysCommand : public Command {
  public:
    const char* getName() { return "keys"; }
    const char* getHelp() { return "display status of key matrix"; }
    void execute(Stream* c, uint8_t paramCount, char** params) {
      keys.printStatus(c);
    }
};

KeysCommand theKeysCommand;

void KeyMatrix::printStatus(Stream* c) {
  if (c == nullptr) { c = &console; }
  c->println("---------------");
  c->println("Keyboard Status:");
  int i = 0;
  keyswitch_t k = getNthKey(i);
  while (k != NO_KEY) {
    if (switchIsDown(k)) {
      c->printf(" Key '%s' is down (switch: %d)\n", getKeyLabel(getCode(k)), k);
    }
    i++;
    k = getNthKey(i);
  }
  c->println("");
}

