#ifndef _Sound_
#define _Sound_

#include "Clock.h"
#include "Screen.h"

typedef const uint8_t* tune_t;

class Sound {
  public:
    static constexpr float MIDDLE_C_FREQ = 261.6;
    static constexpr float A440_FREQ = 440.0;

    Sound();
    void idle();
    void begin();

    void beep(millis_t ms = 20, float freq = 440);  // plays a generic beep tone
    void bell(millis_t ms = 1000, float freq = 440); // plays a bell sound
    void click(float loudness = 0.5);               // plays a click sound
    void bump();                                    // plays a bump sound
    void swipe(direction_t d);                      // plays a swipe sound

    void tone(float freq, float volume = 1.0);  // plays a sine tone, set volume to zero to end, may fade in/out to avoid clicks (only one tone at a time)
    void noise(float volume = 1.0);             // plays some white noise, may fade in/out to avoid clicks, set volume to zero to end

    void setMute(bool mute);
    bool getMute() { return mainGain < 0.0; };

    float getVolume() { return mainGain; };
    void setVolume(float volume);

    void tunePlay(const tune_t tuneBytes);  // pass nullptr to stop the tune.
    void tuneTempo(float t) { tempo = t; }  // tempo is multiplier against timing in tuneBytes, i.e. 2.0 plays twice as fast
    void tuneVolume(float v);
    void tuneTranspose(int8_t k) { transpose = k; }
    void tuneRepeat(bool r) { repeat = r; }

  private:
    void updateMainGain();
    float mainGain = 1.0;  // negative gains are muted

    float lastToneVolume = 0.0;
    float lastToneFreq = 0.0;
    int   toneSynth = -1;

    int freeSynth(); // get a free synth

    void tunePlayerBegin();
    void tunePlayerIdle();
    tune_t curTune = nullptr;
    int curTuneIndex;
    float tempo;
    bool repeat = false;
    millis_t nextTime;
    int8_t transpose;
};

#endif

