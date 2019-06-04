
# MEGAKORG

## WHAT IS THIS?
The software and hardware specifics for a programmable system that can control the Korg Microkorg synthesizer.

## WHY?

The Microkorg is a really great synth. Cheap, sounds very nice, tons of features, available everywhere, used in great musical projects, and chances are, you know somebody that has one. The only problem is, the interface is absolute trash. Two bank knobs control what five potentiometers control. A giant, hard to read matrix tells you where each knob needs to be in order to change the sound you want. It's not intuitive, it's not fun to play with, and it's impossible to change two knobs at the same time if they exist in two separate banks. I started this project with the hope of using the Microkorg and cheap, common parts, to build a synthesizer that I really wanted to play.

## HOW DOES IT WORK?
After a bit of research and reverse engineering, I discovered it would be fairly straightforward to create a simple digital interface with a minimum of devices:

1. Five digitally-contorolled potentiometer chips onto the board to control the paramter knobs,

2. Two analog multiplexers to control the bank knobs,

3. Twelve optocouplers to control the buttons, and

4. An arduino to control all those controls

With these, one could write software to send any parameter change to the microkorg as fast as the SPI bus or microkorg hardware will allow, which from my initial testing, is extremely fast. From there, I plan on creating a full physical interface with enough rotary encoders to control every single individual parameter on the microkorg, finally opening the system up the musical expression it is capable of producing.
## OTHER POSSIBLE FEATURES?
By opening the system up to user-programming, the chip will allow the Microkorg to do things that simply were not possible before. For example:
1. Managing/sharing patches easily via WiFi via an ESP8266. Patches can be saved instantly with the press of a single button, and stored in a human readable json format.
2. Creating new envelopes/LFOs that can change parameters as if you had a third invisible hand moving a virtual knob.
3. Controlling parameters via non-knob controls, such as proximity sensors, or photo resistors.
4. More!

## WHY NOT JUST BUY AN MS2000?
The MS2000 is much more expensive and hard to find than the Microkorg. Additionally, the MS2000 is not programmable, meaning you get the full knob control, but thats it. No custom parameter controls, and you're still stuck with the default horrible patch loading mechanism.
