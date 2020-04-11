#include "parameter.h"

Parameter::Parameter(int bank, int knob, char value) {
  this->bank = bank;
  this->knob = knob;
  this->active = false;
  this->value = value;
}

void Parameter::setValue(MultiplexerController *multiplexerController, BankController *bankController, Digipot *digipot[], char value) {
  if(this->value == value) {
    Serial.println("Knob already at this value. Exiting.");
    return;
  }
  
  if(this->bank != bankController->getSelectedBank()){
    bankController->selectBank(multiplexerController, this->bank);
    delay(20);
  }

  if(!this->active) {
    Serial.println("Re-activating parameter...");
    char awayValue;
    if(this->value < 128){
      awayValue = 255;
    }else{
      awayValue = 0;
    }

    digipot[this->knob]->setValue(multiplexerController, awayValue);
    delay(30);
    digipot[this->knob]->setValue(multiplexerController, this->value);
    delay(30);
    this->active = true;
  }
  
  digipot[this->knob]->setValue(multiplexerController, value);
  this->value = value;
}
