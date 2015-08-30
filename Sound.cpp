#include "Sound.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include "Debug.h"

// from http://www.pjrc.com/teensy/gui/

AudioSynthWaveform       waveform1;      //xy=704,441
AudioEffectEnvelope      envelope1;      //xy=889,400
AudioMixer4              mixer1;         //xy=1055,488
AudioOutputAnalog        dac1;           //xy=1301,378
AudioConnection          patchCord1(waveform1, envelope1);
AudioConnection          patchCord2(envelope1, 0, mixer1, 0);
AudioConnection          patchCord3(mixer1, dac1);


Sound::Sound(void) {
  AudioMemory(8);
}

void Sound::beep(long ms, long freq)
 {
  waveform1.begin(1.0, freq, WAVEFORM_SINE);
  envelope1.delay(0);
  envelope1.attack(10);
  envelope1.hold(ms);
  envelope1.decay(200);

  envelope1.sustain(0);  //  just a percussive sound, no sustain
  envelope1.noteOn();
  DEBUG_LN("beep");
 }

void Sound::setMute(bool mute) {
  if (mute) {
    if (mainGain > 0) {
      mainGain = -mainGain;
    }
  } else {
    if (mainGain < 0) {
      mainGain = -mainGain;
    }
  }

  mixer1.gain(0, mainGain);
}

void Sound::setVolume(float volume) {

  mainGain = volume;
  mixer1.gain(0, mainGain);

}
