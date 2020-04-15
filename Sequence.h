#ifndef Sequence_h
#define Sequence_h

typedef enum sequenceEventType {
    NONE,
    SELECT_BANK,
    TURN_BANK_KNOB,
    TURN_KNOB,
    PRESS_BUTTON
} SequenceEventType;

struct SequenceEvent {
    SequenceEventType type;
    unsigned char arg0;
    unsigned char arg1;
};

class Sequence {
    public:
        Sequence();
        unsigned char addEventToPosition(SequenceEvent sequenceEvent, int position);
        unsigned char addEventToBottom(SequenceEvent sequenceEvent);
        SequenceEvent getAndRemoveFromTop();
        bool isEmpty();

        SequenceEvent sequenceEvent[32]; //should be private!!!!!!!!!!
    private:
        
};

#endif
