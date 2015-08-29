#include "Sound.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include "Debug.h"


  // GUItool: begin automatically generated code
  AudioSynthWaveformSine   sine1;          //xy=767,286
  AudioEffectEnvelope      envelope1;      //xy=946,463
  AudioOutputAnalog        dac1;           //xy=1349,366
  AudioConnection          patchCord1(sine1, dac1);

Sound::Sound(void) {
//  AudioConnection          patchCord1(sine1, envelope1);
//  AudioConnection          patchCord2(envelope1, pwm1);
  AudioMemory(8);
//  envelope1.attack(50);
}

void Sound::beep(void) {
    DEBUG_LN("beep");
  sine1.frequency(1000);
  sine1.amplitude(1.0);
  delay(100);
  sine1.amplitude(0);
//    envelope1.noteOn();
};

