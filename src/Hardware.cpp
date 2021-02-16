#include "Hardware.h"

#if defined(__MK66FX1M0__) && !defined(TEENSYDEBUG)
extern volatile uint8_t usb_configuration;

bool usbActive() {
  return usb_configuration;
};

void watchdogKick() {
  noInterrupts();
  WDOG_REFRESH = 0xA602;
  WDOG_REFRESH = 0xB480;
  interrupts();
};

const char* resetType() {
  if (RCM_SRS1 & RCM_SRS1_SACKERR)   return("Stop Mode Acknowledge Error Reset");
  if (RCM_SRS1 & RCM_SRS1_MDM_AP)    return("MDM-AP Reset");
  if (RCM_SRS1 & RCM_SRS1_SW)        return("Software Reset");
  if (RCM_SRS1 & RCM_SRS1_LOCKUP)    return("Core Lockup Event Reset");
  if (RCM_SRS0 & RCM_SRS0_POR)       return("Power-on Reset");
  if (RCM_SRS0 & RCM_SRS0_PIN)       return("External Pin Reset");
  if (RCM_SRS0 & RCM_SRS0_WDOG)      return("Watchdog(COP) Reset");
  if (RCM_SRS0 & RCM_SRS0_LOC)       return("Loss of External Clock Reset");
  if (RCM_SRS0 & RCM_SRS0_LOL)       return("Loss of Lock in PLL Reset");
  if (RCM_SRS0 & RCM_SRS0_LVD)       return("Low-voltage Detect Reset");
  return "Reset cause unknown";
};

#ifdef __cplusplus
extern "C" {
void startup_early_hook();
}
extern "C" {
#endif //__cplusplus

  void startup_early_hook() {
    // clock source 0 LPO 1khz, 4 s timeout
    WDOG_TOVALL = WATCHDOG_TIME_MS; // The next 2 lines sets the time-out value. This is the value that the watchdog timer compare itself to.
    WDOG_TOVALH = 0;
    WDOG_STCTRLH = (WDOG_STCTRLH_ALLOWUPDATE | WDOG_STCTRLH_WDOGEN | WDOG_STCTRLH_WAITEN | WDOG_STCTRLH_STOPEN); // Enable WDG
    WDOG_PRESC = 0; // prescaler
  }
#ifdef __cplusplus
}
#endif

#else
bool usbActive() { return false; }
void watchdogKick() {};
const char* resetType() { return"Reset cause unknown";};
#endif
