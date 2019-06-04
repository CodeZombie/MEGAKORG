#ifndef Microkorg_h
#define Microkorg_h
 #import <Arduino.h> 

typedef enum paramaterBank {  pb_voice,
                              pb_pitch,
                              pb_osc_1,
                              pb_osc_2,
                              pb_mixer,
                              pb_filter,
                              pb_filter_eg,
                              pb_amp,
                              pb_amp_eg,
                              pb_lfo_1,
                              pb_lfo_2,
                              // bank 2
                              pb_patch_1,
                              pb_patch_2,
                              pb_patch_3,
                              pb_patch_4,
                              pb_mod_fx,
                              pb_delay,
                              pb_eq,
                              pb_arpeg_a,
                              pb_arpeg_b,
                              pb_global,
                              pb_midi} ParameterBank;
typedef enum chip { ch_knob_4,
                    ch_knob_3,
                    ch_knob_2,
                    ch_knob_1,
                    ch_knob_0,
                    ch_shift_register,
                    ch_blank_0,
                    ch_blank_1} Chip;

typedef enum bankknob { kn_upper,
                        kn_lower} BankKnob;

class Microkorg {
  public:
    Microkorg(int mux_pin_0, int mux_pin_1, int mux_pin_2, int mux_enable_pin);
    //void selectParameterBank(int knob, int index);
    void selectBank(char bank_);
    void setDigipotValue(Chip digipot_index, char value);
  
  private:
    void selectChip(int index);
    void sendByteToShiftRegister(char value);
    void shiftOutByte(char value); 
      
    /*void shiftByte(byte value);*/
    int mux_pin[3];
    int mux_enable_pin;
    int spi_clock_pin = 13;
    int spi_data_pin = 11;
    
    char currentKnobPositions = B00000000;
    BankKnob lastTurnedKnob = kn_upper;
};

#endif



/*
 * initialize() //initializes all values to a determined preset (some patch)
 * loadPatch(patch);
 * setParameter(parameter, value);
 * getParameter(parameter);
 * 
 */
