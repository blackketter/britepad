#include "Sound.h"

#include <Audio.h>

#include "Debug.h"
#include "Clock.h"

// Implement a polyphonic midi player, based on the public domain code provided by   :-)
//
// Music data is read from memory.  The "Miditones" program is used to
// convert from a MIDI file to this compact format.


// Table of midi note frequencies * 2
//   They are times 2 for greater accuracy, yet still fits in a word.
//   Generated from Excel by =ROUND(2*440/32*(2^((x-9)/12)),0) for 0<x<128
// The lowest notes might not work, depending on the Arduino clock frequency

// This is for the Teensy Audio library which specifies
// frequencies as floating point. See make_notetab.xlsx

#define MAX_NOTES (16)
const int16_t pitchRange = 128;
const float tune_frequencies2_PGM[pitchRange] =
{
    8.1758,    8.6620,    9.1770,    9.7227,    10.3009,    10.9134,    11.5623,    12.2499,
    12.9783,   13.7500,   14.5676,   15.4339,   16.3516,    17.3239,    18.3540,    19.4454,
    20.6017,   21.8268,   23.1247,   24.4997,   25.9565,    27.5000,    29.1352,    30.8677,
    32.7032,   34.6478,   36.7081,   38.8909,   41.2034,    43.6535,    46.2493,    48.9994,
    51.9131,   55.0000,   58.2705,   61.7354,   65.4064,    69.2957,    73.4162,    77.7817,
    82.4069,   87.3071,   92.4986,   97.9989,   103.8262,   110.0000,   116.5409,   123.4708,
    130.8128,  138.5913,  146.8324,  155.5635,  164.8138,   174.6141,   184.9972,   195.9977,
    207.6523,  220.0000,  233.0819,  246.9417,  261.6256,   277.1826,   293.6648,   311.1270,
    329.6276,  349.2282,  369.9944,  391.9954,  415.3047,   440.0000,   466.1638,   493.8833,
    523.2511,  554.3653,  587.3295,  622.2540,  659.2551,   698.4565,   739.9888,   783.9909,
    830.6094,  880.0000,  932.3275,  987.7666,  1046.5023,  1108.7305,  1174.6591,  1244.5079,
    1318.5102, 1396.9129, 1479.9777, 1567.9817, 1661.2188,  1760.0000,  1864.6550,  1975.5332,
    2093.0045, 2217.4610, 2349.3181, 2489.0159, 2637.0205,  2793.8259,  2959.9554,  3135.9635,
    3322.4376, 3520.0000, 3729.3101, 3951.0664, 4186.0090,  4434.9221,  4698.6363,  4978.0317,
    5274.0409, 5587.6517, 5919.9108, 6271.9270, 6644.8752,  7040.0000,  7458.6202,  7902.1328,
    8372.0181, 8869.8442, 9397.2726, 9956.0635, 10548.0818, 11175.3034, 11839.8215, 12543.8540
};

#define CMD_PLAYNOTE	0x90	/* play a note: low nibble is generator #, note is next byte */
#define CMD_STOPNOTE	0x80	/* stop a note: low nibble is generator # */
#define CMD_RESTART	0xe0	/* restart the score from the beginning */
#define CMD_STOP	0xf0	/* stop playing */

const float velocity2amplitude[127] = {
0.01778,0.01966,0.02164,0.02371,0.02588,0.02814,0.03049,0.03294,0.03549,0.03812,
0.04086,0.04369,0.04661,0.04963,0.05274,0.05594,0.05924,0.06264,0.06613,0.06972,
0.07340,0.07717,0.08104,0.08500,0.08906,0.09321,0.09746,0.10180,0.10624,0.11077,
0.11539,0.12011,0.12493,0.12984,0.13484,0.13994,0.14513,0.15042,0.15581,0.16128,
0.16685,0.17252,0.17828,0.18414,0.19009,0.19613,0.20227,0.20851,0.21484,0.22126,
0.22778,0.23439,0.24110,0.24790,0.25480,0.26179,0.26887,0.27605,0.28333,0.29070,
0.29816,0.30572,0.31337,0.32112,0.32896,0.33690,0.34493,0.35306,0.36128,0.36960,
0.37801,0.38651,0.39511,0.40381,0.41260,0.42148,0.43046,0.43953,0.44870,0.45796,
0.46732,0.47677,0.48631,0.49595,0.50569,0.51552,0.52544,0.53546,0.54557,0.55578,
0.56609,0.57648,0.58697,0.59756,0.60824,0.61902,0.62989,0.64085,0.65191,0.66307,
0.67432,0.68566,0.69710,0.70863,0.72026,0.73198,0.74380,0.75571,0.76771,0.77981,
0.79201,0.80430,0.81668,0.82916,0.84174,0.85440,0.86717,0.88003,0.89298,0.90602,
0.91917,0.93240,0.94573,0.95916,0.97268,0.98629,1.00000
};
/*
#! /usr/bin/perl
# The Interpretation of MIDI Velocity
# Roger B. Dannenberg
# School of Computer Science, Carnegie Mellon University
$dynamic_range = 35;
$r = 10 ** ($dynamic_range / 20);
$b = 127 / (126 * sqrt($r)) - 1/126;
$m = (1 - $b) / 127;
print "const float velocity2amplitude[127] = {\n";
for ($v=1; $v <= 127; $v++) {
	$a = ($m * $v + $b) ** 2;
	printf "%.5f", $a;
	print "," if $v < 127;
	print "\n" if ($v % 10) == 0;
}
print "\n};\n";
*/


#define AMPLITUDE (0.5)

// Create waveforms, one for each MIDI channel
AudioSynthWaveform waves[MAX_NOTES];

// allocate a wave type to each channel.
// The types used and their order is purely arbitrary.
short wave_type[MAX_NOTES] = {
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
  WAVEFORM_SINE,
};

// Each waveform will be shaped by an envelope
AudioEffectEnvelope envs[MAX_NOTES];

// Route each waveform through its own envelope effect
AudioConnection tunePatchCord01(waves[0], envs[0]);
AudioConnection tunePatchCord02(waves[1], envs[1]);
AudioConnection tunePatchCord03(waves[2], envs[2]);
AudioConnection tunePatchCord04(waves[3], envs[3]);
AudioConnection tunePatchCord05(waves[4], envs[4]);
AudioConnection tunePatchCord06(waves[5], envs[5]);
AudioConnection tunePatchCord07(waves[6], envs[6]);
AudioConnection tunePatchCord08(waves[7], envs[7]);
AudioConnection tunePatchCord09(waves[8], envs[8]);
AudioConnection tunePatchCord10(waves[9], envs[9]);
AudioConnection tunePatchCord11(waves[10], envs[10]);
AudioConnection tunePatchCord12(waves[11], envs[11]);
AudioConnection tunePatchCord13(waves[12], envs[12]);
AudioConnection tunePatchCord14(waves[13], envs[13]);
AudioConnection tunePatchCord15(waves[14], envs[14]);
AudioConnection tunePatchCord16(waves[15], envs[15]);

// Four mixers are needed to handle 16 channels of music
AudioMixer4     tuneMixer1;
AudioMixer4     tuneMixer2;
AudioMixer4     tuneMixer3;
AudioMixer4     tuneMixer4;

// Mix the 16 channels down to 4 audio streams
AudioConnection tunePatchCord17(envs[0], 0, tuneMixer1, 0);
AudioConnection tunePatchCord18(envs[1], 0, tuneMixer1, 1);
AudioConnection tunePatchCord19(envs[2], 0, tuneMixer1, 2);
AudioConnection tunePatchCord20(envs[3], 0, tuneMixer1, 3);
AudioConnection tunePatchCord21(envs[4], 0, tuneMixer2, 0);
AudioConnection tunePatchCord22(envs[5], 0, tuneMixer2, 1);
AudioConnection tunePatchCord23(envs[6], 0, tuneMixer2, 2);
AudioConnection tunePatchCord24(envs[7], 0, tuneMixer2, 3);
AudioConnection tunePatchCord25(envs[8], 0, tuneMixer3, 0);
AudioConnection tunePatchCord26(envs[9], 0, tuneMixer3, 1);
AudioConnection tunePatchCord27(envs[10], 0, tuneMixer3, 2);
AudioConnection tunePatchCord28(envs[11], 0, tuneMixer3, 3);
AudioConnection tunePatchCord29(envs[12], 0, tuneMixer4, 0);
AudioConnection tunePatchCord30(envs[13], 0, tuneMixer4, 1);
AudioConnection tunePatchCord31(envs[14], 0, tuneMixer4, 2);
AudioConnection tunePatchCord32(envs[15], 0, tuneMixer4, 3);

// Now create a final mixer for the main
AudioMixer4     tuneMixerFinal;

// Mix all channels to both the outputs
AudioConnection tunePatchCord33(tuneMixer1, 0, tuneMixerFinal, 0);
AudioConnection tunePatchCord34(tuneMixer2, 0, tuneMixerFinal, 1);
AudioConnection tunePatchCord35(tuneMixer3, 0, tuneMixerFinal, 2);
AudioConnection tunePatchCord36(tuneMixer4, 0, tuneMixerFinal, 3);

extern AudioMixer4 finalMixer;
AudioConnection tunePatchCord41(tuneMixerFinal, 0, finalMixer, 1);
void Sound::tuneVolume(float v) {
  finalMixer.gain(1, v);
}

void Sound::tunePlay(const tune_t tuneBytes) {
  curTuneIndex = 0;
  curTune = tuneBytes;
  nextTime = 0;

  if (tuneBytes == nullptr) {
    AudioNoInterrupts();
    for (int i = 0; i < MAX_NOTES; i++) {
      envs[i].noteOff();
      waves[i].amplitude(0);
    }
    AudioInterrupts();
  }

}

void Sound::tunePlayerBegin()
{

  // set envelope parameters, for pleasing sound :-)
  for (int i=0; i<MAX_NOTES; i++) {
    envs[i].attack(9.2);
    envs[i].hold(2.1);
    envs[i].decay(31.4);
    envs[i].sustain(0.6);
    envs[i].release(84.5);
    // uncomment these to hear without envelope effects
    //envs[i].attack(0.0);
    //envs[i].hold(0.0);
    //envs[i].decay(0.0);
    //envs[i].release(0.0);
  }

  curTune = nullptr;
  curTuneIndex = 0;
  tuneVolume(1.0);
  tuneTempo(1.0);
  tuneTranspose(0);
}


void Sound::tunePlayerIdle()
{
  uint8_t c,opcode,chan;
  uint32_t d_time;

  if (curTune == nullptr || tempo == 0 || nextTime > Uptime::millis()) {
    return;
  }

  while (1) {
    // read the next note from the table
    c = curTune[curTuneIndex++];
    opcode = c & 0xF0;
    chan = c & 0x0F;

    if (opcode < 0x80) {
      // Delay
      d_time = (c << 8) | curTune[curTuneIndex++];
      d_time = d_time / tempo;
      if (d_time) {
        nextTime = Uptime::millis() + d_time;
        return;
      }
    } else if (opcode == CMD_STOP) {
      if (repeat) {
        curTuneIndex = 0;
      } else {
        tunePlay(nullptr);
      }
      return;
    } else if (opcode == CMD_RESTART) {
      curTuneIndex = 0;
      return;
    } else if (opcode == CMD_STOPNOTE) {
      envs[chan].noteOff();
      // don't return and parse the next note
    } else if (opcode == CMD_PLAYNOTE) {
      uint16_t note = curTune[curTuneIndex++];
      uint8_t velocity = curTune[curTuneIndex++];
      note = note + transpose;
      if ((note >= 0) && (note < pitchRange)) {
        AudioNoInterrupts();
        waves[chan].begin(AMPLITUDE * velocity2amplitude[velocity-1],
                           tune_frequencies2_PGM[note],
                           wave_type[chan]);
        envs[chan].noteOn();
        // don't return and parse the next note
        AudioInterrupts();
      }
    }
  }
}
