#include "bankcontroller.h"

BankController::BankController(MultiplexerController *multiplexerController) {
    this->selectBank(multiplexerController, 0);
}

void BankController::sendByteToShiftRegister(MultiplexerController *multiplexerController, char value) {
  multiplexerController->selectChip(5); //select the bank controller shift register.
  SPI.transfer(value);
  multiplexerController->disable(); //commit the values by disabling the mux.
}

void BankController::setUpperBankPosition(MultiplexerController *multiplexerController, char value) {
    this->upperBankPosition = value;
    this->sendByteToShiftRegister(multiplexerController, ((this->upperBankPosition << 4) | this->lowerBankPosition));
}

void BankController::setLowerBankPosition(MultiplexerController *multiplexerController, char value) {
    this->lowerBankPosition = value;
    this->sendByteToShiftRegister(multiplexerController, ((this->upperBankPosition << 4) | this->lowerBankPosition));
}

void BankController::selectBank(MultiplexerController *multiplexerController, char bank){
    //REWRITE THE LOGIC OF THIS SO THAT UPPER AND LOWER ARE AN ARRAY, NOT DISTINCT VARS.
    //ALSO FIGURE OUT IF THE DELAY(1)S ARE NECESSARY OR NOT.
    Serial.print("Attempting to select bank: ");
    Serial.print((int)bank);
    Serial.print("\n");
    if(bank > 22){
      Serial.print("Bank > 22. Exiting.");
        return;
    }

    if(this->getSelectedBank() == bank){
        Serial.print("Bank already selected. Exiting.");
        return;
    }

    //Determine which knob will need to be turned:
    BankKnob bankKnobToBeTurned;
    char relativeKnobPosition;

    if(bank < 11){
        bankKnobToBeTurned = kn_upper;
        relativeKnobPosition = bank;
    }else{
        bankKnobToBeTurned = kn_lower;
        relativeKnobPosition = bank - 11;
    }

    if(bankKnobToBeTurned == kn_upper){
        if(this->upperBankPosition == relativeKnobPosition) { //if the knob is already in the position, we need to turn away and turn back.
            if(relativeKnobPosition == 0){ //if the position we want to turn to is 0.
                //TURN bankKnobToBeTurned to 1.
                this->setUpperBankPosition(multiplexerController, 1);
                //delay(100);
            }else{
                this->setUpperBankPosition(multiplexerController, 0);
                //delay(100);
            }
        }
        this->setUpperBankPosition(multiplexerController, relativeKnobPosition);
    }else{ //we need to turn kn_lower.
        if(this->lowerBankPosition == relativeKnobPosition){
            if(relativeKnobPosition == 0){
                this->setLowerBankPosition(multiplexerController, 1);
                //delay(100);
            }else{
                this->setLowerBankPosition(multiplexerController, 0);
                //delay(100);
            }
        }
        this->setLowerBankPosition(multiplexerController, relativeKnobPosition);
    }
    this->lastTurnedBankKnob = bankKnobToBeTurned;
}

char BankController::getSelectedBank() {
    if(this->lastTurnedBankKnob == kn_upper){
        return this->upperBankPosition;
    }else{
        return this->lowerBankPosition + 11;
    }
}
