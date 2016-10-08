#include "Hardware.h"

#ifdef TEENSYDUINO
extern volatile uint8_t usb_configuration;

bool usbActive() {
  return usb_configuration;
};

uint32_t FreeMem(){ // for Teensy 3.0
    uint32_t stackTop;
    uint32_t heapTop;

    // current position of the stack.
    stackTop = (uint32_t) &stackTop;

    // current position of heap.
    void* hTop = malloc(1);
    heapTop = (uint32_t) hTop;
    free(hTop);

    // The difference is (approximately) the free, available ram.
    return stackTop - heapTop;
}

#else
bool usbActive() { return false; }
#endif
