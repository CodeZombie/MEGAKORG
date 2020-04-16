#ifndef HardwareController_h
#define HardwareController_h
#include "Sequence.h"
#include "HardwareEvent.h"
#include "SPI.h"

typedef enum bankKnob { 
    BANKKNOB_0,
    BANKKNOB_1
} BankKnob;

typedef enum knob {
    KNOB_0,
    KNOB_1, 
    KNOB_2,
    KNOB_3,
    KNOB_4
} Knob;

class HardwareController {
    public:
        HardwareController(char mux_select_0, char mux_select_1, char mux_select_2, char mux_enable);
        unsigned char addEvent(HardwareEvent hardwareEvent);
        void update();

        /* getters */
        char getSelectedBank();
        char getKnobValue(char knobIndex);

    private:
        /* Sequencer */
        const uint32_t eventInterval = 1000;
        uint32_t lastEventTime;
        Sequence<HardwareEvent> *eventSequence;

        /* Multiplexer */
        void selectChip(char chipIndex);
        void disableMultiplexer();
        void enableMultiplexer();
        char mux_select[3];
        char mux_enable;

        /* ICs */
        void sendByteToShiftRegister(char value);

        /* Edit Select Banks */
        void selectBank(char bank); //rename selectBank
        void setBankKnobPosition(char bankKnobIndex, char relativePosition);
        char bankKnobPosition[2];
        char lastTurnedBankKnob;

        /* Knobs */
        void setKnobValue(char knobIndex, char value);
        char knobValue[5];

        /* Buttons */
        void pressButton(char buttonIndex);
        void releaseButton(char buttonIndex);
        bool buttonPressed[16];
};

#endif
