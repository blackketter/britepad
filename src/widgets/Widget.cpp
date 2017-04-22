#include "Widget.h"

void Widget::add(Widget* w) {
  Widget* old = getNext();
  setNext(w);
  w->setNext(old);
  if (old) {
    old->setPrevious(w);
  }
}

void Widget::remove(Widget* w) {
  Widget* i = getNext();

  while (i) {
    if (i == w) {
      Widget* oldNext = w->getNext();
      Widget* oldPrev = w->getPrevious();
      if (oldNext) {
        oldNext->setPrevious(oldPrev);
      }
      if (oldPrev) {
        oldPrev->setNext(oldNext);
      }
      break;
    }
  }
};

