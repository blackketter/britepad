#include "SetTimerApp.h"

TimerApp SetTimerApp::timer_app;

SetTimerApp::SetTimerApp(const char* name, time_t duration) : BritepadApp() {
  timer_duration = duration;

  name_str = name;

}
