#if TEENSY
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>

#include "Sound.h"
#include "Clock.h"
#include "Console.h"
#include "Timer.h"

extern Console console;

#define NO_SYNTH (-1)
#define SYNTH_COUNT (4)

// waveform synths
AudioSynthWaveform       waveforms[SYNTH_COUNT];

// envelopes for the synths
AudioEffectEnvelope      envelopes[SYNTH_COUNT];
CallbackTimer            envelopeTimer[SYNTH_COUNT];
AudioEffectFade          fades[SYNTH_COUNT];

void noteOff(void* envelope) {
  ((AudioEffectEnvelope*)envelope)->noteOff();
}

// mix down the four synth channels
AudioMixer4              mixer1;

AudioConnection          patchCordA0(waveforms[0], envelopes[0]);
AudioConnection          patchCordA1(waveforms[1], envelopes[1]);
AudioConnection          patchCordA2(waveforms[2], envelopes[2]);
AudioConnection          patchCordA3(waveforms[3], envelopes[3]);

AudioConnection          patchCordB0(envelopes[0], fades[0]);
AudioConnection          patchCordB1(envelopes[1], fades[1]);
AudioConnection          patchCordB2(envelopes[2], fades[2]);
AudioConnection          patchCordB3(envelopes[3], fades[3]);

AudioConnection          patchCordC0(fades[0], 0, mixer1, 0);
AudioConnection          patchCordC1(fades[1], 0, mixer1, 1);
AudioConnection          patchCordC2(fades[2], 0, mixer1, 2);
AudioConnection          patchCordC3(fades[3], 0, mixer1, 3);

// final mixer/volume control
AudioMixer4              finalMixer;

// output dac
AudioOutputAnalog        dac1;

AudioConnection          patchCord9(mixer1, 0, finalMixer, 0);
AudioConnection          patchCord10(finalMixer, dac1);


Sound::Sound() {
}

void Sound::begin() {
  AudioMemory(20 + 18);  // 18 from the TunePlayer

  // since we have 4 inputs, we need to cut them down to avoid clipping
  mixer1.gain(0, 0.25);
  mixer1.gain(1, 0.25);
  mixer1.gain(2, 0.25);
  mixer1.gain(3, 0.25);

  updateMainGain();

  tunePlayerBegin();

  // Initialize processor and memory measurements
  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();

}

int Sound::freeSynth() {
  for (int i = 0; i < SYNTH_COUNT; i++) {
    if (envelopes[i].getState() == STATE_IDLE) {
//      console.debugf("freeSynth = %d\n", i);
      return i;
    }
  }
  console.debugln("NO_SYNTH available!");
  return NO_SYNTH;
}


void Sound::click() {
  int beeper = freeSynth();
  if (beeper != NO_SYNTH) {
    AudioNoInterrupts();
    fades[beeper].fadeIn(0);
    envelopes[beeper].delay(0);
    envelopes[beeper].attack(0);
    envelopes[beeper].hold(10);
    envelopes[beeper].decay(10);
    envelopes[beeper].sustain(0);  //  just a percussive sound, no sustain

    waveforms[beeper].begin(1.0, 100, WAVEFORM_PULSE);
    envelopes[beeper].noteOn();
    AudioInterrupts();
//    console.debugln("click");
  }
 }


void Sound::beep(millis_t ms, float freq)
 {
  int beeper = freeSynth();
  if (beeper != NO_SYNTH) {
    AudioNoInterrupts();
    fades[beeper].fadeIn(0);
    envelopes[beeper].delay(0);
    envelopes[beeper].attack(20);
    envelopes[beeper].hold(0);
    envelopes[beeper].decay(0);
    envelopes[beeper].sustain(1.0);
    envelopes[beeper].release(20.0);
    waveforms[beeper].begin(1.0, freq, WAVEFORM_SINE);
    envelopes[beeper].noteOn();
    AudioInterrupts();
    envelopeTimer[beeper].setMillis(ms, &noteOff, (void*)(&envelopes[beeper]));
//    console.debugf("beep (%d) freq: %f\n", beeper, freq);
  }
}

void Sound::bell(millis_t ms, float freq)
 {
  int beeper = freeSynth();
  if (beeper != NO_SYNTH) {
    AudioNoInterrupts();
    fades[beeper].fadeIn(0);
    envelopes[beeper].delay(0);
    envelopes[beeper].attack(20);
    envelopes[beeper].hold(0);
    envelopes[beeper].decay(0);
    envelopes[beeper].sustain(1.0);
    envelopes[beeper].release(20.0);
    waveforms[beeper].begin(1.0, freq, WAVEFORM_SINE);
    fades[beeper].fadeOut(ms);
    envelopes[beeper].noteOn();
    AudioInterrupts();
    envelopeTimer[beeper].setMillis(ms, &noteOff, (void*)(&envelopes[beeper]));
//    console.debugf("bell (%d) freq: %f\n", beeper, freq);
  }
}

void Sound::bump() {
//  console.debugln("bump");
  beep(1, 300);
}

void Sound::swipe(direction_t d) {
  float f;
  switch (d) {
    case DIRECTION_UP:
      f = MIDDLE_C_FREQ*2;
      break;
    case DIRECTION_DOWN:
      f = MIDDLE_C_FREQ;
      break;
    case DIRECTION_LEFT:
      f = A440_FREQ;
      break;
    case DIRECTION_RIGHT:
      f = A440_FREQ*2;
      break;
    default:
      console.debugln("Bad direction");
      f = MIDDLE_C_FREQ;
  }
  bell(250, f);
}

void Sound::tone(float freq, float volume) {

  if (lastToneFreq == freq && lastToneVolume == volume) {
    return;
  }

//  console.debugf("tone freq: %f\n",freq);

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
    fades[toneSynth].fadeIn(0);
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
//  console.debugf("noise vol: %f\n", volume);
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
  updateMainGain();
}

void Sound::setVolume(float volume) {

  mainGain = mainGain < 0 ? -volume : volume;
  updateMainGain();
}

void Sound::updateMainGain() {
  finalMixer.gain(0, mainGain < 0 ? 0 : mainGain);
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
#endif // TEENSY