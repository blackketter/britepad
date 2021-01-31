#include "BritepadKeyEventQueue.h"
#include "BritepadShared.h"

void BritepadKeyEventQueue::processEvent(KeyEvent* e) {
  if (launcher.event(e))  {
    removeEvent(e);
  }
}
