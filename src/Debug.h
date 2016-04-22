#ifndef _DEBUG_
#define _DEBUG_

#include <Arduino.h>

// Global debug printing control
#ifndef DEBUG_ON
#define DEBUG_ON 1
#endif

void debugf(const char* format, ...);
void debugprintf(const char* format, ...);
uint32_t FreeMem();

#if DEBUG_ON
#define DEBUGF debugf
#define PRINTF debugprintf
#define DEBUG_INIT() Serial.begin(115200)
#define DEBUG_LN(x) DEBUGF(x "\n")
#else
#define DEBUGF(x)
#define PRINTF(x)
#define DEBUG_INIT()
#define DEBUG_LN(x)
#endif

#endif

