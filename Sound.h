#ifndef _SOUND_
#define _SOUND_

class Sound {
  public:
    Sound(void);
    void beep(long ms = 20, long freq = 440);
    bool getMute(void) { return mainGain < 0.0; };
    void setMute(bool mute);
    float getVolume(void) { return mainGain; };
    void setVolume(float volume);

  private:
    float mainGain = 1.0;  // negative gains are mute

};

#endif

