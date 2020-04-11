#include "multiplexercontroller.h"

MultiplexerController::MultiplexerController(char mux_pin_0, char mux_pin_1, char mux_pin_2, char mux_enable_pin){
    this->mux_pin[0] = mux_pin_0;
    this->mux_pin[1] = mux_pin_1;
    this->mux_pin[2] = mux_pin_2;
    this->mux_enable_pin = mux_enable_pin;

    pinMode(this->mux_pin[0], OUTPUT);
    pinMode(this->mux_pin[1], OUTPUT);
    pinMode(this->mux_pin[2], OUTPUT);
    pinMode(this->mux_enable_pin, OUTPUT);

    this->disable();
}
void MultiplexerController::selectChip(char chipIndex) {
    this->disable();
    digitalWrite(this->mux_pin[0], (1 & ((chipIndex) >> 0)));
    digitalWrite(this->mux_pin[1], (1 & ((chipIndex) >> 1)));
    digitalWrite(this->mux_pin[2], (1 & ((chipIndex) >> 2)));
    this->enable();
}

void MultiplexerController::disable() {
    digitalWrite(this->mux_enable_pin, HIGH); //Disable the MUX chip, setting all pins HIGH. 
}

void MultiplexerController::enable() {
    digitalWrite(this->mux_enable_pin, LOW); //Enable the mux chip.  
}
