#ifndef BankController_h
#define BankController_h
#include <Arduino.h> 
#include "SPI.h"
#include "multiplexercontroller.h"

//will handle all methods/functionality regarding changing parameter banks.

typedef enum bankknob { kn_upper,
                        kn_lower} BankKnob;

class BankController {
    private:
        char upperBankPosition;
        char lowerBankPosition;
        BankKnob lastTurnedBankKnob;
        void sendByteToShiftRegister(MultiplexerController *multiplexerController, char value);
        void setUpperBankPosition(MultiplexerController *multiplexerController, char value);
        void setLowerBankPosition(MultiplexerController *multiplexerController, char value);

    public:
        BankController(MultiplexerController *multiplexerController);
        void selectBank(MultiplexerController *multiplexerController, char index);
        char getSelectedBank();
};

#endif
