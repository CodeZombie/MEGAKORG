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
    this->eventDeque = new Deque<HardwareEvent>();

    for(int i = 0; i < 5; i++) {
        this->knobValue[i] = 0;
    }
    for(int i = 0; i < 2; i++) {
        this->bankKnobPosition[i] = 0;
    }

    this->lastEventTime = millis();
}

unsigned char HardwareController::addEvent(HardwareEvent hardwareEvent) {
    return this->eventDeque->pushBack(hardwareEvent);
}

void HardwareController::update() {
    if(this->eventDeque->getElementCount() == 0){
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
        activeEvent = this->eventDeque->popFront();

        switch(activeEvent.type) {
            case SELECT_BANK:
                //fail if the deque doesnt have 2 free spots...
                if(this->eventDeque->getElementCount() < DEQUE_MAX_ELEMENTS - 2) {
                    this->selectBank(activeEvent.arg0);
                }else{
                    returnValue = 1;
                }
                break;
            case TURN_BANK_KNOB:
                this->turnBankKnob(activeEvent.arg0, activeEvent.arg1);
                //Serial.print("EVENT: TURNING BANK KNOB: " + String(activeEvent.arg0) + " TO: " + String(activeEvent.arg1)  + "\n");
                returnValue = 1; //break
                break;
            case TURN_KNOB:
                this->setKnobValue(activeEvent.arg0, activeEvent.arg1);
                //Serial.print("EVENT: SET KNOB : " + String(activeEvent.arg0) + " TO VALUE: " + String(activeEvent.arg1)  + "\n");
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

void HardwareController::selectBank(char bank){
    if(this->getSelectedBank() == bank) {
        Serial.println("Bank Knob already selected. Cancelling.");
        return;
    }
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

    this->eventDeque->pushFront(HardwareEvent{TURN_BANK_KNOB, bankKnobToBeTurned, relativeBankPosition});

    //if the knob is already in the position...
    if(this->bankKnobPosition[bankKnobToBeTurned] == relativeBankPosition) {
        if(relativeBankPosition == 0){
            this->eventDeque->pushFront(HardwareEvent{TURN_BANK_KNOB, bankKnobToBeTurned, 1});
        }else{
            this->eventDeque->pushFront(HardwareEvent{TURN_BANK_KNOB, bankKnobToBeTurned, 0});
        }
    }

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
    this->selectChip(5); //select the bank controller shift register.
    SPI.transfer(value);
    this->disableMultiplexer(); //commit the values by disabling the mux.
}

void HardwareController::turnBankKnob(char bankKnobIndex, char relativePosition) {
    //Serial.println("turnBankKnob( " + String((int)bankKnobIndex) + ", " + String((int)relativePosition) + " );");
    
    this->bankKnobPosition[bankKnobIndex] = relativePosition;
    this->sendByteToShiftRegister(((this->bankKnobPosition[0] << 4) | this->bankKnobPosition[1]));
}
