#include "BritepadShared.h"

#include "Britepad.h"
#include "Debug.h"
#include "Types.h"

Britepad::Britepad(void) {

}

void Britepad::addApp(BritepadApp* app) {
  if (appCount < maxApps) {
    apps[appCount++] = app;
  } else {
    DEBUG_LN("TOO MANY APPS!");
  }
}

BritepadApp* Britepad::getApp(int appIndex) {
  return (appIndex >= appCount) ? nil : apps[appIndex];
}
