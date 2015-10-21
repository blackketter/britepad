#include "InfoApp.h"

uint32_t FreeRam(){ // for Teensy 3.0
    uint32_t stackTop;
    uint32_t heapTop;

    // current position of the stack.
    stackTop = (uint32_t) &stackTop;

    // current position of heap.
    void* hTop = malloc(1);
    heapTop = (uint32_t) hTop;
    free(hTop);

    // The difference is the free, available ram.
    return stackTop - heapTop;
}

void InfoApp::run(void) {

  BritepadApp::run();

  screen.setTextColor(screen.white, screen.black);
  screen.setFont(Arial_16);
  screen.setCursor(screen.clipLeft() + screen.fontLineSpacing(), screen.clipTop()+screen.fontLineSpacing());
  screen.drawTextF("X: %d Y: %d        \n", pad.x(), pad.y());
  screen.drawTextF("Points captured: %d        \n", pad.getHistoryCount());
  screen.drawTextF("Proximity: %d    \n", pad.getProximityDistance());
  screen.drawTextF("Ambient: %d         \n", pad.getAmbientLight());
  screen.drawTextF("Free ram: %d            \n", FreeRam());
  screen.drawTextF("Uptime: %d\n", (long)(clock.millis()/1000));
  screen.drawTextF("FPS: %d      ", 1000/(pad.time()-lastUpdate));
  lastUpdate = pad.time();
};
