#ifndef HardwareController_h
#define HardwareController_h
#include "Sequence.h"
#include "SPI.h"

typedef enum editSelectBank { 
    EditSelectBank_1,
    EditSelectBank_2
} EditSelectBank;

typedef enum knob {
    Knob_0,
    Knob_1, 
    Knob_2,
    Knob_3,
    Knob_4
} Knob;

class HardwareController {
    public:
        HardwareController(char mux_select_0, char mux_select_1, char mux_select_2, char mux_enable);
        unsigned char addEvent(SequenceEvent sequenceEvent);    
        
        void setKnobValue(char knobIndex, char value);
        void selectEditSelectBank(char editSelectBank);
        char getSelectedEditSelectBank();
        char getKnobValue(char knobIndex);
        void pressButton(char buttonIndex);
        void releaseButton(char buttonIndex);
        void update();
        
        /* Event Sequencer */
        Sequence *sequence;

    private:
        /* Multiplexer */
        void setMultiplexer(char chipIndex);
        void disableMultiplexer();
        void enableMultiplexer();
        char mux_select[3];
        char mux_enable;

        /* Edit Select Banks */
        void sendByteToShiftRegister(char value);
        void setEditSelectBankPosition(char editSelectKnobIndex, char position); //sets a specific edit select knob to a value.
        char editSelectBankPosition[2];
        char lastTurnedEditSelectBankKnob;

        /* Knobs */
        char knobValue[5];

        /* Buttons */
        bool buttonPressed[16];
};

#endif
