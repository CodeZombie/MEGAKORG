#include "microkorg.h"

Microkorg *microkorg;
byte test_index = 1;

void setup() {
  microkorg = new Microkorg(2, 3, 4, 5);
}

void loop() {
  microkorg->selectBank(fuck);
  for(int i = 0; i < 255; i++) {
    microkorg->setDigipotValue(ch_knob_0, i); 
    delay(1);
  }
  for(int x = 255; x > 0; x--) {
    microkorg->setDigipotValue(ch_knob_0, x); 
    delay(1);
  }
  
  
  if(test_index == 1) {
    test_index = 5; //FILTER  -  -24dB LPF, -12dB LPF, -12dB BPF, -12dB HPF 
  }else if(test_index == 5) { 
    test_index = 9; //LFO 1;  -  Saw, Square1, Triangle, Sample&Hold
  }else if( test_index == 9) {
    test_index = 15; // MOD FX - Chorus/Flanger, Ensemble, Phaser
  }else if(test_index == 15) {
    test_index = 10; //OSC 1  -  -24 - +24
  }else if(test_index == 10){
    test_index = 11;
  }else if(test_index == 11){
    test_index = 1;
  }
}
