#include "Sound.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include "Debug.h"

#define NO_SYNTH (-1)
#define SYNTH_COUNT (4)

// waveform synths
AudioSynthWaveform       waveform1;
AudioSynthWaveform       waveform2;
AudioSynthWaveform       waveform3;
AudioSynthWaveform       waveform4;

AudioSynthWaveform* waveforms[] = { &waveform1, &waveform2, &waveform3, &waveform4};

// envelopes for the synths
AudioEffectEnvelope      envelope1;
AudioEffectEnvelope      envelope2;
AudioEffectEnvelope      envelope3;
AudioEffectEnvelope      envelope4;

AudioEffectEnvelope* envelopes[] = { &envelope1, &envelope2, &envelope3, &envelope4};

// mix down the four synth channels
AudioMixer4              mixer1;

AudioConnection          patchCord1(waveform1, envelope1);
AudioConnection          patchCord2(waveform2, envelope2);
AudioConnection          patchCord3(waveform3, envelope3);
AudioConnection          patchCord4(waveform4, envelope4);

AudioConnection          patchCord6(envelope1, 0, mixer1, 0);
AudioConnection          patchCord5(envelope2, 0, mixer1, 1);
AudioConnection          patchCord7(envelope3, 0, mixer1, 2);
AudioConnection          patchCord8(envelope4, 0, mixer1, 3);

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
    if (envelopes[i]->getState() == STATE_IDLE) {
      return i;
    } else {
    }
  }
  DEBUG_LN("NO_SYNTH available!");
  return NO_SYNTH;
}


void Sound::click() {
  int beeper = freeSynth();
  if (beeper != NO_SYNTH) {
    waveforms[beeper]->begin(1.0, 100, WAVEFORM_PULSE);
    envelopes[beeper]->delay(0);
    envelopes[beeper]->attack(0);
    envelopes[beeper]->hold(10);
    envelopes[beeper]->decay(10);

    envelopes[beeper]->sustain(0);  //  just a percussive sound, no sustain
    envelopes[beeper]->noteOn();
//    DEBUG_LN("click");
  }
 }

void Sound::beep(millis_t ms, float freq)
 {
  int beeper = freeSynth();
  if (beeper != NO_SYNTH) {
    waveforms[beeper]->begin(1.0, freq, WAVEFORM_SINE);
    envelopes[beeper]->delay(0);
    envelopes[beeper]->attack(10);
    envelopes[beeper]->hold(ms);
    envelopes[beeper]->decay(200);

    envelopes[beeper]->sustain(0);  //  just a percussive sound, no sustain
    envelopes[beeper]->release(0.0);
    envelopes[beeper]->noteOn();
//    DEBUG_LN("beep");
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

  waveform2.frequency(freq);

  if (volume != 0) {
    waveform2.amplitude(volume);
  }

  if (lastToneVolume == 0 && volume != 0) {
    envelope2.delay(0);
    envelope2.attack(200);
    envelope2.hold(0);
    envelope2.decay(0);
    envelope2.sustain(1.0);
    envelope2.release(200);
    waveform2.begin(WAVEFORM_SINE);
    envelope2.noteOn();
  } else if (lastToneVolume != 0 && volume == 0) {
    envelope2.noteOff();
  }

  lastToneVolume = volume;
}

void Sound::noise(float volume) {
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
