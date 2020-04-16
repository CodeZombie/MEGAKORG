#include "HardwareController.h"

HardwareController::HardwareController(char mux_select_0, char mux_select_1, char mux_select_2, char mux_enable) {
    pinMode(mux_select_0, OUTPUT);
    pinMode(mux_select_1, OUTPUT);
    pinMode(mux_select_2, OUTPUT);
    pinMode(mux_enable, OUTPUT);
    
    this->mux_select[0] = mux_select_0;
    this->mux_select[1] = mux_select_1;
    this->mux_select[2] = mux_select_2;
    this->mux_enable    = mux_enable;
    
    this->disableMultiplexer();
    this->eventSequence = new Sequence<HardwareEvent>();

    for(int i = 0; i < 5; i++) {
        this->knobValue[i] = 0;
    }
    for(int i = 0; i < 2; i++) {
        this->bankKnobPosition[i] = 0;
    }

    this->lastEventTime = millis();
}

unsigned char HardwareController::addEvent(HardwareEvent hardwareEvent) {
    return this->eventSequence->addToEnd(hardwareEvent);
}

void HardwareController::update() {
    if(this->eventSequence->getElementCount() == 0){
        return;
    }

    if(millis() - this->lastEventTime  < this->eventInterval){
        return;
    }
    
    unsigned char returnValue = 0; //0 == continue. 1 == stop.
    HardwareEvent activeEvent;
    //Grabs the event at the top, removing it from the list.
    //Executes this event, which may place new events in the list.
    //if new events were placed in this stack without rate-limited action, the event is deleted and the process repeated.
    //if no new events are added, just delete the event and end.

    while(returnValue == 0) {
        activeEvent = this->eventSequence->getAndRemoveFromBeginning();
        
        switch(activeEvent.type) {
            case SELECT_BANK:
                Serial.print("EVENT: SELECT BANK: " + String(activeEvent.arg0) + "\n");
                if(this->eventSequence->addToPosition(HardwareEvent{TURN_BANK_KNOB, 0, 0}, 0) == 0){
                    Serial.print("Failed to Add Event To Position: Sequence List Full\n");
                }
                if(this->eventSequence->addToPosition(HardwareEvent{TURN_BANK_KNOB, 0, activeEvent.arg0}, 1) == 0){
                    Serial.print("Failed to Add Event To Position: Sequence List Full\n");
                }
                returnValue = 0; //continue
                //returnValue = this->selectBank(activeEvent->arg0);
                break;
            case TURN_BANK_KNOB:
                Serial.print("EVENT: TURNING BANK KNOB: " + String(activeEvent.arg0) + " TO: " + String(activeEvent.arg1)  + "\n");
                returnValue = 1; //break
                break;
            case TURN_KNOB:
                Serial.print("EVENT: SET KNOB : " + String(activeEvent.arg0) + " TO VALUE: " + String(activeEvent.arg1)  + "\n");
                //returnValue = this->setKnobValue(activeEvent->arg0, activeEvent->arg1);
                returnValue = 1;
                break;
            default:
                Serial.print("EVENT: ?????? - Unknown event type! \n");
                returnValue = 1;
                break;
        }
    }

    this->lastEventTime = millis();
}

/*********************************************************************************
 * PRIVATE MEMBERS:
 *********************************************************************************/

void HardwareController::setKnobValue(char knobIndex, char value) {
    this->selectChip(4 - knobIndex);                    //Select the right digipot
    SPI.transfer(B00010001);                                //tell the digipot that we're sending it data...
    SPI.transfer(255 - value);                              //and send it the data.
    this->selectChip(4 - knobIndex);                    //cycle the multiplexer, which will set the digipots 
                                                              //CS from HIGH back to LOW, shifting the data in.
    this->disableMultiplexer();                             // !!!! Disable the mux (Might be unecessary????)
    this->knobValue[knobIndex] = value;
}

char HardwareController::getKnobValue(char knobIndex){
    return this->knobValue[knobIndex];
}


//* TODO: Make asynchronous
//  Will create a Sequence object that is added to the list of Sequences.
//  This sequence will contain at least two actions which will turn knobs.
void HardwareController::selectBank(char bank){
    if(this->getSelectedBank() == bank) {
        Serial.println("Bank Knob already selected. Cancelling.");
        return;
    }

    Serial.println("selectBank( " + String((int)bank) + " );");

    //Determine which knob will need to be turned:
    char bankKnobToBeTurned;
    char relativeBankPosition;

    if(bank < 11){
        bankKnobToBeTurned = BANKKNOB_0; //upper
        relativeBankPosition = bank;
    }else{
        bankKnobToBeTurned = BANKKNOB_1;
        relativeBankPosition = bank - 11;
    }

    //if the knob is already in the position...
    if(this->bankKnobPosition[bankKnobToBeTurned] == relativeBankPosition) {
        if(relativeBankPosition == 0){
            this->setBankKnobPosition(bankKnobToBeTurned, 1);
            delay(32);
        }else{
            this->setBankKnobPosition(bankKnobToBeTurned, 0);
            delay(32);
        }
    }
    this->setBankKnobPosition(bankKnobToBeTurned, relativeBankPosition);
    this->lastTurnedBankKnob = bankKnobToBeTurned;
}

char HardwareController::getSelectedBank() {
    if(this->lastTurnedBankKnob == BANKKNOB_0){
        return this->bankKnobPosition[BANKKNOB_0];
    }else{
        return this->bankKnobPosition[BANKKNOB_1] + 11;
    }
}

/* Button */
void HardwareController::pressButton(char buttonIndex) {
    //...
}

void HardwareController::releaseButton(char buttonIndex) {
    //...
}

/* MULTIPLEXER */

void HardwareController::selectChip(char chipIndex) {
    Serial.print("Setting multiplexer to value: " + String((int)chipIndex));
    Serial.print(" [");
    Serial.print("Pin " + String((int)this->mux_select[0]) + " = " + String((int) (1 & ((chipIndex) >> 0))));
    Serial.print("Pin " + String((int)this->mux_select[1]) + " = " + String((int) (1 & ((chipIndex) >> 1))));
    Serial.print("Pin " + String((int)this->mux_select[2]) + " = " + String((int) (1 & ((chipIndex) >> 2))));
    Serial.println("]");
    
    this->disableMultiplexer();
    digitalWrite((int)this->mux_select[0], (int)(1 & ((chipIndex) >> 0)));
    digitalWrite((int)this->mux_select[1], (int)(1 & ((chipIndex) >> 1)));
    digitalWrite((int)this->mux_select[2], (int)(1 & ((chipIndex) >> 2)));
    this->enableMultiplexer();
}

void HardwareController::enableMultiplexer() {
    digitalWrite(this->mux_enable, LOW);
}

void HardwareController::disableMultiplexer() {
    //sets all output pins on the mux chip to HIGH
    digitalWrite(this->mux_enable, HIGH); 
}

/* EDIT SELECT BANKS */

void HardwareController::sendByteToShiftRegister(char value) {
        Serial.print("Sending byte to shift register... [");
        Serial.print((int)(1 & ((value) >> 7)));
        Serial.print((int)(1 & ((value) >> 6)));
        Serial.print((int)(1 & ((value) >> 5)));
        Serial.print((int)(1 & ((value) >> 4)));
        Serial.print((int)(1 & ((value) >> 3)));
        Serial.print((int)(1 & ((value) >> 2)));
        Serial.print((int)(1 & ((value) >> 1)));
        Serial.print((int)(1 & ((value) >> 0)));
        Serial.print("]\n");
        
    this->selectChip(5); //select the bank controller shift register.
    SPI.transfer(value);
    this->disableMultiplexer(); //commit the values by disabling the mux.
}

void HardwareController::setBankKnobPosition(char bankKnobIndex, char relativePosition) {
    Serial.println("setBankKnobPosition( " + String((int)bankKnobIndex) + ", " + String((int)relativePosition) + " );");
    
    this->bankKnobPosition[bankKnobIndex] = relativePosition;
    this->sendByteToShiftRegister(((this->bankKnobPosition[0] << 4) | this->bankKnobPosition[1]));
}
