#include "WidgetGroup.h"
#include "BritepadShared.h"

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
    console.debugln("draw button");
    britepad.idle();
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

Widget* WidgetGroup::pressedWidget() {
  Widget* i = getNext();
  while (i) {
    if (i->pressed()) {
      break;
    } else {
      i = i->getNext();
    }
  }
  return i;
}

Widget* WidgetGroup::releasedWidget() {
  Widget* i = getNext();
  while (i) {
    if (i->released()) {
      break;
    } else {
      i = i->getNext();
    }
  }
  return i;
}

Widget* WidgetGroup::heldWidget() {
  Widget* i = getNext();
  while (i) {
    if (i->held()) {
      break;
    } else {
      i = i->getNext();
    }
  }
  return i;
}
