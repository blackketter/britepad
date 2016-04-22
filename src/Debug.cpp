#include "Debug.h"
#include <stdarg.h>
#include <stdio.h>


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

#if DEBUG_ON

void debugprintf(const char* format, ...) {
  char foo[500];
  va_list argptr;
  va_start(argptr, format);
  vsprintf(foo, format, argptr);
  va_end(argptr);
  Serial.print(foo);
}

void debugf(const char* format, ...) {
  char foo[500];

  Serial.print(::millis());
  Serial.print(": ");
  va_list argptr;
  va_start(argptr, format);
  vsprintf(foo, format, argptr);
  va_end(argptr);
  Serial.print(foo);
};

#else

void debugf(const char* format, ...) {
}

#endif
