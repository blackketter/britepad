#include "BritepadKeyEventQueue.h"
#include "BritepadShared.h"

void BritepadKeyEventQueue::processEvent(KeyEvent* e) {
  if (britepad.event(e))  {
    removeEvent(e);
  }
}
