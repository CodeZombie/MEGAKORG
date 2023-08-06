#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Arduino.h> 
#include "HardwareEvent.h"
#include "HardwareController.h"
#include "SPI.h"
#include "webpages.h"

#define PARAM_YAML "parameters:\n\
  Voice:\n\
    SYNTH_VOCODER:  " + String(VOICE_SYNTH_VOCODER) + "\n\
    SINGLE_LAYER: " + String(VOICE_SINGLE_LAYER) + "\n\
    TRIGGER_MODE: " + String(VOICE_TRIGGER_MODE) + "\n\
    UNISON_DETUNE: " + String(VOICE_UNISON_DETUNE) + "\n\
  Pitch:\n\
    TRANSPOSE: " + String(PITCH_TRANSPOSE) + "\n\
    TUNE: " + String(PITCH_TUNE) + "\n\
    PORTAMENTO: " + String(PITCH_PORTAMENTO) + "\n\
    BEND_RANGE: " + String(PITCH_BEND_RANGE) + "\n\
    VIBRATO_INT: " + String(PITCH_VIBRATO_INT) + "\n\
  Osc1:\n\
    WAVE: " + String(OSC1_WAVE) + "\n\
    CONTROL_1: " + String(OSC1_CONTROL_1) + "\n\
    CONTROL_2: " + String(OSC1_CONTROL_2) + "\n\
  Osc2:\n\
    WAVE: " + String(OSC2_WAVE) + "\n\
    OSC_MOD: " + String(OSC2_OSC_MOD) + "\n\
    SEMITONE: " + String(OSC2_SEMITONE) + "\n\
    TUNE: " + String(OSC2_TUNE) + "\n\
  Mixer:\n\
    OSC1_LEVEL: " + String(MIXER_OSC1_LEVEL) + "\n\
    OSC2_LEVEL: " + String(MIXER_OSC2_LEVEL) + "\n\
    NOISE_LEVEL: " + String(MIXER_NOISE_LEVEL) + "\n\
  Filter:\n\
    TYPE: " + String(FILTER_TYPE) + "\n\
    CUTOFF: " + String(FILTER_CUTOFF) + "\n\
    RESONANCE: " + String(FILTER_RESONANCE) + "\n\
    FILTER_EG_INT: " + String(FILTER_FILTER_EG_INT) + "\n\
    FILTER_KEY_TRACK: " + String(FILTER_FILTER_KEY_TRACK) + "\n\
  Filter Envelope:\n\
    ATTACK: " + String(FILTER_EG_ATTACK) + "\n\
    DECAY: " + String(FILTER_EG_DECAY) + "\n\
    SUSTAIN: " + String(FILTER_EG_SUSTAIN) + "\n\
    RELEASE: " + String(FILTER_EG_RELEASE) + "\n\
    EG_RESET: " + String(FILTER_EG_EG_RESET) + "\n\
  Amp: \n\
    LEVEL: " + String(AMP_LEVEL) + "\n\
    PANPOT: " + String(AMP_PANPOT) + "\n\
    DISTORTION: " + String(AMP_DISTORTION) + "\n\
    KBD_TRACK: " + String(AMP_KBD_TRACK) + "\n\
  Amp Envelope: \n\
    ATTACK: " + String(AMP_EG_ATTACK) + "\n\
    DECAY: " + String(AMP_EG_DECAY) + "\n\
    SUSTAIN: " + String(AMP_EG_SUSTAIN) + "\n\
    RELEASE: " + String(AMP_EG_RELEASE) + "\n\
    EG_RESET: " + String(AMP_EG_EG_RESET) + "\n\
  LFO1: \n\
    WAVE: " + String(LFO1_WAVE) + "\n\
    KEY_SYNC: " + String(LFO1_KEY_SYNC) + "\n\
    TEMPO_SYNC: " + String(LFO1_TEMPO_SYNC) + "\n\
    FREQUENCY: " + String(LFO1_FREQUENCY) + "\n\
  LFO2: \n\
    WAVE: " + String(LFO2_WAVE) + "\n\
    KEY_SYNC: " + String(LFO2_KEY_SYNC) + "\n\
    TEMPO_SYNC: " + String(LFO2_TEMPO_SYNC) + "\n\
    FREQUENCY: " + String(LFO2_FREQUENCY) + "\n\
  Patch1: \n\
    SOURCE: " + String(PATCH1_SOURCE) + "\n\
    DEST: " + String(PATCH1_DEST) + "\n\
    MOD_INT: " + String(PATCH1_MOD_INT) + "\n\
    BLANK: " + String(PATCH1_BLANK) + "\n\
  Patch2: \n\
    SOURCE: " + String(PATCH2_SOURCE) + "\n\
    DEST: " + String(PATCH2_DEST) + "\n\
    MOD_INT: " + String(PATCH2_MOD_INT) + "\n\
    BLANK: " + String(PATCH2_BLANK) + "\n\
  Patch3: \n\
    SOURCE: " + String(PATCH3_SOURCE) + "\n\
    DEST: " + String(PATCH3_DEST) + "\n\
    MOD_INT: " + String(PATCH3_MOD_INT) + "\n\
    BLANK: " + String(PATCH3_BLANK) + "\n\
  Patch4: \n\
    SOURCE: " + String(PATCH4_SOURCE) + "\n\
    DEST: " + String(PATCH4_DEST) + "\n\
    MOD_INT: " + String(PATCH4_MOD_INT) + "\n\
    BLANK: " + String(PATCH4_BLANK) + "\n\
  ModFX: \n\
    TYPE: " + String(MODFX_TYPE) + "\n\
    LFO_SPEED: " + String(MODFX_LFO_SPEED) + "\n\
    EFFECT_DEPTH: " + String(MODFX_EFFECT_DEPTH) + "\n\
  Delay Type: \n\
    TYPE: " + String(DELAY_TYPE) + "\n\
    TEMPO_SYNC: " + String(DELAY_TEMPO_SYNC) + "\n\
    DELAY_TIME: " + String(DELAY_DELAY_TIME) + "\n\
    DELAY_DEPTH: " + String(DELAY_DELAY_DEPTH) + "\n\
  Equalizer: \n\
    LOW_EQ_FREQ: " + String(EQ_LOW_EQ_FREQ) + "\n\
    LOW_EQ_GAIN: " + String(EQ_LOW_EQ_GAIN) + "\n\
    HI_EQ_FREQ: " + String(EQ_HI_EQ_FREQ) + "\n\
    HI_EQ_GAIN: " + String(EQ_HI_EQ_GAIN) + "\n\
  Arpeggiator A: \n\
    TEMPO: " + String(ARPEG_A_TEMPO) + "\n\
    RESOLUTION: " + String(ARPEG_A_RESOLUTION) + "\n\
    DATE: " + String(ARPEG_A_DATE) + "\n\
    TYPE: " + String(ARPEG_A_TYPE) + "\n\
    RANGE: " + String(ARPEG_A_RANGE) + "\n\
  Arpeggiator B: \n\
    LATCH: " + String(ARPEG_B_LATCH) + "\n\
    SWING: " + String(ARPEG_B_SWING) + "\n\
    KEY_SYNC: " + String(ARPEG_B_KEY_SYNC) + "\n\
    LAST_STEP: " + String(ARPEG_B_LAST_STEP) + "\n\
    TARGET_TIMBRE: " + String(ARPEG_B_TARGET_TIMBRE) + "\n\
  Global: \n\
    GLOBAL_MASTER_TUNE: " + String(GLOBAL_MASTER_TUNE) + "\n\
    MASTER_TRANSPOSE: " + String(GLOBAL_MASTER_TRANSPOSE) + "\n\
    VELOCITY_CURVE: " + String(GLOBAL_VELOCITY_CURVE) + "\n\
    POSITION: " + String(GLOBAL_POSITION) + "\n\
    AUDIO_IN_THRU: " + String(GLOBAL_AUDIO_IN_THRU) + "\n\
  MIDI: \n\
    MIDI_CH: " + String(MIDI_MIDI_CH) + "\n\
    LOCAL: " + String(MIDI_LOCAL) + "\n\
    CLOCK: " + String(MIDI_CLOCK) + "\n"

ESP8266WebServer server(80);
HardwareController *hardwareController;

void setup() {
  Serial.begin(115200);

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

  Serial.print("Soft-AP Dick Penis Gay SEx IP address = ");
  Serial.println(WiFi.softAPIP());

   //HARDWARE SPI:
  SPI.begin();
  SPI.setFrequency(8000000); //8000000

  server.on("/", handleRoot);
  //server.on("/changeParam", changeParam);
  server.on("/turnKnob", turnKnob);
  server.on("/selectBank", selectBank);
  server.on("/special", handleSpecial);
  server.on("/reset", resetAll);
  server.on("/sweep", sweep);
  server.on("/param_list", httpReturnParamList);
  server.begin();
  
  hardwareController = new HardwareController(4, 0, 2, 5);
}

void loop() {
  server.handleClient();
  hardwareController->update();
}

void httpReturnParamList() {
  Serial.println(PARAM_YAML);
  server.send(200, "text/json", "Smina :3");

}


void handleSpecial() {
  hardwareController->addEvent(HardwareEvent{SELECT_BANK, 0});
  hardwareController->addEvent(HardwareEvent{TURN_KNOB, 0, 0});
  hardwareController->addEvent(HardwareEvent{TURN_KNOB, 0, 255});
  hardwareController->addEvent(HardwareEvent{SELECT_BANK, 4});
  hardwareController->addEvent(HardwareEvent{TURN_KNOB, 0, 0});
  hardwareController->addEvent(HardwareEvent{TURN_KNOB, 0, 255});
}

void turnKnob() {
  String knob = server.arg("knob");
  String value = server.arg("value");
  
  hardwareController->addEvent(HardwareEvent{TURN_KNOB, (unsigned char)knob.toInt(), (unsigned char)value.toInt()});
  //hardwareController->setKnobValue(knob.toInt(), value.toInt());
  server.send(200, "text/plain", value); //(int)hardwareController->getKnobValue(knob.toInt())
}

void resetAll() {
  for (unsigned char i = 0; i < 22; i++) {
    Serial.println("Selecting bank " + String(i));
    hardwareController->addEvent(HardwareEvent{SELECT_BANK, i, 0});

    for (unsigned char j = 0; j < 6; j++) {
      for (unsigned char k = 0; k < 255; k += 15) {
        Serial.println("Turning knob " + String(j) + " " + String(k));
        hardwareController->addEvent(HardwareEvent{TURN_KNOB, j, k});
      }
    }
  }
}

void sweep() {
  String knob = server.arg("knob");
  for (unsigned char i = 0; i <= 17; i++) {
    hardwareController->addEvent(HardwareEvent{TURN_KNOB, (unsigned char)knob.toInt(), (unsigned char)(i * 15)});
  }
}

void selectBank() {
  String value = server.arg("value");
  hardwareController->addEvent(HardwareEvent{SELECT_BANK, (unsigned char)value.toInt()});
  //hardwareController->selectEditSelectBank(value.toInt());
  server.send(200, "text/plain", value);
  //server.send(200, "text/plain", String((int)hardwareController->getSelectedEditSelectBank()));
}

void handleRoot() {
  server.send(200, "text/html", Resources::index_html);
}

/*
void changeParam() {
  String parameter = server.arg("parameter");
  String value = server.arg("value");

  microkorg->setParameter(parameter.toInt(), value.toInt());

  server.send(200, "text/plain", "success");
}
*/
