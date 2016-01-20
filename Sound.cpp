#include "Sound.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include "Debug.h"

#define NO_SYNTH (-1)
#define SYNTH_COUNT (4)

// waveform synths
AudioSynthWaveform       waveforms[SYNTH_COUNT];

// envelopes for the synths
AudioEffectEnvelope      envelopes[SYNTH_COUNT];

// mix down the four synth channels
AudioMixer4              mixer1;

AudioConnection          patchCord1(waveforms[0], envelopes[1]);
AudioConnection          patchCord2(waveforms[1], envelopes[1]);
AudioConnection          patchCord3(waveforms[2], envelopes[2]);
AudioConnection          patchCord4(waveforms[3], envelopes[3]);

AudioConnection          patchCord6(envelopes[0], 0, mixer1, 0);
AudioConnection          patchCord5(envelopes[1], 0, mixer1, 1);
AudioConnection          patchCord7(envelopes[2], 0, mixer1, 2);
AudioConnection          patchCord8(envelopes[3], 0, mixer1, 3);

// final mixer/volume control
AudioMixer4              mixer2;

// output dac
AudioOutputAnalog        dac1;

AudioConnection          patchCord9(mixer1, 0, mixer2, 0);
AudioConnection          patchCord10(mixer2, dac1);


Sound::Sound() {
}

void Sound::begin() {
  AudioMemory(16);

  // since we have 4 inputs, we need to cut them down to avoid clipping
  mixer1.gain(0, 0.25);
  mixer1.gain(1, 0.25);
  mixer1.gain(2, 0.25);
  mixer1.gain(3, 0.25);

  mixer2.gain(0, mainGain);

}

int Sound::freeSynth() {
  for (int i = 0; i < SYNTH_COUNT; i++) {
    if (envelopes[i].getState() == STATE_IDLE) {
      return i;
    }
  }
  DEBUG_LN("NO_SYNTH available!");
  return NO_SYNTH;
}


void Sound::click() {
  int beeper = freeSynth();
  if (beeper != NO_SYNTH) {
    waveforms[beeper].begin(1.0, 100, WAVEFORM_PULSE);
    envelopes[beeper].delay(0);
    envelopes[beeper].attack(0);
    envelopes[beeper].hold(10);
    envelopes[beeper].decay(10);

    envelopes[beeper].sustain(0);  //  just a percussive sound, no sustain
    envelopes[beeper].noteOn();
//    DEBUG_LN("click");
  }
 }

void Sound::beep(millis_t ms, float freq)
 {
  int beeper = freeSynth();
  if (beeper != NO_SYNTH) {
    waveforms[beeper].begin(1.0, freq, WAVEFORM_SINE);
    envelopes[beeper].delay(0);
    envelopes[beeper].attack(10);
    envelopes[beeper].hold(ms);
    envelopes[beeper].decay(200);

    envelopes[beeper].sustain(0);  //  just a percussive sound, no sustain
    envelopes[beeper].release(0.0);
    envelopes[beeper].noteOn();
    DEBUGF("freq: %f\n", freq);
  }
}

void Sound::bump() {
//  DEBUG_LN("bump");
  beep(1, 300);
}

void Sound::swipe(direction_t d) {
  beep(250, 300);
}

void Sound::tone(float freq, float volume) {

  waveforms[1].frequency(freq);

  if (volume != 0) {
    waveforms[1].amplitude(volume);
  }

  if (lastToneVolume == 0 && volume != 0) {
    envelopes[1].delay(0);
    envelopes[1].attack(200);
    envelopes[1].hold(0);
    envelopes[1].decay(0);
    envelopes[1].sustain(1.0);
    envelopes[1].release(200);
    waveforms[1].begin(WAVEFORM_SINE);
    envelopes[1].noteOn();
  } else if (lastToneVolume != 0 && volume == 0) {
    envelopes[1].noteOff();
  }

  lastToneVolume = volume;
}

void Sound::noise(float volume) {
//  todo: create noise sourc
  DEBUGF("noise vol: %f\n", volume);
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

  mixer2.gain(0, mainGain);
}

void Sound::setVolume(float volume) {

  mainGain = volume;
  mixer2.gain(0, mainGain);
}
