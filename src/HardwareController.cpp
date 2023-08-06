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

    if(millis() - this->lastEventTime < this->eventInterval){
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
                Serial.print(String(millis()) + ": Selecting bank: " + String(activeEvent.arg0) + "\n");
                //fail if the deque doesnt have 2 free spots...
                //if(this->eventDeque->getElementCount() < DEQUE_MAX_ELEMENTS - 2) {
                this->selectBank(activeEvent.arg0);
                returnValue = 1;
                //}
                break;
            case TURN_BANK_KNOB:
                Serial.print(String(millis()) + ": Turning Bank Knob: " + String(activeEvent.arg0) + " to " + String(activeEvent.arg1) + "\n");
                this->turnBankKnob(activeEvent.arg0, activeEvent.arg1);
                returnValue = 1; //break
                break;
            case TURN_KNOB:
                Serial.print(String(millis()) + ": Attempting to turn Knob: " + String((this->getSelectedBank() * 5) + activeEvent.arg0) + " to " + String(activeEvent.arg1) + "\n");
                if(this->knobValue[activeEvent.arg0] != this->virtualKnobValue[(this->getSelectedBank() * 5) + activeEvent.arg0]) {
                    // Reset this physical knob so that it matches the virtual knobs "position"
                    Serial.println("Resetting physical knob from (" + String(int(this->knobValue[activeEvent.arg0])) + ") to (" + String(int(this->virtualKnobValue[(this->getSelectedBank() * 5) + activeEvent.arg0])) + ")");
                    this->setKnobValue(activeEvent.arg0, this->virtualKnobValue[(this->getSelectedBank() * 5) + activeEvent.arg0]);
                    this->eventDeque->pushFront(HardwareEvent{TURN_KNOB, (unsigned char)activeEvent.arg0, (unsigned char)activeEvent.arg1});
                }else{
                    this->setKnobValue(activeEvent.arg0, activeEvent.arg1);
                }
                returnValue = 1;
                break;
            default:
                Serial.print(String(millis()) + ": EVENT:" + String(activeEvent.type) + " - Unknown event type! \n");
                returnValue = 1;
                break;
        }
    }

    this->lastEventTime = millis();
}

/*********************************************************************************
 * PRIVATE MEMBERS:
 *********************************************************************************/

void HardwareController::setKnobValue(char knobIndex, unsigned char value) {
    this->selectChip(4 - knobIndex);                        //Select the right digipot
    SPI.transfer(B00010001);                                //tell the digipot that we're sending it data...
    SPI.transfer(255 - value);                              //and send it the data.
    this->selectChip(4 - knobIndex);                        //cycle the multiplexer, which will set the digipots 
                                                            //CS from HIGH back to LOW, shifting the data in.
    this->disableMultiplexer();                             // !!!! Disable the mux (Might be unecessary????)
    Serial.println("Setting hardware knob " + String(int(knobIndex)) + " to " + String(value)); 
    this->knobValue[knobIndex] = value;
    Serial.println("Setting virtual knob " + String((this->getSelectedBank() * 5) + knobIndex) + " to " + String(value));
    this->virtualKnobValue[(this->getSelectedBank() * 5) + knobIndex] = value;//Keep track of this value in the virtual knobs
}

char HardwareController::getKnobValue(char knobIndex){
    return this->knobValue[knobIndex];
}

void HardwareController::selectBank(char bank){
    // if(this->getSelectedBank() == bank) {
    //     Serial.println("Bank Knob already selected. Cancelling.");
    //     return;
    // }
    //Determine which knob will need to be turned:
    unsigned char bankKnobToBeTurned;
    unsigned char relativeBankPosition;

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

Bank HardwareController::getBankFromParameter(parameter parameter) {
    switch (parameter) {
        case VOICE_SYNTH_VOCODER:
        case VOICE_SINGLE_LAYER:
        case VOICE_VOICE_ASSIGN:
        case VOICE_TRIGGER_MODE:
        case VOICE_UNISON_DETUNE:
            return BANK_0;
        case PITCH_TRANSPOSE:
        case PITCH_TUNE:
        case PITCH_PORTAMENTO:
        case PITCH_BEND_RANGE:
        case PITCH_VIBRATO_INT:
            return BANK_1;
        case OSC1_WAVE:
        case OSC1_CONTROL_1:
        case OSC1_CONTROL_2:
            return BANK_2;
        case OSC2_WAVE:
        case OSC2_OSC_MOD:
        case OSC2_SEMITONE:
        case OSC2_TUNE:
            return BANK_3;
        case MIXER_OSC1_LEVEL:
        case MIXER_OSC2_LEVEL:
        case MIXER_NOISE_LEVEL:
            return BANK_4;
        case FILTER_TYPE:
        case FILTER_CUTOFF:
        case FILTER_RESONANCE:
        case FILTER_FILTER_EG_INT:
        case FILTER_FILTER_KEY_TRACK:
            return BANK_5;
        case FILTER_EG_ATTACK:
        case FILTER_EG_DECAY:
        case FILTER_EG_SUSTAIN:
        case FILTER_EG_RELEASE:
        case FILTER_EG_EG_RESET:
            return BANK_6;
        case AMP_LEVEL:
        case AMP_PANPOT:
        case AMP_DISTORTION:
        case AMP_KBD_TRACK:
            return BANK_7;
        case AMP_EG_ATTACK:
        case AMP_EG_DECAY:
        case AMP_EG_SUSTAIN:
        case AMP_EG_RELEASE:
        case AMP_EG_EG_RESET:
            return BANK_8;
        case LFO1_WAVE:
        case LFO1_KEY_SYNC:
        case LFO1_TEMPO_SYNC:
        case LFO1_FREQUENCY:
            return BANK_9;
        case LFO2_WAVE:
        case LFO2_KEY_SYNC:
        case LFO2_TEMPO_SYNC:
        case LFO2_FREQUENCY:
            return BANK_10;
        case PATCH1_SOURCE:
        case PATCH1_DEST:
        case PATCH1_MOD_INT:
        case PATCH1_BLANK:
            return BANK_11;
        case PATCH2_SOURCE:
        case PATCH2_DEST:
        case PATCH2_MOD_INT:
        case PATCH2_BLANK:
            return BANK_12;
        case PATCH3_SOURCE:
        case PATCH3_DEST:
        case PATCH3_MOD_INT:
        case PATCH3_BLANK:
            return BANK_13;
        case PATCH4_SOURCE:
        case PATCH4_DEST:
        case PATCH4_MOD_INT:
        case PATCH4_BLANK:
            return BANK_14;
        case MODFX_TYPE:
        case MODFX_LFO_SPEED:
        case MODFX_EFFECT_DEPTH:
            return BANK_15;
        case DELAY_TYPE:
        case DELAY_TEMPO_SYNC:
        case DELAY_DELAY_TIME:
        case DELAY_DELAY_DEPTH:
            return BANK_16;
        case EQ_LOW_EQ_FREQ:
        case EQ_LOW_EQ_GAIN:
        case EQ_HI_EQ_FREQ:
        case EQ_HI_EQ_GAIN:
            return BANK_17;
        case ARPEG_A_TEMPO:
        case ARPEG_A_RESOLUTION:
        case ARPEG_A_DATE:
        case ARPEG_A_TYPE:
        case ARPEG_A_RANGE:
            return BANK_18;
        case ARPEG_B_LATCH:
        case ARPEG_B_SWING:
        case ARPEG_B_KEY_SYNC:
        case ARPEG_B_LAST_STEP:
        case ARPEG_B_TARGET_TIMBRE:
            return BANK_19;
        case GLOBAL_MASTER_TUNE:
        case GLOBAL_MASTER_TRANSPOSE:
        case GLOBAL_VELOCITY_CURVE:
        case GLOBAL_POSITION:
        case GLOBAL_AUDIO_IN_THRU:
            return BANK_20;
        case MIDI_MIDI_CH:
        case MIDI_LOCAL:
        case MIDI_CLOCK:
            return BANK_21;
        default:
            Serial.println("Error getting bank from parameter. Unknown parameter: " + String((int)parameter));
            return BANK_INVALID; // this is an error :)
    }
}

Knob HardwareController::getKnobFromParameter(parameter parameter){
    switch(parameter) {
        case VOICE_SYNTH_VOCODER:
        case PITCH_TRANSPOSE:
        case OSC1_WAVE:
        case OSC2_WAVE:
        case MIXER_OSC1_LEVEL:
        case FILTER_TYPE:
        case FILTER_EG_ATTACK:
        case AMP_LEVEL:
        case AMP_EG_ATTACK:
        case LFO1_WAVE:
        case LFO2_WAVE:
        case PATCH1_SOURCE:
        case PATCH2_SOURCE:
        case PATCH3_SOURCE:
        case PATCH4_SOURCE:
        case MODFX_TYPE:
        case DELAY_TYPE:
        case EQ_LOW_EQ_FREQ:
        case ARPEG_A_TEMPO:
        case ARPEG_B_LATCH:
        case GLOBAL_MASTER_TUNE:
        case MIDI_MIDI_CH:
            return KNOB_0;
        case VOICE_SINGLE_LAYER:
        case PITCH_TUNE:
        case OSC1_CONTROL_1:
        case OSC2_OSC_MOD:
        case MIXER_OSC2_LEVEL:
        case FILTER_CUTOFF:
        case FILTER_EG_DECAY:
        case AMP_PANPOT:
        case AMP_EG_DECAY:
        case LFO1_KEY_SYNC:
        case LFO2_KEY_SYNC:
        case PATCH1_DEST:
        case PATCH2_DEST:
        case PATCH3_DEST:
        case PATCH4_DEST:
        case MODFX_LFO_SPEED:
        case DELAY_TEMPO_SYNC:
        case EQ_LOW_EQ_GAIN:
        case ARPEG_A_RESOLUTION:
        case ARPEG_B_SWING:
        case GLOBAL_MASTER_TRANSPOSE:
        case MIDI_LOCAL:
            return KNOB_1;
        case VOICE_VOICE_ASSIGN:
        case PITCH_PORTAMENTO:
        case OSC1_CONTROL_2:
        case OSC2_SEMITONE:
        case MIXER_NOISE_LEVEL:
        case FILTER_RESONANCE:
        case FILTER_EG_SUSTAIN:
        case AMP_DISTORTION:
        case AMP_EG_SUSTAIN:
        case LFO1_TEMPO_SYNC:
        case LFO2_TEMPO_SYNC:
        case PATCH1_MOD_INT:
        case PATCH2_MOD_INT:
        case PATCH3_MOD_INT:
        case PATCH4_MOD_INT:
        case MODFX_EFFECT_DEPTH:
        case DELAY_DELAY_TIME:
        case EQ_HI_EQ_FREQ:
        case ARPEG_A_DATE:
        case ARPEG_B_KEY_SYNC:
        case GLOBAL_VELOCITY_CURVE:
        case MIDI_CLOCK:
            return KNOB_2;
        case VOICE_TRIGGER_MODE:
        case PITCH_BEND_RANGE:
        case OSC2_TUNE:
        case FILTER_FILTER_EG_INT:
        case FILTER_EG_RELEASE:
        case AMP_KBD_TRACK:
        case AMP_EG_RELEASE:
        case LFO1_FREQUENCY:
        case LFO2_FREQUENCY:
        case PATCH1_BLANK:
        case PATCH2_BLANK:
        case PATCH3_BLANK:
        case PATCH4_BLANK:
        case DELAY_DELAY_DEPTH:
        case EQ_HI_EQ_GAIN:
        case ARPEG_A_TYPE:
        case ARPEG_B_LAST_STEP:
        case GLOBAL_POSITION:
            return KNOB_3;
        case VOICE_UNISON_DETUNE:
        case PITCH_VIBRATO_INT:
        case FILTER_FILTER_KEY_TRACK:
        case FILTER_EG_EG_RESET:
        case AMP_EG_EG_RESET:
        case ARPEG_A_RANGE:
        case ARPEG_B_TARGET_TIMBRE:
        case GLOBAL_AUDIO_IN_THRU:
            return KNOB_4;
        default:
            Serial.println("Error getting knob from parameter. Unknown parameter: " + String((int)parameter));
            return KNOB_INVALID;
    }
}