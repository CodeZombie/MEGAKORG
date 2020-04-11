#include "microkorg.h"
#include "SPI.h"
#include "multiplexercontroller.h"

Microkorg::Microkorg(char mux_pin_0, char mux_pin_1, char mux_pin_2, char mux_enable_pin) {
  Serial.print("Microkorg library initializing...\n");

  this->multiplexerController = new MultiplexerController(mux_pin_0, mux_pin_1, mux_pin_2, mux_enable_pin);

  //instantiate all 5 digipots, setting them all to zero.
  for(char i = 0; i < 5; i++){
    this->digipot[i] = new Digipot(this->multiplexerController, i);
  }
  
  //intantiate the bank, setting it to the 0th bank.
  this->bankController = new BankController(this->multiplexerController);

  this->parameter[VOICE_synth_vocoder] = new Parameter(0, 0, 0);
  this->parameter[VOICE_single_layer] = new Parameter(0, 1, 0);
  this->parameter[VOICE_voice_assign] = new Parameter(0, 2, 0);
  this->parameter[VOICE_trigger_mode] = new Parameter(0, 3, 0);
  this->parameter[VOICE_unison_detune] = new Parameter(0, 4, 0);
  
  this->parameter[PITCH_transpose] = new Parameter(1, 0, 0);
  this->parameter[PITCH_tune] = new Parameter(1, 1, 0);
  this->parameter[PITCH_portamento] = new Parameter(1, 2, 0);
  this->parameter[PITCH_bend_range] = new Parameter(1, 3, 0);
  this->parameter[PITCH_vibrato_int] = new Parameter(1, 4, 0);
  
  this->parameter[OSC1_wave] = new Parameter(2, 0, 0);
  this->parameter[OSC1_control1] = new Parameter(2, 1, 0);
  this->parameter[OSC1_control2] = new Parameter(2, 2, 0);
  
  this->parameter[OSC2_wave] = new Parameter(3, 0, 0);
  this->parameter[OSC2_osc_mod] = new Parameter(3, 1, 0);
  this->parameter[OSC2_semitone] = new Parameter(3, 2, 0);
  this->parameter[OSC2_tune] = new Parameter(3, 3, 0);
  
  this->parameter[MIXER_osc1_level] = new Parameter(4, 0, 0);
  this->parameter[MIXER_OSC2_level] = new Parameter(4, 1, 0);
  this->parameter[MIXER_noise_level] = new Parameter(4, 2, 0);

}

void Microkorg::setParameter(int parameterIndex, char value){
  //deactivate params if we are going to be changing banks...
  if(this->bankController->getSelectedBank() != this->parameter[parameterIndex]->bank) {
    for(int i = 0; i < 20; i++) {
      this->parameter[i]->active = false;
    }
  }
  Serial.print("REQUEST: Change parameter at bank ");
  Serial.print((int)this->parameter[parameterIndex]->bank);
  Serial.print(" and knob ");
  Serial.print((int)this->parameter[parameterIndex]->knob);
  Serial.print(" to value: ");
  Serial.print((int)value);
  Serial.print("\n");
  this->parameter[parameterIndex]->setValue(this->multiplexerController, this->bankController, this->digipot, value);
}
