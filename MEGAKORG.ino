#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Arduino.h> 
#include "HardwareEvent.h"
#include "HardwareController.h"
#include "SPI.h"
#include "webpages.h"


ESP8266WebServer server(80);
HardwareController *hardwareController;

void setup() {
  Serial.begin(115200);

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

   //HARDWARE SPI:
  SPI.begin();
  SPI.setFrequency(8000000); //8000000

  server.on("/", handleRoot);
  //server.on("/changeParam", changeParam);
  server.on("/turnKnob", turnKnob);
  server.on("/selectBank", selectBank);
  server.begin();
  
  hardwareController = new HardwareController(4, 0, 2, 5);
}

void loop() {
  server.handleClient();
  hardwareController->update();
}

void turnKnob() {
  String knob = server.arg("knob");
  String value = server.arg("value");
  
  hardwareController->addEvent(HardwareEvent{TURN_KNOB, knob.toInt(), value.toInt()});
  //hardwareController->setKnobValue(knob.toInt(), value.toInt());
  server.send(200, "text/plain", String((int)hardwareController->getKnobValue(knob.toInt())));
}

void selectBank() {
  String value = server.arg("value");
  hardwareController->addEvent(HardwareEvent{SELECT_BANK, value.toInt()});
  //hardwareController->selectEditSelectBank(value.toInt());
  server.send(200, "text/plain", "Suck me off");
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
