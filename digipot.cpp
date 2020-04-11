#include "digipot.h"

Digipot::Digipot(MultiplexerController *multiplexerController, char index){
    this->index = index;
    this->setValue(multiplexerController, 0);
}

void Digipot::setValue(MultiplexerController *multiplexerController, char value){
    Serial.print("Setting knob ");
    Serial.print((int)this->index);
    Serial.print(" to : ");
    Serial.print((int)value);
    Serial.print("\n");
    multiplexerController->selectChip(4 - this->index);     //Select the right digipot
    SPI.transfer(B00010001);                                //tell the digipot that we're sending it data...
    SPI.transfer(255 - value);                              //and send it the data.
    multiplexerController->selectChip(4 - this->index);     //cycle the multiplexer, which will set the digipots 
                                                              //CS from HIGH back to LOW, shifting the data in.
    multiplexerController->disable();                       // !!!! Disable the mux (Might be unecessary????)
    this->value = value;
}

char Digipot::getValue(){
    return this->value;
}
