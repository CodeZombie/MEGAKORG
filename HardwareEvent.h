#ifndef HardwareEvent_H
#define HardwareEvent_H

typedef enum hardwareEventType {
    NONE,
    SELECT_BANK,
    TURN_BANK_KNOB,
    TURN_KNOB,
    PRESS_BUTTON
} HardwareEventType;

struct HardwareEvent {
    unsigned char type;
    unsigned char arg0;
    unsigned char arg1;
};

#endif