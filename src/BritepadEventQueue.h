#ifndef _BritepadEventQueue_
#define _BritepadEventQueue_
#include "EventQueue.h"


class BritepadEventQueue : public EventQueue {
  public:
    void processEvent(Event* e) override;
};

#endif
