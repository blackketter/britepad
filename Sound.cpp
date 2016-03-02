#include "Sound.h"
#include "Clock.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>

#include "Debug.h"

#define NO_SYNTH (-1)
#define SYNTH_COUNT (4)

// waveform synths
AudioSynthWaveform       waveforms[SYNTH_COUNT];

// envelopes for the synths
AudioEffectEnvelope      envelopes[SYNTH_COUNT];

// mix down the four synth channels
AudioMixer4              mixer1;

AudioConnection          patchCord1(waveforms[0], envelopes[0]);
AudioConnection          patchCord2(waveforms[1], envelopes[1]);
AudioConnection          patchCord3(waveforms[2], envelopes[2]);
AudioConnection          patchCord4(waveforms[3], envelopes[3]);

AudioConnection          patchCord6(envelopes[0], 0, mixer1, 0);
AudioConnection          patchCord5(envelopes[1], 0, mixer1, 1);
AudioConnection          patchCord7(envelopes[2], 0, mixer1, 2);
AudioConnection          patchCord8(envelopes[3], 0, mixer1, 3);

// final mixer/volume control
AudioMixer4              finalMixer;

// output dac
AudioOutputAnalog        dac1;

AudioConnection          patchCord9(mixer1, 0, finalMixer, 0);
AudioConnection          patchCord10(finalMixer, dac1);


Sound::Sound() {
}

void Sound::begin() {
  AudioMemory(16 + 18);  // 18 from the TunePlayer

  // since we have 4 inputs, we need to cut them down to avoid clipping
  mixer1.gain(0, 0.25);
  mixer1.gain(1, 0.25);
  mixer1.gain(2, 0.25);
  mixer1.gain(3, 0.25);

  finalMixer.gain(0, mainGain);

  tunePlayerBegin();

  // Initialize processor and memory measurements
  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();

}

int Sound::freeSynth() {
  for (int i = 0; i < SYNTH_COUNT; i++) {
    if (envelopes[i].getState() == STATE_IDLE) {
      DEBUGF("freeSynth = %d\n", i);
      return i;
    }
  }
  DEBUG_LN("NO_SYNTH available!");
  return NO_SYNTH;
}


void Sound::click() {
  int beeper = freeSynth();
  if (beeper != NO_SYNTH) {
    AudioNoInterrupts();
    envelopes[beeper].delay(0);
    envelopes[beeper].attack(0);
    envelopes[beeper].hold(10);
    envelopes[beeper].decay(10);
    envelopes[beeper].sustain(0);  //  just a percussive sound, no sustain

    waveforms[beeper].begin(1.0, 100, WAVEFORM_PULSE);
    envelopes[beeper].noteOn();
    AudioInterrupts();
    DEBUG_LN("click");
  }
 }

void Sound::beep(millis_t ms, float freq)
 {
  int beeper = freeSynth();
  if (beeper != NO_SYNTH) {
    AudioNoInterrupts();
    envelopes[beeper].delay(0);
    envelopes[beeper].attack(10);
    envelopes[beeper].hold(ms);
    envelopes[beeper].decay(200);
    envelopes[beeper].sustain(0);  //  just a percussive sound, no sustain
    envelopes[beeper].release(0.0);
    waveforms[beeper].begin(1.0, freq, WAVEFORM_SINE);
    envelopes[beeper].noteOn();
    AudioInterrupts();
    DEBUGF("beep (%d) freq: %f\n", beeper, freq);
  }
}

void Sound::bump() {
  DEBUG_LN("bump");
  beep(1, 300);
}

void Sound::swipe(direction_t d) {
  beep(250, 300);
}

void Sound::tone(float freq, float volume) {

  if (lastToneFreq == freq && lastToneVolume == volume) {
    return;
  }

  DEBUGF("tone freq: %f\n",freq);

  if (toneSynth == NO_SYNTH) {
    if (volume == 0.0) {
      return;
    } else {
      toneSynth = freeSynth();
      if (toneSynth == NO_SYNTH) {
        return;
      }
    }
  }

  AudioNoInterrupts();
  waveforms[toneSynth].frequency(freq);

  if (volume != 0) {
    waveforms[toneSynth].amplitude(volume);
  }

  if (lastToneVolume == 0 && volume != 0) {
    envelopes[toneSynth].delay(0);
    envelopes[toneSynth].attack(200);
    envelopes[toneSynth].hold(0);
    envelopes[toneSynth].decay(0);
    envelopes[toneSynth].sustain(1.0);
    envelopes[toneSynth].release(200);
    waveforms[toneSynth].begin(WAVEFORM_SINE);
    envelopes[toneSynth].noteOn();
  } else if (lastToneVolume != 0 && volume == 0) {
    envelopes[toneSynth].noteOff();
    toneSynth = -1;
  }

  AudioInterrupts();

  lastToneVolume = volume;
  lastToneFreq = freq;
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

  finalMixer.gain(0, mainGain);
}

void Sound::setVolume(float volume) {

  mainGain = volume;
  finalMixer.gain(0, mainGain);
}

void Sound::idle() {
// Change this to if(1) for measurement output every 5 seconds
if(0) {
  static unsigned long last_time = millis();
  if(millis() - last_time >= 5000) {
    Serial.print("Proc = ");
    Serial.print(AudioProcessorUsage());
    Serial.print(" (");
    Serial.print(AudioProcessorUsageMax());
    Serial.print("),  Mem = ");
    Serial.print(AudioMemoryUsage());
    Serial.print(" (");
    Serial.print(AudioMemoryUsageMax());
    Serial.println(")");
    last_time = millis();
  }
}
  tunePlayerIdle();
}
