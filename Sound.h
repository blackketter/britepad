#ifndef _Sound_
#define _Sound_

#include "Types.h"

class Sound {
  public:
    const float MIDDLE_C_FREQ = 261.6;
    const float A440_FREQ = 440.0;

    Sound();
    void begin();

    void beep(millis_t ms = 20, float freq = 440);  // plays a generic beep tone

    void click();                           // plays a click sound
    void bump();                            // plays a bump sound
    void swipe(direction_t d);                    // plays a swipe sound

    void tone(float freq, float volume = 1.0);  // plays a sine tone, set volume to zero to end, may fade in/out to avoid clicks
    void noise(float volume = 1.0);             // plays some white noise, may fade in/out to avoid clicks, set volume to zero to end

    void setMute(bool mute);
    bool getMute() { return mainGain < 0.0; };
    float getVolume() { return mainGain; };
    void setVolume(float volume);

  private:
    float mainGain = 1.0;  // negative gains are mute
    float lastToneVolume = 0.0;
    int freeSynth(); // get a free synth

};

#endif

