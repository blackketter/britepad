#include "Widget.h"
#include "BritepadShared.h"

Widget::~Widget() {
  Widget* i = getNext();
  while (i) {
    Widget* n = i->getNext();
    i->setNext(nullptr);
    delete i;
    i = n;
  }
}

void Widget::addWidget(Widget* w) {
  Widget* old = getNext();
  setNext(w);
  w->setNext(old);
}

void Widget::draw() {
  if (!_visible) return;
  Widget* w = getNext();
  while (w) {
    w->draw();
    w = w->getNext();
    launcher.idle();
  }
};


Widget* Widget::getByID(widgetid_t id) {
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

Widget* Widget::pressedWidget() {
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

Widget* Widget::releasedWidget() {
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

Widget* Widget::heldWidget() {
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
