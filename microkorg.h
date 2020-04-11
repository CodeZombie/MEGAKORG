#ifndef Microkorg_h
#define Microkorg_h
#include <Arduino.h> 
#include "multiplexercontroller.h"
#include "digipot.h"
#include "bankcontroller.h"
#include "parameter.h"

typedef enum synthParameter {
  VOICE_synth_vocoder,
  VOICE_single_layer,
  VOICE_voice_assign,
  VOICE_trigger_mode,
  VOICE_unison_detune,
  PITCH_transpose,
  PITCH_tune,
  PITCH_portamento,
  PITCH_bend_range,
  PITCH_vibrato_int,
  OSC1_wave,
  OSC1_control1,
  OSC1_control2,
  OSC2_wave,
  OSC2_osc_mod,
  OSC2_semitone,
  OSC2_tune,
  MIXER_osc1_level,
  MIXER_OSC2_level,
  MIXER_noise_level
} SynthParameter;

typedef enum paramaterBank {  
  pb_voice,
  pb_pitch,
  pb_osc_1,
  pb_osc_2,
  pb_mixer,
  pb_filter,
  pb_filter_eg,
  pb_amp,
  pb_amp_eg,
  pb_lfo_1,
  pb_lfo_2,
  // bank 2 (lower)
  pb_patch_1,
  pb_patch_2,
  pb_patch_3,
  pb_patch_4,
  pb_mod_fx,
  pb_delay,
  pb_eq,
  pb_arpeg_a,
  pb_arpeg_b,
  pb_global,
  pb_midi} ParameterBank;

class Microkorg {
  public:
    Microkorg(char mux_pin_0, char mux_pin_1, char mux_pin_2, char mux_enable_pin);
    void setParameter(int parameter, char value);

  private:
    Digipot *digipot[5];
    BankController *bankController;
    MultiplexerController *multiplexerController;
    Parameter *parameter[20];  
};

#endif
