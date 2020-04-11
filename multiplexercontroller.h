#ifndef MultiplexerController_h
#define MultiplexerController_h
#include <Arduino.h> 

class MultiplexerController {
    private:
        char mux_pin[3];
        char mux_enable_pin;
    public:
        MultiplexerController(char mux_pin_0, char mux_pin_1, char mux_pin_2, char mux_enable_pin);
        void selectChip(char chipIndex);
        void disable();
        void enable();
};

#endif
