#include "BritepadEventQueue.h"
#include "BritepadShared.h"

void BritepadEventQueue::processEvent(Event* e) {
  if (launcher.event(e))  {
    removeEvent(e);
  }
}
