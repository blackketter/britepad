#ifndef _BritepadKeyEventQueue_
#define _BritepadKeyEventQueue_
#include "KeyEventQueue.h"


class BritepadKeyEventQueue : public KeyEventQueue {
  public:
    void processEvent(KeyEvent* e) override;
};

extern BritepadKeyEventQueue keyEvents;

#endif
