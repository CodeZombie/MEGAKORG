#include "microkorg.h"

Microkorg::Microkorg(int mux_pin_0, int mux_pin_1, int mux_pin_2, int mux_enable_pin) {
  //Serial.print("Microkorg library initializing...\n");
  //enable SPI
  //SPI.begin();
  //SPI.setBitOrder(MSBFIRST);

  this->mux_pin[0] = mux_pin_0;
  this->mux_pin[1] = mux_pin_1;
  this->mux_pin[2] = mux_pin_2;
  this->mux_enable_pin = mux_enable_pin;
  
  pinMode(this->mux_pin[0], OUTPUT);
  pinMode(this->mux_pin[1], OUTPUT);
  pinMode(this->mux_pin[2], OUTPUT);
  pinMode(this->mux_enable_pin, OUTPUT);

  pinMode(this->spi_data_pin, OUTPUT);
  pinMode(this->spi_clock_pin, OUTPUT);
}

void Microkorg::shiftOutByte(char value) {
  for(int i = 0; i < 8; i++) { //for each bit in the byte
    digitalWrite(spi_data_pin, !!(value & (1 << (7 - i))));
    digitalWrite(spi_clock_pin, HIGH);//cycle the clock pin to commit this data write.
    digitalWrite(spi_clock_pin, LOW);
  }
}

void Microkorg::sendByteToShiftRegister(char value) {
  selectChip(ch_shift_register);//Set the shift regsiter's latch pin LOW.
  this->shiftOutByte(value);
  digitalWrite(mux_enable_pin, HIGH); //Disable the MUX chip, which sets SR latch HIGH, commiting the values.
}

//turns the correct bank knob to a position
void Microkorg::selectBank(char bank_) {
  BankKnob bankKnob;
  char pos;

  //check for out-of-bounds
  if(bank_ > 20){
    return;
  }

  //determine which physical knob needs to be turned...
  if(bank_ >= 10) {//lower bank
    bankKnob = kn_lower;
    pos = bank_ - 10;
  }else{ //under 10 (upper bank)
    bankKnob = kn_upper;
    pos = bank_;
  }
  
  //determine if anything needs to be done at all.
  if(bankKnob == this->lastTurnedKnob) { //if this knob is the last one that turned.
    if(bankKnob == kn_upper) { //if it was upper
      if(pos == this->currentKnobPositions >> 4) { //and we want to turn it to its current position...
        //Serial.print("Already in position UPPER");
        return;
      }
    }else{
      if(pos << 4 == this->currentKnobPositions << 4) {
        //Serial.print("Already in position LOWER");
        return;
      }
    }
  }

  char newKnobPositions;

  if(bankKnob == kn_upper) {
    if(pos == this->currentKnobPositions >> 4) { //if it is already in that position, this means that we need to turn the knob then turn it back.
      if(pos == 0){ //if we want to set it to position zero
        this->sendByteToShiftRegister( (1 << 4) | (this->currentKnobPositions & 0xF)); // 0001<lower 4 bits> //set it to position 1
      }else{ //else
        this->sendByteToShiftRegister(this->currentKnobPositions & 0xF); // 0000<lower 4 bits> //set it to position 0
      }
    }
    newKnobPositions = (pos << 4) | (this->currentKnobPositions & 0xF); // <pos><lower 4 bits> //then set it to the final position.
  }else{
    if( (pos & 0xF)  == (this->currentKnobPositions & 0xF) ) { //if the lower knob is already in the correct position...
      if(pos == 0) {
        this->sendByteToShiftRegister( 1 | (this->currentKnobPositions & 240)); //<upper 4 bits> 0001 //set it to positon 1
      }else{
        this->sendByteToShiftRegister((this->currentKnobPositions & 240)); //<upper 4 bits> 0000 //set it to positon 0
      }
    }
    newKnobPositions = pos | (this->currentKnobPositions & 240);
  }

  this->sendByteToShiftRegister(newKnobPositions);
  this->currentKnobPositions = newKnobPositions;
}


void Microkorg::selectChip(int index) {
  //selects a chip for SPI communication.
  
  //0 - Digipot controlling knob 5
  //1 - Digipot controlling knob 4
  //2 - Digipot controlling knob 3
  //3 - Digipot controlling knob 2
  //4 - Digipot controlling knob 1
  //5 - The Shift Register which controls the parameter knob multiplexers.
  
  digitalWrite(mux_enable_pin, HIGH); //Disable the MUX chip, setting all pins HIGH. 
                                      //This is done so the mux doesnt select random chips while settings the individual pins high/low.
  
  if(index == 0) { // 000
    digitalWrite(mux_pin[0], LOW);
    digitalWrite(mux_pin[1], LOW);
    digitalWrite(mux_pin[2], LOW);
  }else if(index == 1) { //001
    digitalWrite(mux_pin[0], HIGH);
    digitalWrite(mux_pin[1], LOW);
    digitalWrite(mux_pin[2], LOW);
  }else if(index == 2) { //010
    digitalWrite(mux_pin[0], LOW);
    digitalWrite(mux_pin[1], HIGH);
    digitalWrite(mux_pin[2], LOW);
  }else if(index == 3) { //011
    digitalWrite(mux_pin[0], HIGH);
    digitalWrite(mux_pin[1], HIGH);
    digitalWrite(mux_pin[2], LOW);
  }else if(index == 4) { //100
    digitalWrite(mux_pin[0], LOW);
    digitalWrite(mux_pin[1], LOW);
    digitalWrite(mux_pin[2], HIGH);
  }else if(index == 5) { // 101
    digitalWrite(mux_pin[0], HIGH);
    digitalWrite(mux_pin[1], LOW);
    digitalWrite(mux_pin[2], HIGH);
  }else if(index == 6) { // 110
    digitalWrite(mux_pin[0], LOW);
    digitalWrite(mux_pin[1], HIGH);
    digitalWrite(mux_pin[2], HIGH);
  }else if(index == 7) { //111
    digitalWrite(mux_pin[0], HIGH);
    digitalWrite(mux_pin[1], HIGH);
    digitalWrite(mux_pin[2], HIGH);
  }
  digitalWrite(mux_enable_pin, LOW); //Re-enable the mux chip.  
}

void Microkorg::setDigipotValue(Chip digipot_index, char value) {
  selectChip(digipot_index);          //select the digipot.
  this->shiftOutByte(B00010001);      //tell the digipot that we're sending it data...
  this->shiftOutByte(255 - value);    //and send it the data.
  selectChip(digipot_index);          //cycle the multiplexer, which will set the digipots CS from HIGH back to LOW, shifting the data in.
  digitalWrite(mux_enable_pin, HIGH); //Disable the mux chip.
}
