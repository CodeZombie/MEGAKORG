#include "Sequence.h"

Sequence::Sequence() {
    for(int i = 0; i < 32; i++){
        this->sequenceEvent[i] = SequenceEvent{NONE, 0, 0};
    }
}

unsigned char Sequence::addEventToPosition(SequenceEvent sequenceEvent, int position){
    if(this->sequenceEvent[31].type != NONE){ //if this array is full...
        return 0;
    }

    for(int i = 31; i >=position; i--){
        this->sequenceEvent[i] = (i == position) ? sequenceEvent : this->sequenceEvent[i - 1];
    }
    return 1;
}


unsigned char Sequence::addEventToBottom(SequenceEvent sequenceEvent) {
    for(int i = 0; i < 32; i++){
        if(this->sequenceEvent[i].type == NONE) {
            this->sequenceEvent[i] = sequenceEvent;
            return 1;
        }
    }
    //Serial.println("AddEvent() failed: Could not find nullptr.");
    return 0;//failed
}


SequenceEvent Sequence::getAndRemoveFromTop() {
    SequenceEvent topEvent = this->sequenceEvent[0];
    for(int i = 0; i < 32; i++) {
        this->sequenceEvent[i] = (i == 31) ? SequenceEvent{NONE, 0, 0} : this->sequenceEvent[i+1];
    }
    return topEvent;
}

bool Sequence::isEmpty() {
    return this->sequenceEvent[0].type == NONE;
}
