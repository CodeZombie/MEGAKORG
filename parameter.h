#ifndef Parameter_h
#define Parameter_h
#include <Arduino.h> 
#include "multiplexercontroller.h"
#include "bankcontroller.h"
#include "digipot.h"

class Parameter {
  public:
    Parameter(int bank, int knob, char value);
    int bank;
    int knob;
    char value;
    bool active;
    void setValue(MultiplexerController *multiplexerController, BankController *bankController, Digipot *digipot[], char value);
};

#endif
