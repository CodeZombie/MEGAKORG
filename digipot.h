#ifndef Digipot_h
#define Digipot_h
#include <Arduino.h> 
#include "SPI.h"
#include "multiplexercontroller.h"


class Digipot {
    private:
        char index;
        char value;
    public:
        Digipot(MultiplexerController *multiplexerController, char index);
        void setValue(MultiplexerController *multiplexerController, char value);
        char getValue();
};

#endif
