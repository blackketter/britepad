#include "WidgetGroup.h"

WidgetGroup::~WidgetGroup() {
  Widget* i = getNext();
  while (i) {
    Widget* n = i->getNext();
    delete i;
    i = n;
  }
};

void WidgetGroup::draw() {
  if (!_visible) return;
  Widget* w = getNext();
  while (w) {
    w->draw();
    w = w->getNext();
  }
};


Widget* WidgetGroup::getByID(widgetid_t id) {
  Widget* i = getNext();
  while (i) {
    if (i->getID() == id) {
      break;
    } else {
      i = i->getNext();
    }
  }
  return i;
};

