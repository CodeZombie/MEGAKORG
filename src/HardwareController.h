#ifndef HardwareController_h
#define HardwareController_h
#include "Deque.h"
#include "HardwareEvent.h"
#include "SPI.h"
#include <map>
#include <string>

typedef enum bankKnob { 
    BANKKNOB_0,
    BANKKNOB_1,
    BANKKNOB_INVALID,
} BankKnob;

typedef enum bank {
    BANK_0,
    BANK_1,
    BANK_2,
    BANK_3,
    BANK_4,
    BANK_5,
    BANK_6,
    BANK_7,
    BANK_8,
    BANK_9,
    BANK_10,
    BANK_11,
    BANK_12,
    BANK_13,
    BANK_14,
    BANK_15,
    BANK_16,
    BANK_17,
    BANK_18,
    BANK_19,
    BANK_20,
    BANK_21,
    BANK_INVALID,
} Bank;

typedef enum knob {
    KNOB_0,
    KNOB_1, 
    KNOB_2,
    KNOB_3,
    KNOB_4,
    KNOB_INVALID,
} Knob;

// We have an enum of "parameters". These will all map to a specific number.
// When the client requests a list of parameters, we return a string, along with the associated enum number.
// Then, when the client makes a parameter change, it can submit just the number so no string comparison needs to happen.
// This way we don't keep strings in memory either (they're in the instructions, not regular ram).
// We just need a function that returns a bunch of json mapping a string to an enum.
typedef enum parameter {
    VOICE_SYNTH_VOCODER,
    VOICE_SINGLE_LAYER,
    VOICE_VOICE_ASSIGN,
    VOICE_TRIGGER_MODE,
    VOICE_UNISON_DETUNE,

    PITCH_TRANSPOSE,
    PITCH_TUNE,
    PITCH_PORTAMENTO,
    PITCH_BEND_RANGE,
    PITCH_VIBRATO_INT,
    
    OSC1_WAVE,
    OSC1_CONTROL_1,
    OSC1_CONTROL_2,

    OSC2_WAVE,
    OSC2_OSC_MOD,
    OSC2_SEMITONE,
    OSC2_TUNE,

    MIXER_OSC1_LEVEL,
    MIXER_OSC2_LEVEL,
    MIXER_NOISE_LEVEL,
    
    FILTER_TYPE,
    FILTER_CUTOFF,
    FILTER_RESONANCE,
    FILTER_FILTER_EG_INT,
    FILTER_FILTER_KEY_TRACK,

    FILTER_EG_ATTACK,
    FILTER_EG_DECAY,
    FILTER_EG_SUSTAIN,
    FILTER_EG_RELEASE,
    FILTER_EG_EG_RESET,

    AMP_LEVEL,
    AMP_PANPOT,
    AMP_DISTORTION,
    AMP_KBD_TRACK,

    AMP_EG_ATTACK,
    AMP_EG_DECAY,
    AMP_EG_SUSTAIN,
    AMP_EG_RELEASE,
    AMP_EG_EG_RESET,

    LFO1_WAVE,
    LFO1_KEY_SYNC,
    LFO1_TEMPO_SYNC,
    LFO1_FREQUENCY,

    LFO2_WAVE,
    LFO2_KEY_SYNC,
    LFO2_TEMPO_SYNC,
    LFO2_FREQUENCY,

    PATCH1_SOURCE,
    PATCH1_DEST,
    PATCH1_MOD_INT,
    PATCH1_BLANK,

    PATCH2_SOURCE,
    PATCH2_DEST,
    PATCH2_MOD_INT,
    PATCH2_BLANK,

    PATCH3_SOURCE,
    PATCH3_DEST,
    PATCH3_MOD_INT,
    PATCH3_BLANK,

    PATCH4_SOURCE,
    PATCH4_DEST,
    PATCH4_MOD_INT,
    PATCH4_BLANK,

    MODFX_TYPE,
    MODFX_LFO_SPEED,
    MODFX_EFFECT_DEPTH,

    DELAY_TYPE,
    DELAY_TEMPO_SYNC,
    DELAY_DELAY_TIME,
    DELAY_DELAY_DEPTH,

    EQ_LOW_EQ_FREQ,
    EQ_LOW_EQ_GAIN,
    EQ_HI_EQ_FREQ,
    EQ_HI_EQ_GAIN,

    ARPEG_A_TEMPO,
    ARPEG_A_RESOLUTION,
    ARPEG_A_DATE,
    ARPEG_A_TYPE,
    ARPEG_A_RANGE,

    ARPEG_B_LATCH,
    ARPEG_B_SWING,
    ARPEG_B_KEY_SYNC,
    ARPEG_B_LAST_STEP,
    ARPEG_B_TARGET_TIMBRE,

    GLOBAL_MASTER_TUNE,
    GLOBAL_MASTER_TRANSPOSE,
    GLOBAL_VELOCITY_CURVE,
    GLOBAL_POSITION,
    GLOBAL_AUDIO_IN_THRU,

    MIDI_MIDI_CH,
    MIDI_LOCAL,
    MIDI_CLOCK
} Parameter;

class HardwareController {
    public:
        HardwareController(char mux_select_0, char mux_select_1, char mux_select_2, char mux_enable);
        unsigned char addEvent(HardwareEvent hardwareEvent);
        void update();

        /* getters */
        char getSelectedBank();
        char getKnobValue(char knobIndex);

    private:
        /* PARAMETER RESOLVING */
        Bank getBankFromParameter(Parameter parameter);
        Knob getKnobFromParameter(Parameter parameter);
        /* Sequencer */
        const uint32_t eventInterval = 8;
        uint32_t lastEventTime;
        Deque<HardwareEvent> *eventDeque;

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
        void turnBankKnob(char bankKnobIndex, char relativePosition);
        char bankKnobPosition[2];
        char lastTurnedBankKnob;

        /* Knobs */
        void setKnobValue(char knobIndex, unsigned char value);
        char knobValue[5];

        /* Virtual Knobs */
        char virtualKnobValue[22 * 5];

        /* Buttons */
        void pressButton(char buttonIndex);
        void releaseButton(char buttonIndex);
        bool buttonPressed[16];
};

#endif
